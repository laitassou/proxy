#include <memory>
#include <string>
#include <chrono>

#include "connector.hh"
#include "bucket.hh"

namespace backend {

using namespace std::chrono;

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
    std::string bucket_key = buckets_space + std::string{"/"} + account + std::string{"/"} + bucket;
    std::string val {"1"};
    std::string object_count = bucket_key + std::string{"/objects"};
    std::string cnt {"1"};
    std::string bucket_ctime = bucket_key + std::string{"/ctime"};
    milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
);
    std::map<const char *, const char *> m{
        {bucket_key.c_str(), val.c_str()},
        {bucket_ctime.c_str(), cnt.c_str()},
        {object_count.c_str(), std::to_string(ms.count()).c_str()}
    };
    _db_connector->store_data(m);
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


bucket::bucket(const std::shared_ptr<connector> & connect) {
     _pimpl = std::make_unique<bucket_impl>(connect);
}
bucket::~bucket() {

}
void bucket::create(std::string &account, std::string &bucket){
    _pimpl->create(account, bucket);  
}
void bucket::del(std::string &account, std::string &bucket){
    _pimpl->del(account, bucket);      
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