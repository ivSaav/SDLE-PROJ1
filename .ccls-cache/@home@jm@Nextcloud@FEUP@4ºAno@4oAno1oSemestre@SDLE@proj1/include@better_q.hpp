#ifndef BETTERQ_H
#define BETTERQ_H

#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <string>

using namespace std;

typedef list<string>::iterator list_iter;
typedef map<string, list_iter> peer_iterator_map;

class BetterQ {
private:
  peer_iterator_map peer_map;
  list<string> q;
  int start_cnt = 0;
  mutex m;

  // Iterators
  bool is_at_end(const list_iter &i);
  bool is_at_start(const list_iter &i);
  void dec_cnt();
  void trim_queue();
  string get(const list_iter &i);

public:
  BetterQ() : q(list<string>()) { q.push_back(""); }
  BetterQ(const BetterQ &bq) : q(bq.q), peer_map(bq.peer_map), m() {}

  // List/Queue
  void push_back(string s);
  bool pop_front();

  mutex &getM() { return m; }

  // Peer
  void sub_peer(string peer_id);
  void unsub_peer(string peer_id);
  bool contains_peers();
  bool contains_peer(string peer_id);

  // Iterators
  string next(string peer_id);

  friend ostream &operator<<(ostream &os, BetterQ &q);
};

#endif /* ifndef BETTERQ_H */
