#pragma once

#include <string>
#include <memory>
#include <functional>

#define FDB_API_VERSION 610
#include <foundationdb/fdb_c.h>

#include "connector.hh"

namespace backend {

 class fdb_connector : public connector 
 {
    public:
        virtual ~ fdb_connector() {};
      	fdb_connector * clone () const;
      	const char * helpMessage() const;

    protected:
        fdb_connector() {};
	    fdb_connector (const fdb_connector&) {};

    private:
      	virtual void openImpl() noexcept override;
      	virtual void closeImpl() noexcept override;


	    fdb_connector(fdb_connector&&) = delete;
	    fdb_connector & operator = (const fdb_connector &) = delete;
	    fdb_connector & operator = (fdb_connector &&) = delete;
 };

}