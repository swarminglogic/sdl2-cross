#ifndef UTIL_UUID_H
#define UTIL_UUID_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

/**
 * Uuid class.
 *
 * @author SwarmingLogic
 */
class Uuid : public boost::uuids::uuid
{
 public:
  Uuid();
  ~Uuid();
 private:
};


#endif  // UTIL_UUID_H
