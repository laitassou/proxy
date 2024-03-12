#include "api_handler.hh"
#include <seastar/http/function_handlers.hh>

namespace ns_proxyserver {

    api_handler::api_handler(const std::unique_ptr<request> &req) : _f_handle(
         [this, _handle](std::unique_ptr<http::request> req, std::unique_ptr<http::reply> rep) {
         return seastar::futurize_invoke(_handle, std::move(req)).then_wrapped([this, rep = std::move(rep)](future<executor::request_return_type> resf) mutable {
             if (resf.failed()) {
                 // Exceptions of type api_error are wrapped as JSON and
                 // returned to the client as expected. Other types of
                 // exceptions are unexpected, and returned to the user
                 // as an internal server error:
                 try {
                     resf.get();
                 } catch (api_error &ae) {
                     generate_error_reply(*rep, ae);
                 } catch (rjson::error & re) {
                     generate_error_reply(*rep,
                             api_error::validation(re.what()));
                 } catch (...) {
                     generate_error_reply(*rep,
                             api_error::internal(format("Internal server error: {}", std::current_exception())));
                 }
                 return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
             }
             auto res = resf.get();
             std::visit(overloaded_functor {
                 [&] (const json::json_return_type& json_return_value) {
                     slogger.trace("api_handler success case");
                     if (json_return_value._body_writer) {
                         // Unfortunately, write_body() forces us to choose
                         // from a fixed and irrelevant list of "mime-types"
                         // at this point. But we'll override it with the
                         // one (application/x-amz-json-1.0) below.
                         rep->write_body("json", std::move(json_return_value._body_writer));
                     } else {
                         rep->_content += json_return_value._res;
                     }
                 },
                 [&] (const api_error& err) {
                     generate_error_reply(*rep, err);
                 }
             }, res);

             return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
         });
    }){};

    future<std::unique_ptr<reply>> api_handler::handle(const sstring& path,
            std::unique_ptr<request> req, std::unique_ptr<reply> rep) override {
        return _f_handle(std::move(req), std::move(rep)).then(
                [](std::unique_ptr<reply> rep) {
                    rep->set_mime_type("application/x-amz-json-1.0");
                    rep->done();
                    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
                });
    }

    void api_handler::generate_error_reply(reply& rep, const api_error& err) {
        rjson::value results = rjson::empty_object();
        if (!err._extra_fields.IsNull() && err._extra_fields.IsObject()) {
            results = rjson::copy(err._extra_fields);
        }
        rjson::add(results, "__type", rjson::from_string("com.amazonaws.dynamodb.v20120810#" + err._type));
        rjson::add(results, "message", err._msg);
        rep._content = rjson::print(std::move(results));
        rep._status = err._http_code;
        slogger.trace("api_handler error case: {}", rep._content);
    }

}
