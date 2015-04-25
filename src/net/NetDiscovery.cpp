#include <net/NetDiscovery.h>

#include <cstring>
#include <sstream>
#include <stdexcept>

#include <net/NetPublisher.h>
#include <net/NetSubscriber.h>
#include <util/Exception.h>


NetDiscovery::NetDiscovery()
    : log_("NetDiscovery")
{
  broadcastData_.magic = 0xDEADFACE;  // :)
}



NetDiscovery::~NetDiscovery()
{
  if (thread_.joinable())
    stop();
}


void NetDiscovery::init()
{
  log_.i() << "Initializing service discovery" << Log::end;
  log_.d() << "Uuid: " << broadcastData_.uuid << Log::end;

  const int port = 4242;
  discoveryPub_.reset(zactor_new(zbeacon, nullptr));
  discoverySub_.reset(zactor_new(zbeacon, nullptr));
  {
    std::lock_guard<std::mutex> guard(mutex_);
    zsock_send(discoveryPub_.get(), "si", "CONFIGURE", port);
    char *hostname = zstr_recv(discoveryPub_.get());
    if (hostname) {
      hostname_ = std::string(hostname);
    }
    free(hostname);
    log_.d() << "Hostname: " << hostname_ << Log::end;

    subscriber_.reset(new NetSubscriber);
    publisher_.reset(new NetPublisher(broadcastData_.uuid,
                                      hostname_));
    broadcastData_.port = publisher_->getSocketPort();

    zsock_send(discoveryPub_.get(),
               "sbi",
               "PUBLISH",
               reinterpret_cast<const char*>(&broadcastData_),
               sizeof(broadcastData_), 1000);

    zsock_send(discoverySub_.get(), "si", "CONFIGURE", port);
    zsock_send(discoverySub_.get(), "sb", "SUBSCRIBE",
               reinterpret_cast<const char*>(&broadcastData_.magic),
               sizeof(int32_t));
    zsock_set_rcvtimeo(discoverySub_.get(), 100);
  }

  if (!thread_.joinable()) {
    log_.d() << "Starting thread" << Log::end;
    isRunning_ = true;
    thread_ = std::thread(run);
  } else {
    log_.e() << "Failed to start background thread. Already initialized?"
             << Log::end;
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

  log_.d() << "Clearing up zmq objects" << Log::end;

  // Premeptive reset because the zmq context needs to be destroyed
  // after whatever it is the context for.
  publisher_    = nullptr;
  subscriber_   = nullptr;
  discoveryPub_ = nullptr;
  discoverySub_ = nullptr;
}


void NetDiscovery::suspend()
{
  // TODO swarminglogic, 2015-04-25: Determine how exactly this should work.
  // currently unused, and untested.
  isSuspended_ = true;
  std::lock_guard<std::mutex> guard(mutex_);
  for (auto& broadcaster : knownBroadcasters_)
    broadcaster.second.pause();
}


void NetDiscovery::resume()
{
  isSuspended_ = false;
  std::lock_guard<std::mutex> guard(mutex_);
  for (auto& broadcaster : knownBroadcasters_)
    broadcaster.second.resume();
}


NetDiscovery& NetDiscovery::instance()
{
  static NetDiscovery instance_;
  return instance_;
}


bool NetDiscovery::processMessage(zmsg_t *msg)
{
  if (!msg)
    return true;

  if (zmsg_size(msg) != 2) {
    return true;
  }

  net::czmq::zframe_ptr frame1(zmsg_pop(msg));
  net::czmq::zframe_ptr frame2(zmsg_pop(msg));
  assert(zframe_size(frame2.get()) == sizeof(BroadcastData));
  const BroadcastData* bcData =
      reinterpret_cast<const BroadcastData*>(zframe_data(frame2.get()));


  bool isMe = (bcData->uuid == broadcastData_.uuid);
  if (isMe) {
    return false;
  }

  std::lock_guard<std::mutex> guard(mutex_);
  auto knownBroadcaster = knownBroadcasters_.find(bcData->uuid);

  if (knownBroadcaster == knownBroadcasters_.cend()) {
    log_.i() << "Discovered: " << bcData->uuid << " : "
                        << bcData->port << Log::end;
    // Get the hostname and port to connect
    std::string f1data((const char*)zframe_data(frame1.get()),
                       zframe_size(frame1.get()));

    // connecting to tcp://f1data:port
    auto broadcaster = std::make_pair(bcData->uuid, Timer{});
    broadcaster.second.start();
    knownBroadcasters_.insert(std::move(broadcaster));
    std::string connectString =
        std::string("tcp://") + f1data + ":" + std::to_string(bcData->port);
    subscriber_->connect(bcData->uuid,
                         connectString.c_str());
    return false;
  } else {
    knownBroadcaster->second.reset();
    knownBroadcaster->second.start();
    return true;
  }
}



void NetDiscovery::run()
{
  NetDiscovery& instance_ = instance();
  while (instance_.isRunning_) {
    if (!instance_.isSuspended_) {
      net::zmq::zmsg_ptr msg(zmsg_recv(instance_.discoverySub_.get()));
      if (instance_.processMessage(msg.get()))
        continue;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    instance_.validateHeartBeats();
  }

  zstr_sendx(instance_.discoveryPub_.get(), "SILENCE", NULL);
}


void NetDiscovery::validateHeartBeats()
{
  const unsigned int MAX_MS_TIMEOUT_ALLOWED = 2000u;
  std::lock_guard<std::mutex> guard(mutex_);

  // Cannot use ordinary for(auto x : y) loop due to
  // iterator invalidation after erase().
  for (auto i = knownBroadcasters_.begin() ;
       i != knownBroadcasters_.end();) {
    if (i->second.getTicks() > MAX_MS_TIMEOUT_ALLOWED) {
      const Uuid uuid = i->first;
      log_.d() << uuid << ": node no longer beats" << Log::end;
      subscriber_->disconnect(uuid);
      i = knownBroadcasters_.erase(i);
    } else {
      ++i;
    }
  }
}
