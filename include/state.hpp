#ifndef STATE_H
#define STATE_H
#include "../include/topic_queue.hpp"
#include <thread>
#include <zmqpp/zmqpp.hpp>

using namespace std;

#define SAVE_RATE 1

class State {
private:
  TopicQueue &topic_queue;
  thread t;
  int num_requests;
  mutex m;

public:
  State(TopicQueue &q) : topic_queue(q) {}

  bool need_save();
  void save();
  void load();
  void run();
};

#endif /* ifndef STATE_H */
