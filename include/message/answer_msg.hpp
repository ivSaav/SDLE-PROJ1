#pragma once
#include "message.hpp"

class AnswerMessage : public Message {
public:
  AnswerMessage() {}
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

  virtual void append_to_zmq_msg(zmqpp::message &msg) {
    Message::append_to_zmq_msg(msg);
    msg << body;
  }

  template<class Archive>
  void serialize(Archive & ar)
  { ar(cereal::base_class<Message>(this), body); }

  string get_body() { return this->body; }

  string to_string() const { return Message::to_string() + ";" + this->body; }

private:
  string body;
};
