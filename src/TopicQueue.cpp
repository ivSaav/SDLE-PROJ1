#include "../include/TopicQueue.hpp"
#include <algorithm>
#include <iostream>

/* PRIVATE */
deque<string> &TopicQueue::getQueue(const string topic_name) {
  if (this->queues.find(topic_name) == this->queues.end()) {
    // Need to create new queue
    deque<string> q;
    this->queues.insert({topic_name, q});
  }

  return this->queues.at(topic_name);
}

void TopicQueue::trimQueue(string topic_name) {
  deque<string> &q = this->getQueue(topic_name);
  if (q.empty())
    return;

  if (this->peer_positions.find(topic_name) == this->peer_positions.end())
    return; // No iterator

  map<string, deque<string>::iterator> &id_to_queue =
      this->peer_positions.at(topic_name);

  deque<string>::iterator iter = id_to_queue.begin()->second;

  auto lowest_iter = std::min_element(id_to_queue.begin(), id_to_queue.end(),
                                      [&q](const auto &a, const auto &b) {
                                        return distance(q.begin(), a.second) <
                                               distance(q.begin(), b.second);
                                      });

  if (lowest_iter->second != q.begin())
    q.pop_front();
}

/* PUBLIC */
bool TopicQueue::is_subscribed(string peer_id, string topic_name) const {
  if (this->peer_positions.find(topic_name) == this->peer_positions.end())
    return false;
  auto id_to_queue = this->peer_positions.at(topic_name);

  return id_to_queue.find(peer_id) != id_to_queue.end();
}

void TopicQueue::subscribe(string peer_id, string topic_name) {
  // If no map, create it
  if (this->peer_positions.find(topic_name) == this->peer_positions.end())
    this->peer_positions.insert(
        {topic_name, map<string, deque<string>::iterator>()});

  // Get map that points to queues
  map<string, deque<string>::iterator> &id_to_queue =
      this->peer_positions.at(topic_name);

  // Add reference of peer_id to the map
  deque<string> &queue = this->getQueue(topic_name);
  if (id_to_queue.find(peer_id) == id_to_queue.end()) {
    id_to_queue.insert({peer_id, queue.begin()});
  }

  // Peer was already subscribed
  return;
}

void TopicQueue::unsubscribe(string peer_id, string topic_name) {
  // Get map that points to queues
  if (this->peer_positions.find(topic_name) == this->peer_positions.end())
    return; // Nothing to remove
  else {
    map<string, deque<string>::iterator> &id_to_queue =
        this->peer_positions.at(topic_name);

    // Get reference of peer in map
    if (id_to_queue.find(peer_id) == id_to_queue.end()) {
      return; // Nothing to remove
    }

    // Peer was already subscribed
    id_to_queue.erase(peer_id);
    this->trimQueue(topic_name);
    return;
  }
}

void TopicQueue::put(string topic_name, string content) {
  deque<string> &queue = this->getQueue(topic_name);
  queue.push_back(content);

  return;
}

bool TopicQueue::get(string peer_id, string topic_name, string &content) {
  if (!is_subscribed(peer_id, topic_name))
    return false;

  const deque<string> &queue = this->getQueue(topic_name);
  deque<string>::iterator &iter =
      this->peer_positions.at(topic_name).at(peer_id);
  if (iter == queue.end()) // We have no more messages
    return false;

  content = *iter;
  ++iter;
  this->trimQueue(topic_name);
  return true;
}

ostream &operator<<(ostream &os, TopicQueue &q) {
  string sep = "---------------------------------------";
  os << "Queues:" << endl;
  for (auto i = q.queues.begin(); i != q.queues.end(); ++i) {
    deque<string> curr = (i->second);
    os << i->first + ": ";
    for (auto j = curr.begin(); j != curr.end(); ++j) {
      os << (*j) + " ";
    }
    os << endl;
  }
  os << sep << endl;
  os << "Maps:" << endl;
  for (auto i = q.peer_positions.begin(); i != q.peer_positions.end(); ++i) {
    map<string, deque<string>::iterator> curr = i->second;
    const string topic_name = i->first;
    auto queue = q.getQueue(topic_name);

    os << topic_name + ": ";
    for (auto j = curr.begin(); j != curr.end(); ++j) {
      if (j->second != queue.end())
        os << j->first + "-" + *(j->second) + " ";
      else
        os << j->first + "-END" + " ";
    }
    os << endl;
  }

  return os;
}
