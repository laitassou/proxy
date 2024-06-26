#pragma once

#include <string>
#include <memory>
#include <functional>

#include <map>

namespace backend {

 class connector
 {
    public:
    struct handle {};
    using s_handle = struct handle;
        virtual ~ connector() {};
      	connector * clone () const;
      	//const char * helpMessage() const;
        void open() ;
        void close();
        virtual void store_data(std::map<const char *, const char *> &mp) {};
        virtual void read_data(const char *key) {};
        virtual void clear_key(const char *key) {};

    protected:
        connector() {};
	    connector (const connector&) {};

    private:
      	virtual void openImpl()  = 0;
      	virtual void closeImpl()  = 0;

	    //virtual const char * helpMessageImpl() const noexcept = 0;

	    connector(connector&&) = delete;
	    connector & operator = (const connector &) = delete;
	    connector & operator = (connector &&) = delete;
 };

}