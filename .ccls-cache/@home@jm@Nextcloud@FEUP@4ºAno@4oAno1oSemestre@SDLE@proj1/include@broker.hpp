#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"
#include "../include/titanic.hpp"

#define NUM_WORKERS 3

class Broker {
public:
  Broker(zmqpp::context &);
  Titanic titanic;
  void run();

private:
  zmqpp::socket frontend;
  zmqpp::socket backend;
  TopicQueue topic_queue;
  vector<Worker*> workers;
  void cleanUp();

  void ack();
};
