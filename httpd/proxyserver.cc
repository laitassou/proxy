#include "proxyserver.hh"

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
    _http_server = std::make_unique<httpd::http_server_control>();
    std::cout << name << "\n";
};

future<>  proxyserver::init(net::inet_address addr, std::optional<uint16_t> port, std::optional<uint16_t> https_port) {
    return seastar::async([&] { });
};
future<>  proxyserver::stop() {
    return seastar::async([&] { });
};

void proxyserver::set_routes(seastar::httpd::routes& r){};
future<>  proxyserver::handle_api_request(std::unique_ptr<http::request> req){
    return seastar::async([&] { });
};
}