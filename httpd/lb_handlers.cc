#include "lb_handlers.hh"

namespace ns_proxyserver {

future<std::unique_ptr<reply>> lb_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
        rjson::value results = rjson::empty_array();
        rep->set_status(reply::status_type::ok);
        rep->set_content_type("json");
        rep->_content = rjson::print(results);
        return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
    };

}