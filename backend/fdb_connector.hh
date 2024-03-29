#pragma once

#include <string>
#include <memory>
#include <functional>

#define FDB_API_VERSION 610
#include <foundationdb/fdb_c.h>

#include <thread>


#include "connector.hh"


namespace backend {

 class fdb_connector : public connector 
 {
    public:
        virtual ~ fdb_connector() {};
      	//const char * helpMessage() const { return nullptr; };
        fdb_connector(std::string &cluster_path, FDBDatabase * db);
	    fdb_connector (const fdb_connector&) {};

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