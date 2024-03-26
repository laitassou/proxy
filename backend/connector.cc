#include "connector.hh"


namespace backend
{


void connector::open() const
{
  return;
}

connector * connector::clone() const
{
     return cloneImpl();
}
const char* connector::helpMessage() const
{
    return helpMessageImpl();
}

void connector::deallocate()
{
    delete this;
}

}