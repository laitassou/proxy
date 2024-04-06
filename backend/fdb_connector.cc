
#include <fmt/format.h>
#include "fdb_connector.hh"
#include <thread>

#include <iostream>
#include <map>

namespace backend
{

class fdb_exception : public std::exception {
public:
  explicit fdb_exception(std::string message) : std::exception(), _message(std::move(message)) {
  }

  [[nodiscard]] const char *what() const noexcept override {
	return _message.c_str();
  }

private:
  std::string _message;
};

fdb_connector::fdb_connector(std::string &cluster_path, FDBDatabase * db):
_cluster_path(cluster_path) {
    _db = db;
    

}

void fdb_connector::openImpl() 
{
	  if (auto error = fdb_select_api_version(FDB_API_VERSION); error) {
		throw fdb_exception(fmt::format("Error Selecting version: {}", fdb_get_error(error)));
	  }

	  // Option debug trace
	  // fdb_network_set_option(FDBNetworkOption::FDB_NET_OPTION_TRACE_ENABLE, ...)

	  if (auto error = fdb_setup_network(); error) {
		throw fdb_exception(fmt::format("Error setup network: {}", fdb_get_error(error)));
	  }

	  _thread = std::thread([]() {
		if (auto error = fdb_run_network(); error) {
		  throw fdb_exception(fmt::format("Error while starting network: {}", fdb_get_error(error)));
		}
	  });
	

	if (auto error = fdb_create_database(_cluster_path.c_str(), &_db); error) {
	  throw fdb_exception(fmt::format("Error creating DB: {}", fdb_get_error(error)));
	}  

     
}

void fdb_connector::closeImpl() 
{   
    fdb_stop_network();
    fdb_database_destroy(_db); 
    _thread.join();
}

void fdb_connector::store_data(std::map<const char *, const char *> &mp)
{
    int committed = 0;
    //  create transaction
    FDBTransaction *tr;
    fdb_database_create_transaction(_db, &tr);
    while(!committed)
    {
        //  store data
        for (std::map<const char *,const char *>::iterator it=mp.begin(); it!=mp.end(); ++it){
            std::cout << it->first << " => " << it->second << '\n';
            fdb_transaction_set(tr, (const uint8_t *)(it->first), (int)strlen(it->first),\
                (const uint8_t *)(it->second), (int)strlen(it->second));
        }
        //  commit to database
        FDBFuture *commitFuture = fdb_transaction_commit(tr);
        fdb_future_block_until_ready(commitFuture);
        committed = 1;

        fdb_future_destroy(commitFuture);
    }
    //  destroy transaction
    fdb_transaction_destroy(tr);
}

void fdb_connector::read_data(const char *key)
{
    //  get value
    FDBTransaction *tr;
    fdb_database_create_transaction(_db, &tr);
    FDBFuture *getFuture = fdb_transaction_get(tr, (const uint8_t *)key, (int)strlen(key), 0);
    fdb_bool_t valuePresent;
    const uint8_t *value;
    int valueLength;
    fdb_future_get_value(getFuture, &valuePresent, &value, &valueLength);
    std::cout << "value for key: " << key << "is "<< value << "\n";
    fdb_transaction_destroy(tr);
    fdb_future_destroy(getFuture);
}

void fdb_connector::clear_key(const char *key)
{
    //  get value
    FDBTransaction *tr;
    fdb_database_create_transaction(_db, &tr);
    fdb_transaction_clear(tr, (const uint8_t *)key, (int)strlen(key));
    //  commit to database
    FDBFuture *commitFuture = fdb_transaction_commit(tr);
    fdb_future_block_until_ready(commitFuture);
    fdb_future_destroy(commitFuture);
}

}