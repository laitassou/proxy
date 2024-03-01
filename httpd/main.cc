/*
 * This file is open source software, licensed to you under the terms
 * of the Apache License, Version 2.0 (the "License").  See the NOTICE file
 * distributed with this work for additional information regarding copyright
 * ownership.  You may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright 2015 Cloudius Systems
 */

#include <seastar/http/httpd.hh>
#include <seastar/http/handlers.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/file_handler.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/reactor.hh>
//#include "demo.json.hh"
#include <seastar/http/api_docs.hh>
#include <seastar/core/thread.hh>
#include <seastar/core/prometheus.hh>
#include <seastar/core/print.hh>
#include <seastar/core/sleep.hh>
#include <seastar/net/inet_address.hh>
#include <seastar/util/defer.hh>
#include "../lib/stop_signal.hh"

#include "proxyserver.hh"

#include <iostream>

namespace bpo = boost::program_options;

using namespace seastar;
using namespace httpd;

using namespace ns_proxyserver; 

class handl : public httpd::handler_base {
public:
    virtual future<std::unique_ptr<http::reply> > handle(const sstring& path,
            std::unique_ptr<http::request> req, std::unique_ptr<http::reply> rep) {
        rep->_content = "hello";
        rep->done("html");
        return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
    }
};

void set_routes(routes& r) {
    function_handler* h1 = new function_handler([](const_req req) {
		    std::cout << req.content << "\n";
			//for (auto& pm:req.param)
		    	//std::cout<< pm.first << pm.second << "\n";

	  return "hello";
    });
    function_handler* h2 = new function_handler([](std::unique_ptr<http::request> req) {
        return make_ready_future<json::json_return_type>("{code: 200, status: Ok}");
    });
    function_handler* h3 = new function_handler([](const_req req) {
         std::cout << "h3" << req.content_stream << "\n";
		    return "create";
    });
    function_handler* h4 = new function_handler([](const_req req) {
        return "prepare";
    });

     function_handler* h5 = new function_handler([](std::unique_ptr<http::request> req) {
        return make_ready_future<json::json_return_type>("[{'addr':'127.0.0.1:6008','score':80,'scores':{'score.put':80,'score.get':80},'tags':{'tag.slots':'meta2','tag.loc':'rack.127-0-0-1.1','tag.getlock':false,'tag.putlock':false,'tag.vol':'/home/laitasso/.oio/sds/data/OPENIO-meta2-1','tag.lock':false,'tag.up':true,'tag.service_id':'OPENIO-meta2-1'}}]");
	});



    r.add(operation_type::GET, url("/"), h1);
    r.add(operation_type::GET, url("/jf"), h2);
    r.add(operation_type::GET, url("/file").remainder("path"),
            new directory_handler("/"));

    r.add(operation_type::POST, url("/v3.0/OPENIO/content/create"), h3);
    r.add(operation_type::POST, url("/v3.0/OPENIO/content/prepare"), h4);
  
    r.add(operation_type::POST, url("/v3.0/OPENIO/conscience/register"), h2);

    r.add(operation_type::GET, url("/v3.0/OPENIO/conscience/list?"), h5);

/*
    demo_json::hello_world.set(r, [] (const_req req) {
        demo_json::my_object obj;
        obj.var1 = req.param.at("var1");
        obj.var2 = req.param.at("var2");
        demo_json::ns_hello_world::query_enum v = demo_json::ns_hello_world::str2query_enum(req.query_parameters.at("query_enum"));
        // This demonstrate enum conversion
        obj.enum_var = v;
        return obj;
    });
*/
}

seastar::future<> f() {
    seastar::thread th([] {
        std::cout << "Hi.\n";
        for (int i = 1; ; i++) {
            seastar::sleep(std::chrono::seconds(1)).get();
            std::cout << i << "\n";
        }
    });
    return do_with(std::move(th), [] (auto& th) {
        return th.join();
    });
}

int main(int ac, char** av) {
    httpd::http_server_control prometheus_server;
    prometheus::config pctx;
    app_template app;

    std::unique_ptr<ns_proxyserver::proxyserver> proxy = std::make_unique<ns_proxyserver::proxyserver>("proxy");

    app.add_options()("port", bpo::value<uint16_t>()->default_value(10000), "HTTP Server port");
    app.add_options()("prometheus_port", bpo::value<uint16_t>()->default_value(9180), "Prometheus port. Set to zero in order to disable.");
    app.add_options()("prometheus_address", bpo::value<sstring>()->default_value("0.0.0.0"), "Prometheus address");
    app.add_options()("prometheus_prefix", bpo::value<sstring>()->default_value("seastar_httpd"), "Prometheus metrics prefix");

    int MAX = 10;
    seastar::queue<int> _data = seastar::queue<int>(MAX);

    auto a_lambda = [&]() -> void /*-> seastar::future<>*/ {

        std::cout << "Hi.\n";

        while (true) {
            //auto v = co_await _data.pop_eventually();
            std::cout << "Got data: " << MAX << "\n";
            seastar::sleep(std::chrono::seconds(1)).get();
        }
    };

    
    return app.run(ac, av, [&] {
        return seastar::async([&] {
            seastar_apps_lib::stop_signal stop_signal;
            auto&& config = app.configuration();
            httpd::http_server_control prometheus_server;
            bool prometheus_started = false;

            auto stop_prometheus = defer([&] () noexcept {
                if (prometheus_started) {
                    std::cout << "Stoppping Prometheus server" << std::endl;  // This can throw, but won't.
                    prometheus_server.stop().get();
                }
            });

            uint16_t pport = config["prometheus_port"].as<uint16_t>();
            if (pport) {
                prometheus::config pctx;
                net::inet_address prom_addr(config["prometheus_address"].as<sstring>());

                pctx.metric_help = "seastar::httpd server statistics";
                pctx.prefix = config["prometheus_prefix"].as<sstring>();

                std::cout << "starting prometheus API server" << std::endl;
                prometheus_server.start("prometheus").get();

                prometheus::start(prometheus_server, pctx).get();

                prometheus_started = true;

                prometheus_server.listen(socket_address{prom_addr, pport}).handle_exception([prom_addr, pport] (auto ep) {
                    std::cerr << seastar::format("Could not start Prometheus API server on {}:{}: {}\n", prom_addr, pport, ep);
                    return make_exception_future<>(ep);
                }).get();

            }

            uint16_t port = config["port"].as<uint16_t>();
            auto server = new http_server_control();
            auto rb = std::make_shared<api_registry_builder>("apps/httpd/");
            server->start().get();

            auto stop_server = defer([&] () noexcept {
                std::cout << "Stoppping HTTP server" << std::endl; // This can throw, but won't.
                server->stop().get();
            });

            seastar::thread th(a_lambda);

            server->set_routes(set_routes).get();
            server->set_routes([rb](routes& r){rb->set_api_doc(r);}).get();
            server->set_routes([rb](routes& r) {rb->register_function(r, "demo", "hello world application");}).get();
            server->listen(port).get();

            std::cout << "Seastar HTTP server listening on port " << port << " ...\n";

            stop_signal.wait().get();
            th.join();
            return 0;
        });
    });
}
