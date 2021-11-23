#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "../include/common.hpp"
#include "../include/exceptions.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/node.hpp"

Node::Node(zmqpp::context &context, string id) :
  socket(context, zmqpp::socket_type::req), id(id) {

    this->socket.connect("tcp://127.0.0.1:" + to_string(CLIENT_PORT));
    this->socket.set(zmqpp::socket_option::identity, this->id);
}

Node::~Node() { this->socket.close(); }

int Node::subscribe(std::string topic_name) {
  SubMessage sub_msg = SubMessage(topic_name, this->id);
  zmqpp::message msg = sub_msg.to_zmq_msg();
  this->socket.send(msg);

  try {
    if (receive_ack(this->socket))
      throw AlreadySubscribed(topic_name);

  } catch(const AlreadySubscribed& e) {
    cout << "ERROR: Already subscribed to this topic.\n";
  }

  return 0;
}

int Node::unsubscribe(std::string topic_name) {
  UnsubMessage unsub_msg = UnsubMessage(topic_name, this->id);
  zmqpp::message msg = unsub_msg.to_zmq_msg();
  this->socket.send(msg);
  try {
    if (receive_ack(this->socket))
      throw NotSubscribed(topic_name);
  } catch(const NotSubscribed& e) {
    cout << "ERROR: Not subscribed to this topic.\n";
  }

  return 0;
}

int Node::get(string topic_name, string &content) {
  content = "";
  while (1) {
    Message get_msg = GetMessage(topic_name, this->id);
    zmqpp::message msg = get_msg.to_zmq_msg();
    this->socket.send(msg);

    zmqpp::message response;
    this->socket.receive(response);
    
    try {
      if (response.is_signal())
        throw NotSubscribed(topic_name);
    
    } catch(const NotSubscribed& e) {
      cout << "ERROR: Not subscribed to this topic.\n";
      return 0;
    }

    int type;
    response >> type;

    try {
      if (type != ANSWER)
        throw InvalidMessage(topic_name, (msg_type)type);
    
    } catch(const InvalidMessage& e) {
      cout << "ERROR: Got invalid message.\n";
    }

    AnswerMessage answer_msg(response);
    content = answer_msg.get_body();
    cout << "\t" << answer_msg << endl;
    if (content != "")
      break;
    usleep(RESEND_TIMEOUT);
  }

  return 0;
}

int Node::put(std::string topic_name, std::string content) {
  try {
    if (content == "")
      throw InvalidContent(topic_name);
  } catch(const InvalidContent& e) {
    cout << "ERROR: Invalid content.\n";
    return 0;
  }

  PutMessage put_msg = PutMessage(topic_name, content, this->id);
  zmqpp::message msg = put_msg.to_zmq_msg();
  this->socket.send(msg);
  return receive_ack(this->socket);
}

string Node::getId() const {
  return id;
}