#pragma once


#include <memory>
#include <string>

namespace backend {


class object {
    class object_impl;

public:
    static object& Instance();
    void put();
    void del();
    void get();
    void list();
    void update();

private:
    object();
    ~object();
    object(const object&) = delete;
    object(object&&) = delete;
    object& operator=(const object&) = delete;
    object& operator=(const object&&) = delete;

    std::unique_ptr<object_impl> _pimpl;
};

}