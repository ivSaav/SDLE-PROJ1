#ifndef TITANICMSG_H
#define TITANICMSG_H

#include <string>
#include <zmqpp/zmqpp.hpp>

#include "message.hpp"

using namespace std;

class TitanicMessage {
private:
  string id;
  int type;
public:
  const static int PUT_TIT = 0;
  const static int GET_TIT = 1;
  const static int DEL_TIT = 2;
  TitanicMessage(int t, string id) : id(id), type(t) {}
  TitanicMessage(zmqpp::message &msg) {
    msg >> id;
    msg >> type;
  }

  bool isPut() { return type == PUT_TIT; }
  bool isGet() { return type == GET_TIT; }
  bool isDel() { return type == DEL_TIT; }

  string getId() { return id; }
  virtual zmqpp::message to_zmq_msg() {
    zmqpp::message m(id, type);
    return m;
  }

  virtual string to_string() {
    string type = isPut() ? "PUT" : "GET";
    return "ID: " + id + " Type: " + type;
  }
};

class TitanicGetMessage : TitanicMessage{
private:
  Message *message=nullptr;
public:
  TitanicGetMessage(Message m, string id) : TitanicMessage(GET_TIT, id) {
    message = new Message(m);
  }
  TitanicGetMessage(zmqpp::message &msg, string id) : TitanicMessage(GET_TIT, id), message(new Message(msg)) {}
  ~TitanicGetMessage() {
    if (!message)
      free(message);
  }

  Message* getMessage() { return message; }

  virtual zmqpp::message to_zmq_msg() {
    zmqpp::message m = TitanicMessage::to_zmq_msg();
    zmqpp::message req = message->to_zmq_msg();
    string c;
    while(req.remaining()) {
      req >> c;
      m << c;
    }
    return m;
  }

  virtual string to_string() {
    return  TitanicMessage::to_string() + message->to_string() + "\n";
  }
};

#endif /* ifndef TITANICMSG_H */
