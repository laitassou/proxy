#pragma once

#include <string>
#include <memory>
#include <functional>

namespace backend {

 class connector
 {
    public:
        virtual ~ connector() {};
      	connector * clone () const;
      	const char * helpMessage() const;
      	virtual void deallocate();

    protected:
        connector() {};
	    connector (const connector&) {};
    private:
        virtual void open() const;
      	virtual void openImpl() noexcept = 0;
      	virtual void closeImpl() noexcept = 0;

      	virtual connector* cloneImpl() const = 0;

	 virtual const char * helpMessageImpl() const noexcept = 0;

	 connector(connector&&) = delete;
	 connector & operator = (const connector &) = delete;
	 connector & operator = (connector &&) = delete;
 };

}