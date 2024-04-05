#pragma once

#include <memory>
#include <string>

namespace backend {

class bucket {
    class bucket_impl;
    static constexpr auto buckets_space ="buckets";

public:
    explicit bucket(const std::shared_ptr<connector> & connect);
    ~bucket();
    void create(std::string &account, std::string &bucket);
    void del(std::string &account, std::string &bucket);
    void show(std::string &account, std::string &bucket);
    void list(std::string &account, std::string &marker, std::string &prefix);
    void update(std::string &account, std::string &bucket);

private:
    bucket(bucket&&) = delete;
    bucket& operator=(const bucket&) = delete;
    bucket& operator=(const bucket&&) = delete;

    std::unique_ptr<bucket_impl> _pimpl;
};

}