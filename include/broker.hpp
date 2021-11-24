#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"
#include "../include/titanic.hpp"
#include "../include/state.hpp"

#define NUM_WORKERS 3
#define SAVE_RATE 1

class Broker {
public:
  Broker(zmqpp::context &);
  Titanic titanic;
  void run();
  
private:
  TopicQueue topic_queue;
  State state;
  zmqpp::socket frontend;
  zmqpp::socket backend;
  vector<Worker*> workers;
  void cleanUp();

  void ack();
};
