#ifndef NET_NETDISCOVERY_H
#define NET_NETDISCOVERY_H

#include <util/Log.h>

/**
 * NetDiscovery class.
 * Acts as interface to mDNS service for discovering other
 * running processes on network.
 *
 * @author SwarmingLogic
 */
class NetDiscovery
{
 public:
  NetDiscovery();
  virtual ~NetDiscovery();

  NetDiscovery(const NetDiscovery& c) = delete;
  NetDiscovery& operator=(const NetDiscovery& c) = delete;

 private:
  Log log_;
};

#endif  // NET_NETDISCOVERY_H


