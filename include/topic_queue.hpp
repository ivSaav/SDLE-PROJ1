#pragma once
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include "better_q.hpp"

using namespace std;

/*
   This class is responsible for handling the message queue
   and managing each peer's position in it
*/
class TopicQueue {
private:
  // map<topic_name, queue>
  mutex m;
  unordered_map<string, BetterQ> queues;

  BetterQ &getQueue(const string topic_name);
  bool contains_subscribed(string topic_name);

  friend class cereal::access;
  template <class Archive>
  void serialize(Archive & ar)
  {
    ar(queues);
  }

public:
  TopicQueue() {}

  bool is_subscribed(string peer_id, string topic_name);
  void subscribe(string peer_id, string topic_name);
  void unsubscribe(string peer_id, string topic_name);
  void put(string topic_name, string content);
  bool get(string peer_id, string topic_name, string &content);

  void load_queues();
  void save_queues();

  friend ostream &operator<<(ostream &os, TopicQueue &q);
};
