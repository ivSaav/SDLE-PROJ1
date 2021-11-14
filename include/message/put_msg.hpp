#include "message.hpp"

class PutMessage : public Message {
public:
  PutMessage(string t_name) : Message(t_name) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};
