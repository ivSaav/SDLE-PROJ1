#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"

class Broker {
public:
  Broker(zmqpp::context &);
  void run();

private:
  zmqpp::socket frontend;
  zmqpp::socket backend;
  TopicQueue topic_queue;

  void ack();
};
