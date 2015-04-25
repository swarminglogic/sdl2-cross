#include <net/NetSubscriber.h>


NetSubscriber::NetSubscriber()
    : log_("NetSubscriber"),
      context_(1)
{
  socket_.reset(new zmq::socket_t(context_, ZMQ_SUB));
  socket_->setsockopt(ZMQ_SUBSCRIBE, nullptr, 0u);
}


NetSubscriber::~NetSubscriber()
{
}


void NetSubscriber::connect(const Uuid & uuid,
                            const std::string& connection)
{
  log_.d() << "Connect to: " << connection << Log::end;
  std::lock_guard<std::mutex> guard(mutex_);
  socket_->connect(connection.c_str());
  connections_[uuid] = connection;
}


void NetSubscriber::disconnect(const Uuid & uuid)
{
  const auto it = connections_.find(uuid);
  if (it == connections_.cend()) {
    log_.w() << "Trying to disconnect from uknown node: " << uuid << Log::end;
    return;
  } else {
    log_.d() << "Disconnecting from node  at "
             << it->second << Log::end;
    try {
      std::lock_guard<std::mutex> guard(mutex_);
      socket_->disconnect(it->second.c_str());
      log_.d() << "Disconnect OK" << Log::end;
      connections_.erase(uuid);
    } catch (...) {
      log_.w() << "Disconnect failed" << Log::end;
    }
  }
}

