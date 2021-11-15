#pragma once
#include "message.hpp"

class AnswerMessage : public Message {
public:
  AnswerMessage(string t_name, string body, string id)
      : Message(ANSWER, t_name, id), body(body) {}
  AnswerMessage(zmqpp::message &msg) : Message(msg, ANSWER) {
    msg >> this->body;
  }

  virtual zmqpp::message to_zmq_msg() {
    zmqpp::message msg = Message::to_zmq_msg();
    msg << body;
    return msg;
  }

  string get_body() { return this->body; }

  string to_string() const { return Message::to_string() + ";" + this->body; }

private:
  string body;
};
