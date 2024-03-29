
#include <fmt/format.h>
#include "fdb_connector.hh"
#include <thread>


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
    fdb_database_destroy(_db); 
    _thread.join();
}


}