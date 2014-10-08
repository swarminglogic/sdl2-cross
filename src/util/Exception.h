#ifndef UTIL_EXCEPTION_H
#define UTIL_EXCEPTION_H

#include <string>


/**
 * Basic exception class.
 *
 * @author Roald Fernandez
 */
class Exception
{
 public:
  explicit Exception(const std::string& msg);

  virtual ~Exception();

  virtual const std::string& toString() const;

  /**
   * @throw Exception (itself)
   */
  [[ noreturn ]]
  virtual void raise() const;
  virtual const char* what() const;

 private:
  std::string msg_;
};

#endif  // UTIL_EXCEPTION_H
