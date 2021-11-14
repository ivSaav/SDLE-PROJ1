#include "../include/node.hpp"
#include "../include/message/put_msg.hpp"

// zmqpp::socket server(context, zmqpp::socket_type::rep);
Node::Node(const zmqpp::context &context)
    : s_publish(context, zmqpp::socket_type::req),
      s_subscribe(context, zmqpp::socket_type::req) {

  this->s_subscribe.connect("tcp://127.0.0.1:" + to_string(SUB_PORT));
  this->s_publish.connect("tcp://127.0.0.1:" + to_string(PUB_PORT));
}

Node::~Node() {
  this->s_subscribe.close();
  this->s_publish.close();
}

int Node::send(zmqpp::socket &socket, zmqpp::message &msg) {
#ifdef DEBUG
  std::cout << "Sending Msg" << std::endl;
#endif

  socket.send(msg);

  zmqpp::message response;

#ifdef DEBUG
  std::cout << "Awaiting Response" << std::endl;
#endif
  socket.receive(response);
  if (response.is_signal()) {
    zmqpp::signal resp;
    response >> resp;
    if (resp == zmqpp::signal::ok) // TODO Throw exception?
      return 0;
  }
  return 1;
}

int Node::get(std::string topic_name, std::string &content) {
  cout << topic_name << endl;
  return 0;
}

int Node::put(std::string topic_name, std::string content) {
  Message put_msg = PutMessage(topic_name);
  zmqpp::message msg = put_msg.to_zmq_msg();
  this->send(this->s_publish, msg);
  return 0;
}
