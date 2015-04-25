#ifndef NET_NETPUBLISHER_H
#define NET_NETPUBLISHER_H

#include <memory>
#include <string>

#include <net/Zmq.h>
#include <util/Log.h>
#include <util/Uuid.h>


/**
 * NetPublisher class.
 *
 * Used by NetDiscovery represent a PUB node that is automatically
 * connected to by all discovered SUB nodes.
 *
 * // TODO swarminglogic, 2015-04-25: Add functions for transmitting data
 * // to subscribers
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class NetPublisher
{
 public:
  virtual ~NetPublisher();

  NetPublisher(const NetPublisher& c) = delete;
  NetPublisher& operator=(const NetPublisher& c) = delete;

 private:
  friend class NetDiscovery;

  /**
   * Private constructor as this is a helper class for NetDiscovery
   * @param uuid      The unique ID of local instance
   * @param hostname  Where this publisher resides
   */
  NetPublisher(const Uuid& uuid,
               const std::string& hostname);
  int getSocketPort() const;
  void createSocket();

  Log log_;
  Uuid uuid_;
  zmq::context_t context_;
  std::unique_ptr<zmq::socket_t> socket_;
  std::string hostname_;
  int port_;
};

#endif  // NET_NETPUBLISHER_H
