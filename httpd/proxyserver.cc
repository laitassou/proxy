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

#include <seastar/core/coroutine.hh>
#include <seastar/json/json_elements.hh>
#include <seastar/util/short_streams.hh>

#include "utils/rjson.hh"

#include "bucket_handlers.hh"
#include "object_handlers.hh"
#include "conscience_handlers.hh"


#include "../backend/fdb_connector.hh"

#include <memory>

namespace ns_proxyserver {


static constexpr auto TARGET = "X-Amz-Target";

static constexpr auto fdb_path ="/home/laitasso/.oio/sds/conf/OPENIO-fdb.cluster";

inline std::vector<std::string_view> split(std::string_view text, char separator) {
    std::vector<std::string_view> tokens;
    if (text == "") {
        return tokens;
    }

    while (true) {
        auto pos = text.find_first_of(separator);
        if (pos != std::string_view::npos) {
            tokens.emplace_back(text.data(), pos);
            text.remove_prefix(pos + 1);
        } else {
            tokens.emplace_back(text);
            break;
        }
    }
    return tokens;
}

proxyserver::proxyserver(const std::string_view &name):
_http_server("test")
,_pending_requests{} {
    std::string cluster_file = std::string{fdb_path};
   // std::cout << name << "\n"; 
   FDBDatabase *database {};
    _db_connector = std::make_shared<fdb_connector>(cluster_file, database);
    _bucket = std::make_shared<bucket>(_db_connector);
    _object =  std::make_shared<object>(_db_connector); 
};

future<>  proxyserver::init(net::inet_address &addr, uint16_t port) {
    //_memory_limiter = std::make_shared<semaphore>(0);
    if (!port) {
        return make_exception_future<>(std::runtime_error("port not specified"));
    }
    return seastar::async([this, addr, port] {
        _db_connector->open();
        set_routes(_http_server._routes);
        _http_server.listen(socket_address{addr, port}).get();
    });
};
future<>  proxyserver::stop() {
    return seastar::async([&] {
        _db_connector->close();
     });
};

void proxyserver::set_routes(seastar::httpd::routes& r){

        api_handler* req_handler = new api_handler([this] (
            std::unique_ptr<request> req) mutable {
        return handle_api_request(std::move(req));
    });

    std::string version {"v3.0"};
    std::string ns {"OPENIO"};

    std::cout << "set routes" << "\n";
    r.put(operation_type::POST, "/test", req_handler);
    //r.put(operation_type::GET, "/test", req_handler);

    // Container routes
    r.put(operation_type::POST, "/" + version + "/" + ns + "/container/create", new bucket_create_handler(_pending_requests, _bucket));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/container/destroy", new bucket_destroy_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/container/list", new bucket_list_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/container/show", new bucket_show_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/container/get_properties", new bucket_get_props_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/container/set_properties", new bucket_set_props_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/container/del_properties", new bucket_del_props_handler(_pending_requests));


    // Object routes
    r.put(operation_type::POST, "/" + version + "/" + ns + "/content/create", new object_create_handler(_pending_requests, _object));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/content/prepare2", new object_prepare_handler(_pending_requests, _object));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/content/delete", new object_delete_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/content/show", new object_show_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/content/locate", new object_locate_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/content/get_properties", new object_get_props_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/content/set_properties", new object_set_props_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/content/del_properties", new object_del_props_handler(_pending_requests));

    // Consience routes
    //r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/info", new object_create_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/conscience/list", new object_delete_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/register", req_handler);
    r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/deregister", new object_show_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/flush", new object_show_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/lock", new object_show_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/unlock", new object_show_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/conscience/resolve", new object_show_handler(_pending_requests));

    // Load balancing routes
    //r.put(operation_type::POST, "/" + version + "/" + ns + "/lb/reload", new object_create_handler(_pending_requests));
    r.put(operation_type::GET, "/" + version + "/" + ns + "/lb/choose", new object_delete_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/lb/poll", new object_show_handler(_pending_requests));
    r.put(operation_type::POST, "/" + version + "/" + ns + "/lb/create_pool", new object_show_handler(_pending_requests));

};

future<request_return_type> proxyserver::handle_api_request(std::unique_ptr<http::request> req){
    sstring target = req->get_header(TARGET);
    std::cout << "target " << target << "\n";
    std::vector<std::string_view> split_target = split(target, '.');
    std::cout << req->_method << "\n";
    for (const auto& p : req->query_parameters) {
        std::cout << p.first << "\n";

    }
    /*
    size_t mem_estimate = req->content_length * 2 + 8000;
    auto units_fut = get_units(*(_memory_limiter.get()), mem_estimate);

    auto units = co_await std::move(units_fut);
    */
    //assert(req->content_stream);
    //rjson::chunked_content content = co_await util::read_entire_stream(*req->content_stream);

    rjson::value json_request = co_await _json_parser.parse(req.get()->content);
    for (auto iter = json_request.MemberBegin(); iter != json_request.MemberEnd(); ++iter){
        std::cout << "key:" << iter->name.GetString() << ":" <<  iter->value.GetString() << "\n";
        //_db_connector->store_data(iter->name.GetString(), iter->value.GetString());
        std::string k=iter->name.GetString();
        std::string v=iter->value.GetString();
        //_bucket->create(k,v);
    }
    rjson::value response = rjson::empty_object();
    //rjson::add(response, "Table", "ok");

    co_return  co_await make_ready_future<request_return_type>(rjson::print(std::move(response)));
};

proxyserver::json_parser::json_parser() : _run_parse_json_thread(async([this] {
        while (true) {
            _document_waiting.wait().get();
            if (_as.abort_requested()) {
                return;
            }
            try {
                _parsed_document = rjson::parse_yieldable(std::move(_raw_document));
                _current_exception = nullptr;
            } catch (...) {
                _current_exception = std::current_exception();
            }
            _document_parsed.signal();
        }
    })) {
}

future<rjson::value> proxyserver::json_parser::parse(sstring & content) {
    if (content.size() < yieldable_parsing_threshold) {
        return make_ready_future<rjson::value>(rjson::parse(content.c_str()));
    }
    return with_semaphore(_parsing_sem, 1, [this, content = std::move(content)] () mutable {
        _raw_document = std::move(content);
        _document_waiting.signal();
        return _document_parsed.wait().then([this] {
            if (_current_exception) {
                return make_exception_future<rjson::value>(_current_exception);
            }
            return make_ready_future<rjson::value>(std::move(_parsed_document));
        });
    });
}

future<> proxyserver::json_parser::stop() {
    _as.request_abort();
    _document_waiting.signal();
    _document_parsed.broken();
    return std::move(_run_parse_json_thread);
}


}
