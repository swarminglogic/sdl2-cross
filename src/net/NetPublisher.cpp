#include <net/NetPublisher.h>



NetPublisher::NetPublisher(const Uuid& uuid, const std::string& hostname)
    : log_("NetPublisher"),
      uuid_(uuid),
      context_(1),
      hostname_(hostname)
{
  createSocket();
}


NetPublisher::~NetPublisher()
{
}


int NetPublisher::getSocketPort() const
{
  return port_;
}


void NetPublisher::createSocket()
{
  const int N_ALLOWED_BIND_ATTEMPTS = 64;
  const int INIT_SOCKET_ATTEMPT = 52032;

  socket_.reset(new zmq::socket_t(context_, ZMQ_PUB));

  int socketPort = 0;
  std::string bindString;
  for (int i = 0 ; i < N_ALLOWED_BIND_ATTEMPTS ; ++i) {
    try {
      socketPort = INIT_SOCKET_ATTEMPT + i;
      bindString = std::string("tcp://") + hostname_ + std::string(":") +
          std::to_string(socketPort);
      // Throws on bind failure
      socket_->bind(bindString.c_str());
      port_ = socketPort;
      log_.i() << "Publishing socket bound to: " << bindString << Log::end;
      return;
    }
    catch (...) {
      log_.d() << "Failed to bind socket: " << socketPort << Log::end;
      continue;
    }
    log_.e() << "Failed to acquire bindable socket" << Log::end;
    throw Exception("Socket bind error");
  }
}
