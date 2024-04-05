#include "gated_handler.hh"

namespace ns_proxyserver {
class admin_handler : public gated_handler  {
public:
    admin_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};
}