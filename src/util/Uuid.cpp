#include <util/Uuid.h>

#include <boost/uuid/uuid_generators.hpp>


Uuid::Uuid()
    : boost::uuids::uuid(boost::uuids::random_generator()())
{
}


Uuid::~Uuid()
{
}


