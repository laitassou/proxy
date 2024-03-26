#pragma once


#include <memory>
#include <string>

namespace backend {


class repository {
    class repositoryImpl;

public:
    static repository& Instance();
    

private:
    repository();
    ~repository();
    repository(const repository&) = delete;
    repository(repository&&) = delete;
    repository& operator=(const repository&) = delete;
    repository& operator=(const repository&&) = delete;

    std::unique_ptr<repositoryImpl> pimpl_;
};

}