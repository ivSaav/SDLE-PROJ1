#include "../include/node.hpp"
#include "../include/message/answer_msg.hpp"
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

int Node::subscribe(std::string topic_name) {
  SubMessage sub_msg = SubMessage(topic_name, this->id);
  zmqpp::message msg = sub_msg.to_zmq_msg();
  this->s_subscribe.send(msg);
  return receive_ack(this->s_subscribe);
}

int Node::unsubscribe(std::string topic_name) {
  UnsubMessage unsub_msg = UnsubMessage(topic_name, this->id);
  zmqpp::message msg = unsub_msg.to_zmq_msg();
  this->s_subscribe.send(msg);
  return receive_ack(this->s_subscribe);
}

int Node::get(string topic_name, string &content) {
  Message get_msg = GetMessage(topic_name, this->id);
  zmqpp::message msg = get_msg.to_zmq_msg();
  this->s_subscribe.send(msg);

  zmqpp::message response;
  this->s_subscribe.receive(response);
  AnswerMessage answer_msg(response);
  content = answer_msg.get_body();
  cout << "\t" << answer_msg << endl;

  return 0;
}

int Node::put(std::string topic_name, std::string content) {
  PutMessage put_msg = PutMessage(topic_name, content, this->id);
  zmqpp::message msg = put_msg.to_zmq_msg();
  this->s_publish.send(msg);
  return receive_ack(this->s_publish);
}
