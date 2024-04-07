#include "object_handlers.hh"

namespace ns_proxyserver {

future<std::unique_ptr<reply>> object_create_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();

    std::string account{};
    std::string ref{};
    std::string obj{};

    for (const auto& p : req->query_parameters) {
        std::cout << p.first << "\n";
        if (std::string{"acct"} == p.first) {
            account = p.second;
        }
        if (std::string{"ref"} == p.first) {
            ref = p.second;
        }
       if (std::string{"path"} == p.first) {
            obj = p.second;
        }
    }

    std::cout << "object create: "<< account << ","<< ref << "," << path<< "\n";
    _object->put(account, ref, obj);

    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> object_delete_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> object_show_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> object_locate_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> object_get_props_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> object_set_props_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> object_del_props_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};

}