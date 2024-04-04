#pragma once

#include <string>
#include <memory>
#include <functional>

#define FDB_API_VERSION 630
#include <foundationdb/fdb_c.h>

#include <thread>


#include "connector.hh"


namespace backend {

 class fdb_connector : public connector 
 {
        struct fdb_handle : connector::s_handle {
            FDBDatabase * db;
        };
    public:
        virtual ~ fdb_connector() {};
      	//const char * helpMessage() const { return nullptr; };
        fdb_connector(std::string &cluster_path, FDBDatabase * db);
	    fdb_connector (const fdb_connector&) {};
        FDBDatabase * get_handle() {return _db;};
        virtual void store_data(const char *key, const char *val) override;
        virtual void read_data(const char *key) override;

    private:
      	virtual void openImpl() override;
      	virtual void closeImpl() override;


	    fdb_connector(fdb_connector&&) = delete;
	    fdb_connector & operator = (const fdb_connector &) = delete;
	    fdb_connector & operator = (fdb_connector &&) = delete;
    
    private:
        std::string  _cluster_path;
        FDBDatabase *_db;
        std::thread _thread; 

 };

}