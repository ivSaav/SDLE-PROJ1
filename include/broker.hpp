#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"
#include "../include/state.hpp"

#define NUM_WORKERS 3
#define SAVE_RATE 2

class Broker {
public:
  Broker(zmqpp::context &);
  void run();
  void load_state();

private:
  zmqpp::socket frontend;
  zmqpp::socket backend;
  TopicQueue topic_queue;
  vector<Worker*> workers;
  State state;
  void cleanUp();

  void ack();
};
