#pragma once

#include <seastar/core/future.hh>
#include <seastar/core/condition-variable.hh>
#include <seastar/http/httpd.hh>
#include <seastar/net/tls.hh>
#include <optional>
#include <seastar/core/units.hh>
#include <seastar/json/json_elements.hh>
#include "utils/rjson.hh"
#include "utils/error.hh"


namespace ns_proxyserver {
//using request_return_type = std::variant<json::json_return_type, api_error>;

using namespace std;
using namespace seastar;
using namespace httpd;

class proxyserver {
    class json_parser {
        static constexpr size_t yieldable_parsing_threshold = 16*KB;
        /*
        chunked_content _raw_document;
        rjson::value _parsed_document;
        std::exception_ptr _current_exception;
        semaphore _parsing_sem{1};
        condition_variable _document_waiting;
        condition_variable _document_parsed;
        abort_source _as;
        future<> _run_parse_json_thread;
    public:
        json_parser();
        future<rjson::value> parse(chunked_content&& content);
        future<> stop();
    */
    };
    json_parser _json_parser;

    public:
    proxyserver(const std::string_view &name);
    future<> init(net::inet_address &addr, uint16_t port);
    future<> stop();

    private:
    httpd::http_server _http_server;
    void set_routes(seastar::httpd::routes& r);
    future<request_return_type> handle_api_request(std::unique_ptr<http::request> req);
};

}