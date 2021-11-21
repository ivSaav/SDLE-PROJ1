#ifndef BETTERQ_H
#define BETTERQ_H

#include <iostream>
#include <list>
#include <map>
#include <string>

using namespace std;

typedef list<string>::iterator list_iter;
typedef map<string, list_iter> peer_iterator_map;

class BetterQ {
private:
  peer_iterator_map peer_map;
  list<string> q;
  int start_cnt = 0;

  // Iterators
  bool is_at_end(const list_iter &i) const;
  bool is_at_start(const list_iter &i) const;
  string get(const list_iter &i) const;

public:
  BetterQ() : q(list<string>()) { q.push_back(""); }

  // List/Queue
  void push_back(string s);
  bool pop_front();
  void dec_cnt();
  void trim_queue();

  // Peer
  void sub_peer(string peer_id);
  void unsub_peer(string peer_id);
  bool contains_peers() const;
  bool contains_peer(string peer_id) const;

  // Iterators
  string next(string peer_id);

  friend ostream &operator<<(ostream &os, BetterQ &q);
};

#endif /* ifndef BETTERQ_H */
