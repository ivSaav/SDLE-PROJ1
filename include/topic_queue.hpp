#pragma once
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

typedef map<string, list<string>::iterator> peer_iterator_map;

/*
   This class is responsible for handling the message queue
   and managing each peer's position in it
*/
class TopicQueue {
private:
  // map<topic_name, map<peer_id, iterator>>
  unordered_map<string, peer_iterator_map> peer_positions;
  // map<topic_name, queue>
  unordered_map<string, list<string>> queues;

  list<string> &getQueue(const string topic_name);

  bool getIterator(string topic_name, string peer_id,
                   list<string>::iterator &iter);
  bool contains_subscribed(string topic_name) const;

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
