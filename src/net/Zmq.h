#ifndef NET_ZMQ_H
#define NET_ZMQ_H

#include <zmq.hpp>
#include <czmq.h>

namespace net {
namespace czmq {
struct Czmq_Deleter {
  void operator()(zactor_t*  ptr) { if (ptr) zactor_destroy(&ptr); }
  void operator()(zframe_t* ptr) { if (ptr) zframe_destroy(&ptr); }
};

using zactor_ptr  = std::unique_ptr<zactor_t,  Czmq_Deleter>;
using zframe_ptr  = std::unique_ptr<zframe_t,  Czmq_Deleter>;
}

namespace zmq {
struct Zmq_Deleter {
  void operator()(zmsg_t*  ptr) { if (ptr) zmsg_destroy(&ptr); }
};
using zmsg_ptr  = std::unique_ptr<zmsg_t,  Zmq_Deleter>;
}
}


#endif  // NET_ZMQ_H
