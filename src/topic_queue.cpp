#include "../include/topic_queue.hpp"
#include "../include/better_q.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <mutex>

/* PRIVATE */
BetterQ &TopicQueue::getQueue(const string topic_name) {
  if (this->queues.find(topic_name) == this->queues.end()) {
    // Need to create new queue
    BetterQ q;
    this->queues.insert({topic_name, q});
  }

  return this->queues.at(topic_name);
}

bool TopicQueue::contains_subscribed(string topic_name) {
  return queues.find(topic_name) != queues.end() && // Queue exists
         this->getQueue(topic_name).contains_peers();
}

/* PUBLIC */
bool TopicQueue::is_subscribed(string peer_id, string topic_name) {
  lock_guard<mutex> guard(m);
  if (!contains_subscribed(topic_name))
    return false;
  auto &q = getQueue(topic_name);

  return q.contains_peer(peer_id);
}

void TopicQueue::subscribe(string peer_id, string topic_name) {
  lock_guard<mutex> guard(m);
  // Get map that points to queues
  BetterQ &q = getQueue(topic_name);
  q.sub_peer(peer_id);
}

void TopicQueue::unsubscribe(string peer_id, string topic_name) {
  lock_guard<mutex> guard(m);
  BetterQ &q = getQueue(topic_name);
  q.unsub_peer(peer_id);

  if (!this->contains_subscribed(topic_name)) {
      this->queues.erase(topic_name);
      std::cout << "DELETE " << topic_name << " (NO SUBS)" << std::endl;
  }
}

void TopicQueue::put(string topic_name, string content) {
  lock_guard<mutex> guard(m);
  if (!this->contains_subscribed(topic_name))
    return; // No need to append if nobody is subbed

  BetterQ &queue = this->getQueue(topic_name);
  queue.push_back(content);
}

bool TopicQueue::get(string peer_id, string topic_name, string &content) {
  lock_guard<mutex> guard(m);

  BetterQ &queue = this->getQueue(topic_name);
  string c = queue.next(peer_id);
  if (c == "")
    return false;

  content = c;
  return true;
}

ostream &operator<<(ostream &os, TopicQueue &q) {
  lock_guard<mutex> guard(q.m);
  string sep = "---------------------------------------";
  os << "Queues:" << endl;
  for (auto &i : q.queues) {
    string topic = i.first;
    BetterQ bq = i.second;
    cout << topic << endl;
    cout << bq << endl;
  }

  return os;
}
