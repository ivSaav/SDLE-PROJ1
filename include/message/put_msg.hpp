#pragma once
#include "message.hpp"

class PutMessage : public Message {
public:
  PutMessage(string t_name, string body, string id)
      : Message(PUT, t_name, id), body(body) {}
  PutMessage(zmqpp::message &msg) : Message(msg, PUT) { msg >> this->body; }

  virtual zmqpp::message to_zmq_msg() {
    zmqpp::message msg = Message::to_zmq_msg();
    msg << body;
    return msg;
  }

  virtual string to_string() const { return Message::to_string() + ";" + body; }

  string get_body() { return this->body; }

private:
  string body;
};
