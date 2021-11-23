#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "../include/common.hpp"
#include "../include/exceptions.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/message/titanic_messages.hpp"
#include "../include/node.hpp"

Node::Node(zmqpp::context &context, string id)
    : socket(context, zmqpp::socket_type::req), id(id) {

  this->socket.connect("tcp://127.0.0.1:" + to_string(CLIENT_PORT));
  this->socket.set(zmqpp::socket_option::identity, this->id);
}

Node::~Node() { this->socket.close(); }

int Node::subscribe(std::string topic_name) {
  SubMessage sub_msg = SubMessage(topic_name, this->id);
  // Send request to server
  TitanicMessage req_msg(TitanicMessage::REQ_TIT, sub_msg);
  cout << req_msg.to_string() << endl;
  zmqpp::message msg = req_msg.to_zmq_msg();
  this->socket.send(msg);
  // Get Response
  zmqpp::message response;
  int id;
  this->socket.receive(response);
  response >> id;
  cout << "GOT " << id << endl;
  zmqpp::signal s;
  response >> s;
  assert(zmqpp::signal::ok == s);

  // Poll for reply
  int num_tries = 3;
  while (num_tries > 0) {
    --num_tries;
    TitanicMessage get_msg(TitanicMessage::GET_TIT, sub_msg, id);
    zmqpp::message get = get_msg.to_zmq_msg();
    this->socket.send(get);

    zmqpp::message response;
    this->socket.receive(response); // TODO set this to non blocking
    if (response.is_signal()) {
      zmqpp::signal s;
      response >> s;
      if (s == zmqpp::signal::ko) {
        cout << "Didn't process msg" << endl;
      }
    } else { // TODO bad else
    }
  }

  return 0;
  // zmqpp::message msg = sub_msg.to_zmq_msg();
  // this->socket.send(msg);

  // if (receive_ack(this->socket))
  // throw AlreadySubscribed(topic_name);

  // return 0;
}

int Node::unsubscribe(std::string topic_name) {
  UnsubMessage unsub_msg = UnsubMessage(topic_name, this->id);
  zmqpp::message msg = unsub_msg.to_zmq_msg();
  this->socket.send(msg);
  if (receive_ack(this->socket))
    throw NotSubscribed(topic_name);

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
    if (response.is_signal())
      throw NotSubscribed(topic_name);

    int type;
    response >> type;
    if (type != ANSWER)
      throw InvalidMessage(topic_name, (msg_type)type);

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
  if (content == "")
    throw InvalidContent(topic_name);
  PutMessage put_msg = PutMessage(topic_name, content, this->id);
  zmqpp::message msg = put_msg.to_zmq_msg();
  this->socket.send(msg);
  return receive_ack(this->socket);
}
