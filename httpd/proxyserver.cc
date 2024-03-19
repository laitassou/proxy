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


static constexpr auto TARGET = "X-Amz-Target";

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

proxyserver::proxyserver(const std::string_view &name):_http_server("test") {
   // std::cout << name << "\n";
};

future<>  proxyserver::init(net::inet_address &addr, uint16_t port) {
    if (!port) {
        return make_exception_future<>(std::runtime_error("port not specified"));
    }
    set_routes(_http_server._routes);
    _http_server.listen(socket_address{addr, port}).get();

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
    std::cout << "set routes" << "\n";
    r.put(operation_type::POST, "/test", req_handler);
    r.put(operation_type::GET, "/test", req_handler);

};

future<request_return_type> proxyserver::handle_api_request(std::unique_ptr<http::request> req){
    sstring target = req->get_header(TARGET);
    std::vector<std::string_view> split_target = split(target, '.');
    //chunked_content content = co_await util::read_entire_stream(*req->content_stream);
    //rjson::value json_request = co_await _json_parser.parse(std::move(content));
    co_return api_error::resource_not_found(format("too many requests"));
};
}
