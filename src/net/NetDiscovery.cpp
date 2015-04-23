#include <net/NetDiscovery.h>

#include <cstring>
#include <sstream>
#include <stdexcept>

#include <net/Zmq.h>


NetDiscovery::NetDiscovery()
: log_("NetDiscovery")
{
}



NetDiscovery::~NetDiscovery()
{
}


void NetDiscovery::debugState() const
{
  log_.d() << "Participants:" << Log::end;
  for (auto& p : participants_) {
    log_.d() << p.first << " <-> " << p.second << Log::end;
  }
  log_.w() << "----------------------------------------" << Log::end;
}


void NetDiscovery::init()
{
  log_.i() << "Initializing service discovery" << Log::end;
  log_.d() << "Uuid: " << uuid_ << Log::end;
  log_.e() << "SIZE: " << sizeof(uuid_) << Log::end;
  if (!thread_.joinable()) {
    log_.d() << "Starting thread" << Log::end;
    isRunning_ = true;
    thread_ = std::thread(run);
  }
}

void NetDiscovery::stop()
{
  isRunning_ = false;
  if (thread_.joinable()) {
    log_.d() << "Waiting for NetDiscovery background thread termination."
             << Log::end;
    thread_.join();
  } else {
    log_.w() << "NetDiscovery stopped before calling init()?." << Log::end;
  }
}

void NetDiscovery::suspend()
{
  isSuspended_ = true;
}


void NetDiscovery::resume()
{
  isSuspended_ = false;
}


NetDiscovery& NetDiscovery::instance()
{
  static NetDiscovery instance_;
  return instance_;
}


bool NetDiscovery::extractUuid(zmsg_t *msg)
{
  if (!msg)
    return false;

  if (zmsg_size(msg) != 2)
    return false;

  zframe_t* frame = zmsg_pop(msg);
  zframe_destroy(&frame);

  frame = zmsg_pop(msg);
  assert(zframe_size(frame) == sizeof(Uuid));
  Uuid uuid;
  std::memcpy(&uuid, zframe_data(frame), sizeof(uuid));
  zframe_destroy(&frame);
  instance().log_.d() << uuid << Log::end;
  return uuid == instance().uuid_;
}


void NetDiscovery::run()
{
  const int port = 4242;
  NetDiscovery& instance_ = instance();
  zactor_t* beacon = zactor_new(zbeacon, nullptr);
  zactor_t* subscriber = beacon;
  {
    std::lock_guard<std::mutex> guard(instance_.mutex_);
    zsock_send(beacon, "si", "CONFIGURE", port);
    char *hostname = zstr_recv(beacon);
    if (hostname) {
      instance_.hostname_ = std::string(hostname);
    }
    free(hostname);
    instance_.log_.d() << "Hostname: " << instance_.hostname_ << Log::end;

    // Initializing broadcast, send every 1000 ms
    zsock_send(beacon,
               "sbi",
               "PUBLISH",
               (const char*)&instance_.uuid_,
               sizeof(instance_.uuid_), 1000);

    zsock_send(subscriber, "si", "CONFIGURE", port);
    zsock_send(subscriber, "sb", "SUBSCRIBE", "", 0);
    zsock_set_rcvtimeo(subscriber, 100);
  }

  // Should ignore myself?
  while (instance_.isRunning_) {
    if (!instance_.isSuspended_) {
      std::lock_guard<std::mutex> guard(instance_.mutex_);
      zmsg_t *msg = zmsg_recv(subscriber);
      if (extractUuid(msg))
        continue;
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  instance_.log_.e() << "Destroying zmq stuff" << Log::end;
  zactor_destroy(&beacon);
}
