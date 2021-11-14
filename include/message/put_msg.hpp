#include "message.hpp"

class PutMessage : public Message {
public:
  PutMessage(string t_name, string body) : Message(PUT, t_name) { this->body = body; }
  virtual zmqpp::message to_zmq_msg() { 
    zmqpp::message msg = Message::to_zmq_msg();
    msg << body;
    return msg;
  }

  string get_body() { return this->body; }

  friend ostream& operator<<(ostream& os, const PutMessage& msg);

  
private:
  string body;
};

inline ostream& operator<<(ostream& os, const PutMessage& msg) {
  os << "[-] " /*TODO use seq number */ << msg.topic_name << " ; " << msg.body << endl;
  return os;
}
