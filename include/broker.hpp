#include <zmqpp/zmqpp.hpp>

class Broker {
public:
  Broker(zmqpp::context &);
  void run();

private:
  zmqpp::socket s_publish;
  zmqpp::socket s_subscribe;

  void ack();
};
