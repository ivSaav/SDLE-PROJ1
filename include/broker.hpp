#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"

#define NUM_WORKERS 3

class Broker {
public:
  Broker(zmqpp::context &);
  void run();

private:
  zmqpp::socket frontend;
  zmqpp::socket backend;
  TopicQueue topic_queue;
  vector<Worker*> workers;
  void cleanUp();

  void ack();
};
