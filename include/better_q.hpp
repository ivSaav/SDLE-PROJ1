#ifndef BETTERQ_H
#define BETTERQ_H

#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <string>
// Serialization
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/memory.hpp>

using namespace std;

typedef list<string>::iterator list_iter;
typedef map<string, list_iter> peer_iterator_map;

class BetterQ {
private:
  peer_iterator_map peer_map;
  map<string, int> load_map;
  list<string> q;
  int start_cnt = 0;
  mutex m;

  // Iterators
  bool is_at_end(const list_iter &i);
  bool is_at_start(const list_iter &i);
  void dec_cnt();
  void trim_queue();
  string get(const list_iter &i);

  friend class cereal::access;
  template <class Archive>
  void save(Archive & ar) const {
    ar(load_map, q, start_cnt);
  }
  
  template <class Archive>
  void load(Archive & ar)
  {
    ar(load_map, q, start_cnt);
    cout << "SIZE DO LOAD MAPA: " << load_map.size() << endl;
  }

public:
  BetterQ() : q(list<string>()) { q.push_back(""); }
  BetterQ(const BetterQ &bq) : q(bq.q), peer_map(bq.peer_map), m() {}

  // List/Queue
  void push_back(string s);
  bool pop_front();

  mutex& getM() { return m; }

  // Peer
  void sub_peer(string peer_id);
  void unsub_peer(string peer_id);
  bool contains_peers();
  bool contains_peer(string peer_id);

  // Iterators
  string next(string peer_id);

  void save_queue();
  void load_queue();

  friend ostream &operator<<(ostream &os, BetterQ &q);
};

#endif /* ifndef BETTERQ_H */
