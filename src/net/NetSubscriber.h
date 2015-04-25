#ifndef NET_NETSUBSCRIBER_H
#define NET_NETSUBSCRIBER_H

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include <net/Zmq.h>
#include <util/Log.h>
#include <util/Uuid.h>


class NetMessage;

/**
 * NetSubscriber class.
 *
 * Connects to all other discovered publishers.
 * All broadcasted messages are queued up, and can pe popped()
 * until the queue is empty.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class NetSubscriber
{
 public:
  virtual ~NetSubscriber();

  NetSubscriber(const NetSubscriber& c) = delete;
  NetSubscriber& operator=(const NetSubscriber& c) = delete;

 private:
  friend class NetDiscovery;
  /**
   * Private constructor as this is a helper class for NetDiscovery
   */
  NetSubscriber();

  /**
   * Connects to discovered node with given uuid, and connection string.
   * This is cached locally for later disconnects.
   */
  void connect(const Uuid& uuid,
               const std::string& connection);

  /**
   * Disconnects from a given uuid, with the same connection string as
   * previously provided with connect()
   */
  void disconnect(const Uuid& uuid);

  Log log_;
  std::mutex mutex_;
  zmq::context_t context_;
  std::unique_ptr<zmq::socket_t> socket_;
  std::map<Uuid, std::string> connections_;
};

#endif  // NET_NETSUBSCRIBER_H
