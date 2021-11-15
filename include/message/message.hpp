// Will be

#include <iostream>
#include <zmqpp/zmqpp.hpp>

using namespace std;

// PUT MSG
// SUB MSG
// GET MSG
// UNSUB MSG

typedef enum { PUT = 0, GET = 1, SUB = 2, UNSUB = 3, ANSWER = 4 } msg_type;

/* MESSAGE CLASS */

class Message {

public:
  Message(string t_name) : topic_name(t_name) {}
  Message(msg_type type, string t_name) : type(type), topic_name(t_name) {}
  Message(zmqpp::message &msg, msg_type type)
      : type(type), topic_name(msg.get(1)) {} // TODO Maybe assert with type?

  virtual zmqpp::message to_zmq_msg() {
    return zmqpp::message(type, topic_name);
  }
  virtual string to_string() const {
    /*TODO use seq number */
    return "[" + typeStrings[type] + "]" + ";" + topic_name;
  }

  string get_topic() { return this->topic_name; }
  msg_type get_type() { return this->type; }

  friend ostream &operator<<(ostream &os, const Message &msg);
  static string typeStrings[];

protected:
  /* TODO seq number*/
  msg_type type;
  string topic_name;
};

inline ostream &operator<<(ostream &os, const Message &msg) {
  os << msg.to_string();
  return os;
}

/* SUBCLASSES */

class GetMessage : public Message {
public:
  GetMessage(string t_name) : Message(GET, t_name) {}
  GetMessage(zmqpp::message &msg) : Message(msg, GET) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};

class SubMessage : public Message {
public:
  SubMessage(string t_name) : Message(GET, t_name) {}
  SubMessage(zmqpp::message &msg) : Message(msg, SUB) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};

class UnsubMessage : public Message {
public:
  UnsubMessage(string t_name) : Message(GET, t_name) {}
  UnsubMessage(zmqpp::message &msg) : Message(msg, UNSUB) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};
