#include "message.hpp"

class GetMessage : public Message {
public:
  GetMessage(string t_name, string body) : Message(GET, t_name) { }
  virtual zmqpp::message to_zmq_msg() { 
    return Message::to_zmq_msg();
  }

private:
};


