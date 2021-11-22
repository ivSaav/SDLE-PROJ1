#include <fstream>
#include <mutex>
// Serialization
#include <cereal/archives/json.hpp>

#include "../include/common.hpp"
#include "../include/state.hpp"

using namespace std;

void State::save() {
  ofstream os(STATE_FILE);
  cereal::JSONOutputArchive archive(os);
  
  archive(topic_queue);
  cout << "Saved state" << endl;
}

void State::run() {
  this->t = thread(&State::save, this);
}

void State::join() { t.join(); }
