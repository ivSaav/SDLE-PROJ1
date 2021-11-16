#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"

class Broker {
public:
  Broker(zmqpp::context &);
  void run();

private:
  zmqpp::socket s_publish;
  zmqpp::socket s_subscribe;
  TopicQueue topic_queue;

  void ack();
};
