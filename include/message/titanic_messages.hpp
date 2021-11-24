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
  Message *message = nullptr;

public:
  const static int REQ_TIT = 0;
  const static int GET_TIT = 1;
  const static int DEL_TIT = 2;
  TitanicMessage(int t, Message m, string id = "-1")
      : id(id), type(t), message(new Message(m)) {}
  TitanicMessage(zmqpp::message &msg) {
    msg >> id;
    msg >> type;
    message = new Message(msg);
  }
  ~TitanicMessage() {
    if (!message)
      free(message);
  }

  bool isReq() { return type == REQ_TIT; }
  bool isGet() { return type == GET_TIT; }
  bool isDel() { return type == DEL_TIT; }
  string getId() { return id; }
  Message *getMessage() { return message; }

  zmqpp::message to_zmq_msg() {
    zmqpp::message m(id, type);
    if (message) {
      zmqpp::message req = message->to_zmq_msg();
      string c;
      while (req.remaining()) {
        req >> c;
        m << c;
      }
    }
    return m;
  }

  string to_string() {
    string t;
    if (type == REQ_TIT)
      t = "REQ";
    else if (type == DEL_TIT)
      t = "DEL";
    else if (type == GET_TIT)
      t = "GET";
    string res = "ID: " + id + " Type: " + t;
    if (message)
      res += message->to_string();
    return res;
  }
};

#endif /* ifndef TITANICMSG_H */
