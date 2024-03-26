#include "connector.hh"


namespace backend
{

void connector::open()
{
    return openImpl();
}


void connector::close()
{
    return closeImpl();
}
connector * connector::clone() const
{
}
const char* connector::helpMessage() const
{
    return helpMessageImpl();
}

}