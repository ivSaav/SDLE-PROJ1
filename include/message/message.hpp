#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

using namespace std;

// PUT MSG
// SUB MSG
// GET MSG
// UNSUB MSG

typedef enum { PUT = 0, GET = 1, SUB = 2, UNSUB = 3, ANSWER = 4, OK = 5, KO = 6 } msg_type;


/* MESSAGE CLASS */

class Message {

public:
  Message(){};
  Message(const Message &m) : type(m.type), topic_name(m.topic_name), id(m.id) {};

  Message(msg_type type, string t_name, string id, int seq_num=0)
      : type(type), topic_name(t_name), id(id), seq_num(seq_num) {}

  Message(zmqpp::message &msg) : Message() {
    int t;
    msg >> t;
    this->type = (msg_type) t;
    msg >> this->topic_name;
    msg >> this->id;
    msg >>seq_num;
  }

  Message(zmqpp::message &msg, msg_type type) : Message() {
    this->type = type;
    msg >> this->topic_name;
    msg >> this->id;
    msg >> seq_num;
  }

  template<class Archive>
  void serialize( Archive & ar )
  { ar(type, topic_name, id, seq_num); }

  virtual zmqpp::message to_zmq_msg() {
    return zmqpp::message(type, topic_name, id, seq_num);
  }

  virtual void append_to_zmq_msg(zmqpp::message &msg) {
    msg << type << topic_name << id << seq_num;
  }

  virtual string to_string() const {
    return "[" + typeStrings[type] + "] " + " id(" + id + ")\t" + " topic(" +
           topic_name + ")" + "seq[" + std::to_string(seq_num) + "]";
  }

  string get_topic() { return this->topic_name; }
  string get_id() { return this->id; }
  msg_type get_type() { return this->type; }

  friend ostream &operator<<(ostream &os, const Message &msg);
  static string typeStrings[];

protected:
  msg_type type;
  string topic_name;
  string id;
  int seq_num;
};

inline ostream &operator<<(ostream &os, const Message &msg) {
  os << msg.to_string();
  return os;
}

/* SUBCLASSES */

class GetMessage : public Message {
public:
  GetMessage() {}
  GetMessage(const GetMessage &m) : Message(m) {}
  GetMessage(string t_name, string id, int seq_num) : Message(GET, t_name, id, seq_num) {}
  GetMessage(zmqpp::message &msg) : Message(msg, GET) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar(cereal::base_class<Message>(this));
  }

private:
};

class OkMessage : public Message {
public:
  OkMessage() {}
  OkMessage(string id) : Message(OK, "-", id) {}
  virtual zmqpp::message to_zmq_msg() { return  Message::to_zmq_msg(); }
private:
};

class KoMessage : public Message {
public:
  KoMessage() {}
  KoMessage(string id) : Message(KO, "-", id) {}
  virtual zmqpp::message to_zmq_msg() { return  Message::to_zmq_msg(); }
private:
};

class SubMessage : public Message {
public:
  SubMessage() {}
  SubMessage(string t_name, string id, int seq_num) : Message(SUB, t_name, id, seq_num) {}
  SubMessage(zmqpp::message &msg) : Message(msg, SUB) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};

class UnsubMessage : public Message {
public:
  UnsubMessage() {}
  UnsubMessage(string t_name, string id, int seq_num) : Message(UNSUB, t_name, id, seq_num) {}
  UnsubMessage(zmqpp::message &msg) : Message(msg, UNSUB) {}
  virtual zmqpp::message to_zmq_msg() { return Message::to_zmq_msg(); }

private:
};

