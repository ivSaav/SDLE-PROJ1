#include "../include/better_q.hpp"

/* PRIVATE */
bool BetterQ::is_at_end(const list_iter &i) const {
  list<string>::iterator j = i;
  ++j;
  return (j == q.end());
}

bool BetterQ::is_at_start(const list_iter &i) const { return q.begin() == i; }

string BetterQ::get(const list_iter &i) const {
  if (is_at_end(i))
    return "";
  return *i;
}

/* PUBLIC */

void BetterQ::sub_peer(string peer_id) {
  if (contains_peer(peer_id))
    return;

  list_iter it = --q.end();
  peer_map.insert({peer_id, it});
  
  if (is_at_start(it)) {
    ++start_cnt;
  }
}

void BetterQ::unsub_peer(string peer_id) {
  if (!contains_peer(peer_id))
    return;

  list_iter it = peer_map.at(peer_id);
  if (this->is_at_start(it))
    --start_cnt;

  peer_map.erase(peer_id);
}

bool BetterQ::contains_peers() const { return this->peer_map.size() > 0; }

bool BetterQ::contains_peer(string peer_id) const {
  return peer_map.find(peer_id) != peer_map.end();
}

string BetterQ::next(string peer_id) {
  list_iter &i = this->peer_map.at(peer_id);
  bool was_start = this->is_at_start(i);
  list_iter j = i;
  ++j;

  if (j == q.end())
    return "";

  string c = *(++i);
  if (was_start)
    this->dec_cnt();

  return c;
}

void BetterQ::push_back(string s) { q.push_back(s); }
bool BetterQ::pop_front() {
  if (q.size() == 1)
    return false;
  q.pop_front();
  return true;
}

void BetterQ::dec_cnt() {
  --start_cnt;
  if (start_cnt == 0)
    trim_queue();
}

void BetterQ::trim_queue() {
  if (q.size() > 1 && start_cnt == 0) {
    cout << "TRIMMED";
    cout << *this << endl;
    q.pop_front();
    *q.begin() = "";
    for (auto const &i : this->peer_map) {
      if (is_at_start(i.second))
        ++start_cnt;
    }
  }
}

ostream &operator<<(ostream &os, BetterQ &q) {
  cout << "\tStart Cnt=" << q.start_cnt << "Content:" << endl;
  fflush(stdout);
  for (auto peer_pos : q.q) {
    cout << "\t" << peer_pos << " ";
  }

  cout << endl << "\tSubscriptions:" << endl;
  for (auto id_iter : q.peer_map) {
    string peer_id = id_iter.first;
    list_iter iter = id_iter.second;
    cout << "\t" << peer_id << ": " << q.get(iter) << endl;
  }

  return os;
}
