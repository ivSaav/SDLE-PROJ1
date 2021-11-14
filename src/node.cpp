#include "../include/node.hpp"
#include "../include/message/put_msg.hpp"

#include "../include/common.hpp"

int receive_ack(zmqpp::socket &socket) {
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

// zmqpp::socket server(context, zmqpp::socket_type::rep);

Node::~Node() {
  this->s_subscribe.close();
  this->s_publish.close();
}

int Node::send(zmqpp::socket &socket, zmqpp::message &msg) {
#ifdef DEBUG
  std::cout << "Sending Msg" << std::endl;
#endif

  socket.send(msg);
  return receive_ack(socket);
}

int Node::get(string topic_name, string &content) {
  cout << topic_name << endl;
  Message put_msg = PutMessage(topic_name, "CONT");
  zmqpp::message msg = put_msg.to_zmq_msg();
  this->send(this->s_subscribe, msg);
  return 0;
}

int Node::put(std::string topic_name, std::string content) {
  Message put_msg = PutMessage(topic_name, "body goes here");
  zmqpp::message msg = put_msg.to_zmq_msg();
  this->send(this->s_publish, msg);
  return 0;
}