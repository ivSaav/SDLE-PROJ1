#ifndef WORKER_H
#define WORKER_H
#include "../include/topic_queue.hpp"
#include <thread>
#include <zmqpp/zmqpp.hpp>

using namespace std;

class Worker {
private:
  thread t;

public:
  Worker() {}

  void run();
  void run(TopicQueue &t, zmqpp::socket &s);
  void join();
};

#endif /* ifndef WORKER_H */
