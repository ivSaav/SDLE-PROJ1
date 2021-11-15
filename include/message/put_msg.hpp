#include "message.hpp"

class PutMessage : public Message {
public:
  PutMessage() : Message(PUT, ""), body(""){};
  PutMessage(string t_name, string body) : Message(PUT, t_name), body(body) {}
  PutMessage(zmqpp::message &msg) : PutMessage() {
    string t_name, body;
    // msg >> t_name >> body;
    msg >> t_name >> body;
    this->topic_name = t_name;
    this->body = body;
  }

  virtual zmqpp::message to_zmq_msg() {
    zmqpp::message msg = Message::to_zmq_msg();
    msg << body;
    return msg;
  }

  virtual string to_string() const {
    return Message::to_string() + " ; " + body;
  }

  string get_body() { return this->body; }

private:
  string body;
};
