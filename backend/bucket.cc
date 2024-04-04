#include <memory>
#include <string>

#include "connector.hh"
#include "bucket.hh"

namespace backend {


class bucket::bucket_impl {
    public:
    bucket_impl(const std::shared_ptr<connector> & connect);
    void create(std::string &account, std::string &bucket);
    void del(std::string &account, std::string &bucket);
    void show(std::string &account, std::string &bucket);
    void list(std::string &account, std::string &marker, std::string &prefix);
    void update(std::string &account, std::string &bucket);

    private:
        std::shared_ptr<connector> _db_connector;
};

bucket::bucket_impl::bucket_impl(const std::shared_ptr<connector> & connect):
_db_connector(connect){}

void bucket::bucket_impl::create(std::string &account, std::string &bucket){
    std::string concated = buckets_space + std::string{"/"} + account + std::string{"/"} + bucket;
    _db_connector->store_data(concated.c_str(), "1");
}
void bucket::bucket_impl::del(std::string &account, std::string &bucket){
}
void bucket::bucket_impl::show(std::string &account, std::string &bucket){
        std::string concated = buckets_space + std::string{"/"} + account + std::string{"/"} + bucket;
    _db_connector->read_data(concated.c_str());
}
void bucket::bucket_impl::list(std::string &account, std::string &marker, std::string &prefix){
}
void bucket::bucket_impl::update(std::string &account, std::string &bucket){
}


    
void bucket::create(std::string &account, std::string &bucket){
    _pimpl->create(account, bucket);  
}
void bucket::del(std::string &account, std::string &bucket){
    _pimpl->create(account, bucket);      
}
void bucket::show(std::string &account, std::string &bucket){
    _pimpl->show(account, bucket);      
}
void bucket::list(std::string &account, std::string &marker, std::string &prefix){
    _pimpl->list(account, marker, prefix);
}
void bucket::update(std::string &account, std::string &bucket){
    _pimpl->update(account, bucket);
}

}