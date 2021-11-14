// Will be

#include <zmqpp/zmqpp.hpp>
#include <iostream>

using namespace std;

// PUT MSG
// SUB MSG
// GET MSG
// UNSUB MSG

typedef enum {PUT, GET, SUB, UNSUB, ANSWER} msg_type;

class Message {

public:
  Message(string t_name) : topic_name(t_name) { }
  Message(msg_type type, string t_name) : type(type), topic_name(t_name) { }
  virtual zmqpp::message to_zmq_msg() { return zmqpp::message(type, topic_name); }

  friend ostream& operator<<(ostream& os, const Message& msg);

  string get_topic() { return this->topic_name; }
  msg_type get_type() { return this->type; }

protected:
  /* TODO seq number*/
  msg_type type;
  string topic_name;


};

inline ostream& operator<<(ostream& os, const Message& msg)
{
    os << /*TODO use seq number */ msg.topic_name << ";" << endl;
    return os;
}



