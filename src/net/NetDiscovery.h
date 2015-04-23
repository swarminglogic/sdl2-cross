#ifndef NET_NETDISCOVERY_H
#define NET_NETDISCOVERY_H

#include <atomic>
#include <map>
#include <mutex>
#include <thread>

#include <util/Log.h>
#include <util/Uuid.h>

typedef struct _zmsg_t zmsg_t;

/**
 * NetDiscovery class.
 *
 * Acts as interface to mDNS service for discovering other
 * running processes on the local area network.
 *
 * The application should only ever need one instance of this class.
 * An instance of this class makes it discoverable by other
 * processes with an instance of this class.
 *
 * A connected nodes is called a Peer.
 * ATM a Peer only has
 *
 * This is a PUB/SUB style discovery, where each node is both a SUB and PUB.
 * However, there is no need to worry about these details.
 *
 * It simply provides the following functionality:
 *  - Give me a list of all known peers.
 *  - Send this message to peer X.
 *  - Send this message to all peers.
 *
 * @author SwarmingLogic
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

  void debugState() const;
  NetDiscovery(const NetDiscovery& c) = delete;
  NetDiscovery& operator=(const NetDiscovery& c) = delete;

 private:
  // static void run_broadcast();
  static void run();
  static bool extractUuid(zmsg_t *msg);

  ~NetDiscovery();

  Log log_;
  std::atomic_bool isRunning_;
  std::atomic_bool isSuspended_;
  std::thread thread_;
  std::mutex mutex_;

  std::map<std::string, std::string> participants_;

  Uuid uuid_;
  std::string hostname_;
};

#endif  // NET_NETDISCOVERY_H
