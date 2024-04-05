#pragma once

#include <memory>
#include <string>

namespace backend {

class object {
    class object_impl;
    static constexpr auto objects_space ="objects";

public:
    explicit object(const std::shared_ptr<connector> & connect);
    ~object();

    void put(std::string &account, std::string &bucket, std::string &object);
    void del(std::string &account, std::string &bucket, std::string &object);
    void show(std::string &account, std::string &bucket, std::string &object);
    void list(std::string &account, std::string &bucket, std::string &marker, std::string &prefix);
    void update(std::string &account, std::string &bucket, std::string &object);

private:
    object& operator=(const object&) = delete;
    object& operator=(const object&&) = delete;

    std::unique_ptr<object_impl> _pimpl;
};

}