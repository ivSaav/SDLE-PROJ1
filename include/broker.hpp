#pragma once
#include <zmqpp/zmqpp.hpp>

#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"

#define NUM_WORKERS 3
#define STATE_FILE "backup.cereal"

class Broker {
public:
  Broker(zmqpp::context &);
  void run();
  void save_state();
  void load_state();

  void test() {
    cout << this->topic_queue.is_subscribed("1", "uwu") << endl;
    this->topic_queue.subscribe("1", "uwu");
  }

private:
  zmqpp::socket frontend;
  zmqpp::socket backend;
  TopicQueue topic_queue;
  vector<Worker*> workers;
  void cleanUp();

  void ack();
};
