#ifndef COMMON_H
#define COMMON_H

#include <zmqpp/zmqpp.hpp>
#include "../include/message/message.hpp"

#define CLIENT_PORT 9000
#define WORKER_PORT 9001
#define STATE_FILE "backup.json"

inline void answer(zmqpp::socket &sock, Message &msg) {
  zmqpp::message m = msg.to_zmq_msg();
  sock.send(m);
}

inline void answer(zmqpp::socket &sock, zmqpp::signal sig) {
  zmqpp::message ok_msg;
  ok_msg << sig;
  sock.send(ok_msg);
}

inline void answer_ack(zmqpp::socket &sock) { answer(sock, zmqpp::signal::ok); }

inline void answer_nack(zmqpp::socket &sock) { answer(sock, zmqpp::signal::ko); }

inline int receive_ack(zmqpp::socket &socket) {
#ifdef DEBUG
  std::cout << "Awaiting Response" << std::endl;
#endif
  zmqpp::message response;
  socket.receive(response);
  if (response.is_signal()) {
    zmqpp::signal resp;
    response >> resp;
    if (resp == zmqpp::signal::ok) // TODO Throw exception?
      return 0;
  }
  return 1;
}

#endif /* ifndef COMMON_H */
