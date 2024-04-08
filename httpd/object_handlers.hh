#pragma once 

#include "gated_handler.hh"
#include "../backend/object.hh"

namespace ns_proxyserver {

using namespace backend;

class object_create_handler : public gated_handler  {
public:
    object_create_handler(seastar::gate& pending_requests,
        const std::shared_ptr<object> & _object) :
        gated_handler(pending_requests), _object(_object) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
private:
    const std::shared_ptr<object> _object;
};

class object_prepare_handler : public gated_handler  {
public:
    object_prepare_handler(seastar::gate& pending_requests,
        const std::shared_ptr<object> & _object) :
        gated_handler(pending_requests), _object(_object) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
private:
    const std::shared_ptr<object> _object;
};

class object_delete_handler : public gated_handler  {
public:
    object_delete_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class object_show_handler : public gated_handler  {
public:
    object_show_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class object_locate_handler : public gated_handler  {
public:
    object_locate_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class object_get_props_handler : public gated_handler  {
public:
    object_get_props_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class object_set_props_handler : public gated_handler  {
public:
    object_set_props_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class object_del_props_handler : public gated_handler  {
public:
    object_del_props_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

}