#include <util/Exception.h>

#include <string>


Exception::Exception(const std::string& msg)
  : msg_(msg)
{
}


Exception::~Exception()
{
}


const std::string& Exception::toString() const
{
  return msg_;
}

[[ noreturn ]]
void Exception::raise() const
{
  throw *this;
}


const char* Exception::what() const
{
  return msg_.c_str();
}
