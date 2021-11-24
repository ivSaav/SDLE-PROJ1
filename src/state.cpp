#include <fstream>
#include <mutex>
// Serialization
#include <cereal/archives/binary.hpp>

#include "../include/common.hpp"
#include "../include/state.hpp"

using namespace std;

void State::save() {
  ofstream os(STATE_FILE, std::ios::binary);
  cereal::BinaryOutputArchive archive(os);
  
  topic_queue.save_queues();
  archive(topic_queue);
  cout << "Saved state" << endl;
}

void State::load() {
  ifstream is(STATE_FILE, std::ios::binary);
  if(is.is_open()) {
    cereal::BinaryInputArchive archive(is);
    
    archive(topic_queue);
    topic_queue.load_queues();
    cout << "Loaded state" << endl;
  }
}

void State::run() {
  this->t = thread(&State::save, this);
  t.detach();
}
