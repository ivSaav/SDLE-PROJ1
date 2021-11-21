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
  void save(Archive & ar) const
  {
    map<string, int> srlz_map;
    peer_iterator_map::const_iterator it = peer_map.begin();
    list<string>::const_iterator list_it = q.begin();

    while(it != peer_map.end()) {
      list_it = it->second;

      int d = 0;
      list_it = q.begin();
      while(list_it != q.end()) {
        if (list_it == it->second)
          break;
        ++list_it;
        ++d;
      }
      
      srlz_map.insert({it->first, d});
      it++;
    }
    cout << "DONE" << endl;

    ar(srlz_map, q, start_cnt);
  }
  
  template <class Archive>
  void load(Archive & ar)
  {
    map<string, int> srlz_map;
    ar(srlz_map, q, start_cnt);

    map<string, int>::const_iterator it = srlz_map.begin();
    list<string>::iterator list_it;

    while(it != srlz_map.end()) {
      list_it = q.begin();
      advance(list_it, it->second);
      peer_map.insert({it->first, list_it});
      it++;
    } 
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

  friend ostream &operator<<(ostream &os, BetterQ &q);
};

#endif /* ifndef BETTERQ_H */
