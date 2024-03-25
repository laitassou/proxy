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

class gated_handler : public handler_base {
    seastar::gate& _gate;
public:
    gated_handler(seastar::gate& gate) : _gate(gate) {}
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) = 0;
    virtual future<std::unique_ptr<reply>> handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) final override {
        return with_gate(_gate, [this, &path, req = std::move(req), rep = std::move(rep)] () mutable {
            return do_handle(path, std::move(req), std::move(rep));
        });
    }
};
}