#pragma once 

#include "gated_handler.hh"
#include "../backend/bucket.hh"

using namespace backend;

namespace ns_proxyserver {
class bucket_create_handler : public gated_handler  {
public:
    bucket_create_handler(seastar::gate& pending_requests,
    const std::shared_ptr<bucket> & _bucket) :
        gated_handler(pending_requests), _bucket(_bucket) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;

private:
    std::shared_ptr<bucket> _bucket;
};

class bucket_destroy_handler : public gated_handler  {
public:
    bucket_destroy_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class bucket_list_handler : public gated_handler  {
public:
    bucket_list_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class bucket_show_handler : public gated_handler  {
public:
    bucket_show_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class bucket_get_props_handler : public gated_handler  {
public:
    bucket_get_props_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class bucket_set_props_handler : public gated_handler  {
public:
    bucket_set_props_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

class bucket_del_props_handler : public gated_handler  {
public:
    bucket_del_props_handler(seastar::gate& pending_requests) :
        gated_handler(pending_requests) {};
protected:
    virtual future<std::unique_ptr<reply>> do_handle(const sstring& path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;
};

}