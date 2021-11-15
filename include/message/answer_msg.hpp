#include "message.hpp"

class AnswerMessage : public Message {
public:
  AnswerMessage() : Message(PUT, ""), body(""){};
  AnswerMessage(string t_name, string body)
      : Message(PUT, t_name), body(body) {}
  AnswerMessage(zmqpp::message &msg) : AnswerMessage() {
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

  string get_body() { return this->body; }

  friend ostream &operator<<(ostream &os, const AnswerMessage &msg);

private:
  string body;
};

inline ostream &operator<<(ostream &os, const AnswerMessage &msg) {
  os << "[-] " /*TODO use seq number */ << msg.topic_name << " ; " << msg.body
     << endl;
  return os;
}
