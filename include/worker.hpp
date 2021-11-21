#ifndef WORKER_H
#define WORKER_H
#include "../include/topic_queue.hpp"
#include <thread>
#include <zmqpp/zmqpp.hpp>

using namespace std;

class Worker {
private:
  thread t;
  string id;
  TopicQueue &topic_queue;

  void handler(zmqpp::message &req, zmqpp::message &rep);
  void handle_put(zmqpp::message &req, zmqpp::message &rep);
  void handle_get(zmqpp::message &req, zmqpp::message &rep);
  void handle_sub(zmqpp::message &req, zmqpp::message &rep);
  void handle_unsub(zmqpp::message &req, zmqpp::message &rep);
public:
  Worker(TopicQueue &q, string id) : topic_queue(q), id(id) {}
  Worker(const Worker &w) : topic_queue(w.topic_queue), id(w.id) {}

  void work();
  void run();
  void join();

  string getId() const { return id; }
};

#endif /* ifndef WORKER_H */
