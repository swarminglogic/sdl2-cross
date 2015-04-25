#ifndef NET_NETDISCOVERY_H
#define NET_NETDISCOVERY_H

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#include <net/Zmq.h>
#include <util/Log.h>
#include <util/Timer.h>
#include <util/Uuid.h>


typedef struct _zmsg_t zmsg_t;
class NetPublisher;
class NetSubscriber;

/**
 * NetDiscovery class.
 *
 * Provides service for discovering other
 * running processes on the local area network.
 *
 * The application should only ever need one instance of this class, which
 * is enforced with a singleton construct.
 *
 * After init(), it becomes discoverable by other processes with an instance of
 * this class.
 *
 * getPublisher() and getSubscriber() return nodes that follow the PUB/SUB,
 * style, where the subscriber is connected to all known publishers.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class NetDiscovery
{
 public:
  NetDiscovery();

  void init();
  void suspend();
  void resume();
  void stop();

  static NetDiscovery& instance();

  std::shared_ptr<NetPublisher> getPublisher();
  std::shared_ptr<NetSubscriber> getSubscriber();

  NetDiscovery(const NetDiscovery& c) = delete;
  NetDiscovery& operator=(const NetDiscovery& c) = delete;

 private:
  /**
   * Entry function for background thread
   */
  static void run();

  /**
   * Processes incomming broadcasted message.
   * If this message has already been processed,
   * already aware of this client (e.g. myself)
   * it returns false, otherwise true.
   *
   * A nullptr message is considered to be processed by default.
   */
  bool processMessage(zmsg_t *msg);
  void validateHeartBeats();

  ~NetDiscovery();

  Log log_;

  // zmq stuff
  std::shared_ptr<NetPublisher> publisher_;
  std::shared_ptr<NetSubscriber> subscriber_;

  // czmq stuff
  net::czmq::zactor_ptr discoveryPub_;
  net::czmq::zactor_ptr discoverySub_;

  // Threading
  std::atomic_bool isRunning_;
  std::atomic_bool isSuspended_;
  std::thread thread_;
  std::mutex mutex_;

  // Discovery, <id, time since last heartbeat>
  std::map<Uuid, Timer> knownBroadcasters_;

  // magic needs to be the first data element, as filtering on receibed packages
  // done on this value, assuming it is prefixed
  struct BroadcastData {
    int32_t magic;
    Uuid uuid;
    int32_t port;
  } broadcastData_;

  std::string hostname_;
};

#endif  // NET_NETDISCOVERY_H
