#include <net/NetDiscovery.h>

#include <umundo/config.h>
#include <umundo/core.h>


using namespace umundo;

char* domain = NULL;

class TestReceiver : public Receiver {
 public:
  TestReceiver(): log_("TestReceiver") {};
  void receive(Message*) {
    log_.d() << "i" << Log::end;
  }
 private:
  Log log_;
};

NetDiscovery::NetDiscovery()
: log_("NetDiscovery")
{
  TestReceiver* testRecv = new TestReceiver();
  Publisher pubFoo("pingpong");
  Subscriber subFoo("pingpong");
  subFoo.setReceiver(testRecv);
  DiscoveryConfigMDNS mdnsOpts;
  if (domain)
    mdnsOpts.setDomain(domain);
  Discovery disc(&mdnsOpts);
  Node node;
  disc.add(node);
  node.addPublisher(pubFoo);
  node.addSubscriber(subFoo);

  int nSecs = 10;
  while (--nSecs > 0) {
    Thread::sleepMs(1000);
    Message* msg = new Message();
    msg->setData("ping", 4);
    log_.d() << "o" << Log::end;
    pubFoo.send(msg);
    delete(msg);
  }
}


NetDiscovery::~NetDiscovery()
{
}
