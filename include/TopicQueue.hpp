#pragma once
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

/*
   This class is responsible for handling the message queue
   and managing each peer's position in it
*/
class TopicQueue {

private:
  // map<topic_name, map<peer_id, iterator>>
  unordered_map<string, map<string, deque<string>::iterator>> peer_positions;
  // map<topic_name, queue>
  unordered_map<string, deque<string>> queues;

  deque<string> &getQueue(const string topic_name);

  bool getIterator(string topic_name, string peer_id,
                   deque<string>::iterator &iter);

public:
  TopicQueue() {}
  void trimQueue(string topic_name);

  bool is_subscribed(string peer_id, string topic_name) const;
  void subscribe(string peer_id, string topic_name);
  void unsubscribe(string peer_id, string topic_name);
  void put(string topic_name, string content);
  bool get(string peer_id, string topic_name, string &content);

  friend ostream &operator<<(ostream &os, TopicQueue &q);
};
