#ifndef TITANICMSG_H
#define TITANICMSG_H

#include <string>
#include <zmqpp/zmqpp.hpp>

#include "message.hpp"
#include "put_msg.hpp"
#include "answer_msg.hpp"

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
  TitanicMessage(int t, Message *m, string id = "-1")
      : id(id), type(t), message(m) {}
  TitanicMessage(int t, string id)
      : id(id), type(t) {}
  TitanicMessage(zmqpp::message &msg) {
    msg >> id;
    msg >> type;
    if (msg.remaining()) { // message may be empty if GET or DEL
      int msg_t;
      msg >> msg_t;
      if (msg_t== msg_type::ANSWER)
        message = new AnswerMessage(msg);
      else if (msg_t== msg_type::PUT) {
        message = new PutMessage(msg);
      }
      else  {
        message = new Message(msg, (msg_type) msg_t);
      }
    }
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
      message->append_to_zmq_msg(m);
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
