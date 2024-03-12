#include "proxyserver.hh"
#include "api_handler.hh"

#include <seastar/http/httpd.hh>
#include <seastar/http/handlers.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/file_handler.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/reactor.hh>
#include <seastar/http/api_docs.hh>
#include <seastar/core/thread.hh>
#include <seastar/core/prometheus.hh>
#include <seastar/core/print.hh>
#include <seastar/core/sleep.hh>
#include <seastar/net/inet_address.hh>
#include <seastar/util/defer.hh>

#include <memory>

namespace ns_proxyserver {


proxyserver::proxyserver(const std::string_view &name) {
    _http_server = std::make_unique<httpd::http_server>();
    std::cout << name << "\n";
};

future<>  proxyserver::init(net::inet_address addr, uint16_t port) {
    if (!port) {
        return make_exception_future<>(std::runtime_error("port not specified"));
    }
    set_routes(_http_server.get()->_routes);

    return seastar::async([&] { });
};
future<>  proxyserver::stop() {
    return seastar::async([&] { });
};

void proxyserver::set_routes(seastar::httpd::routes& r){
        api_handler* req_handler = new api_handler([this] (
            std::unique_ptr<request> req) mutable {
        return handle_api_request(std::move(req));
    });

    r.put(operation_type::POST, "/", req_handler);
    r.put(operation_type::GET, "/", req_handler);

};

future<>  proxyserver::handle_api_request(std::unique_ptr<http::request> req){
    return seastar::async([&] { });
};
}