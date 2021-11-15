#pragma once

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
  Message(){};

  Message(msg_type type, string t_name, string id)
      : type(type), topic_name(t_name), id(id) {}

  Message(zmqpp::message &msg, msg_type type) : Message() {
    this->type = type;
    msg >> this->topic_name >> this->id;
  }

  virtual zmqpp::message to_zmq_msg() {
    return zmqpp::message(type, topic_name, id);
  }

  virtual string to_string() const {
    /*TODO use seq number */

    return "[" + typeStrings[type] + "]\t" + "(" + id + ")\t" + ";" +
           topic_name;
  }

  string get_topic() { return this->topic_name; }
  string get_id() { return this->id; }
  msg_type get_type() { return this->type; }

  friend ostream &operator<<(ostream &os, const Message &msg);
  static string typeStrings[];

protected:
  /* TODO seq number*/
  msg_type type;
  string topic_name;
  string id;
};

inline ostream &operator<<(ostream &os, const Message &msg) {
  os << msg.to_string();
  return os;
}

/* SUBCLASSES */

class GetMessage : public Message {
public:
  GetMessage(string t_name, string id) : Message(GET, t_name, id) {}
  GetMessage(zmqpp::message &msg) : Message(msg, GET) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};

class SubMessage : public Message {
public:
  SubMessage(string t_name, string id) : Message(SUB, t_name, id) {}
  SubMessage(zmqpp::message &msg) : Message(msg, SUB) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};

class UnsubMessage : public Message {
public:
  UnsubMessage(string t_name, string id) : Message(UNSUB, t_name, id) {}
  UnsubMessage(zmqpp::message &msg) : Message(msg, UNSUB) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};
