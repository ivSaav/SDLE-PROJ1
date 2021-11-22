#ifndef STATE_H
#define STATE_H
#include "../include/topic_queue.hpp"
#include <thread>
#include <zmqpp/zmqpp.hpp>

using namespace std;

class State {
private:
  TopicQueue &topic_queue;
  thread t;

public:
  State(TopicQueue &q) : topic_queue(q) {}

  void save();
  void run();
  void join();
};

#endif /* ifndef STATE_H */
