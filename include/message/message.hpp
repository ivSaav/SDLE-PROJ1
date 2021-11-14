// Will be

#include <zmqpp/zmqpp.hpp>

using namespace std;

// PUT MSG
// SUB MSG
// GET MSG
// UNSUB MSG

class Message {
public:
  Message(string t_name) : topic_name(t_name) {}
  virtual zmqpp::message to_zmq_msg() { return zmqpp::message(topic_name); }

private:
  string topic_name;
};
