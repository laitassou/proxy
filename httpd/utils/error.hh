#pragma once

#include <seastar/http/httpd.hh>
#include <seastar/http/common.hh>

#include "seastarx.hh"
#include "utils/rjson.hh"

using namespace seastar::http;

namespace ns_proxyserver {

// api_error contains a DynamoDB error message to be returned to the user.
// It can be returned by value (see executor::request_return_type) or thrown.
// The DynamoDB's error messages are described in detail in
// https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Programming.Errors.html
// An error message has an HTTP code (almost always 400), a type, e.g.,
// "ResourceNotFoundException", and a human readable message.
// Eventually alternator::api_handler will convert a returned or thrown
// api_error into a JSON object, and that is returned to the user.
class api_error final : public std::exception {
public:
    using status_type = http::reply::status_type;
    status_type _http_code;
    std::string _type;
    std::string _msg;
    // Additional data attached to the error, null value if not set. It's wrapped in copyable_value
    // class because copy constructor is required for exception classes otherwise it won't compile
    // (despite that its use may be optimized away).
    rjson::copyable_value _extra_fields; 
    api_error(std::string type, std::string msg, status_type http_code = status_type::bad_request,
    rjson::value extra_fields = rjson::null_value())
        : _http_code(std::move(http_code))
        , _type(std::move(type))
        , _msg(std::move(msg))
        , _extra_fields(std::move(extra_fields))
    { }

    // Factory functions for some common types of DynamoDB API errors
    static api_error validation(std::string msg) {
        return api_error("ValidationException", std::move(msg));
    }
    static api_error resource_not_found(std::string msg) {
        return api_error("ResourceNotFoundException", std::move(msg));
    }

    // Provide the "std::exception" interface, to make it easier to print this
    // exception in log messages.
    virtual const char* what() const noexcept override;
    mutable std::string _what_string;
};

}