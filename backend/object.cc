#include <memory>
#include <string>
#include <chrono>

#include "connector.hh"
#include "object.hh"

namespace backend {

using namespace std::chrono;

class object::object_impl {
    public:
    object_impl(const std::shared_ptr<connector> & connect);
    void put(std::string &account, std::string &bucket, std::string &objec);
    void del(std::string &account, std::string &bucket, std::string &objec);
    void show(std::string &account, std::string &bucket, std::string &objec);
    void list(std::string &account, std::string &bucket, std::string &marker, std::string &prefix);
    void update(std::string &account, std::string &bucket, std::string &objec);

    private:
        std::shared_ptr<connector> _db_connector;
};

object::object_impl::object_impl(const std::shared_ptr<connector> & connect):
_db_connector(connect){}

void object::object_impl::put(std::string &account, std::string &bucket, std::string &ref){
    std::string object_key = objects_space + std::string{"/"} + account + std::string{"/"} + bucket + std::string{"/"} + ref;
    std::string val {"1"};
    std::string object_version = object_key + std::string{"/version"};
    std::string version {"0"};
    std::string object_ctime = object_key + std::string{"/ctime"};
    milliseconds ms = duration_cast< milliseconds>(
        system_clock::now().time_since_epoch()
        );
    std::map<const char *, const char *> m{
        {object_key.c_str(), val.c_str()},
        {object_ctime.c_str(), std::to_string(ms.count()).c_str()},
        {object_version.c_str(), version.c_str()}
    };
    _db_connector->store_data(m);

    //_db_connector->store_data(concated.c_str(), "1");
}
void object::object_impl::del(std::string &account, std::string &bucket, std::string &object){
}
void object::object_impl::show(std::string &account, std::string &bucket, std::string &object){
        std::string concated = objects_space + std::string{"/"} + account + std::string{"/"} + bucket;
    _db_connector->read_data(concated.c_str());
}
void object::object_impl::list(std::string &account, std::string &bucket, std::string &marker, std::string &prefix){
}
void object::object_impl::update(std::string &account, std::string &bucket, std::string &object){
}

object::object(const std::shared_ptr<connector> & connect) {
    _pimpl = std::make_unique<object_impl>(connect);
}
object::~object() {
}   
void object::put(std::string &account, std::string &bucket, std::string &object){
    _pimpl->put(account, bucket, object);  
}
void object::del(std::string &account, std::string &bucket, std::string &object){
    _pimpl->del(account, bucket, object);      
}
void object::show(std::string &account, std::string &bucket, std::string &object){
    _pimpl->show(account, bucket, object);      
}
void object::list(std::string &account, std::string &bucket, std::string &marker, std::string &prefix){
    _pimpl->list(account, bucket, marker, prefix);
}
void object::update(std::string &account, std::string &bucket, std::string &object){
    _pimpl->update(account, bucket, object);
}

}