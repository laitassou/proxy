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

#include "utils/rjson.hh"
#include "utils/error.hh"

using namespace seastar;

using request = seastar::http::request;
using reply = seastar::http::reply;
using namespace httpd;

namespace ns_proxyserver {
class api_handler : public handler_base {
public:
    api_handler(const std::unique_ptr<request> &req);
    api_handler(const api_handler&) = default;
    future<std::unique_ptr<reply>> handle(const sstring& path,
            std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;

protected:
    void generate_error_reply(reply& rep, const api_error& err);

    future_handler_function _f_handle;
};
}