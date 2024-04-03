#pragma once


#include <memory>
#include <string>

namespace backend {


template<class T> class bucket {
    class bucket_impl;

public:
    static bucket& Instance();
    void put();
    void del();
    void get();
    void list();
    void update();

private:
    bucket();
    ~bucket();
    bucket(const bucket&) = delete;
    bucket(bucket&&) = delete;
    bucket& operator=(const bucket&) = delete;
    bucket& operator=(const bucket&&) = delete;

    std::unique_ptr<bucket_impl> _pimpl;
};

}