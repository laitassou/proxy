#include "bucket_handlers.hh"

namespace ns_proxyserver {

future<std::unique_ptr<reply>> bucket_create_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    std::cout << req->_method << "\n";
    std::string account{};
    std::string ref{};

    for (const auto& p : req->query_parameters) {
        std::cout << p.first << "\n";
        if (std::string{"acct"} == p.first) {
            account = p.second;
        }
        if (std::string{"ref"} == p.first) {
            ref = p.second;
        }
    }

    std::cout << "bucket create: "<< account << ","<< ref << "\n";
    _bucket->create(account, ref);
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> bucket_destroy_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    std::cout << req->_method << "\n";
    for (const auto& p : req->query_parameters) {
        std::cout << p.first << "\n";

    }
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};

future<std::unique_ptr<reply>> bucket_list_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};

future<std::unique_ptr<reply>> bucket_show_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};


future<std::unique_ptr<reply>> bucket_get_props_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};

future<std::unique_ptr<reply>> bucket_set_props_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};

future<std::unique_ptr<reply>> bucket_del_props_handler::do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)  {
    rjson::value results = rjson::empty_array();
    rep->set_status(reply::status_type::ok);
    rep->set_content_type("json");
    rep->_content = rjson::print(results);
    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
};

}