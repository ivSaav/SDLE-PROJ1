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

/* PRIVATE */

// Returns -1 if request couldn't be made. 0 If request was received by server
int Node::send_request(Message *m, string &request_id, int max_retries) {
  TitanicMessage req_msg(TitanicMessage::REQ_TIT, m);

  bool done = false;
  int n_tries = 0;
  while (n_tries < max_retries && !done) {
    ++n_tries;

    cout << "SENDING REQUEST" << req_msg.to_string() << endl;
    zmqpp::message msg = req_msg.to_zmq_msg();
    this->socket.send(msg);

    // Get Response
    zmqpp::message response;
    this->socket.receive(response);
    response >> request_id;
    cout << "GOT " << request_id << endl;
    zmqpp::signal s;
    response >> s;
    if (zmqpp::signal::ok == s) {
      done = true;
    } else if (zmqpp::signal::ko == s) {
      usleep(RESEND_TIMEOUT);
    } else {
      throw InvalidMessage("REQ", TitanicMessage::REQ_TIT);
    }
  }

  return done ? 0 : -1;
}

// Returns -1 if request wasn't made to the server. 
// 1 If the request is still being processed by the server
// 0 If success
int Node::get_request(string request_id, shared_ptr<Message> &response, int max_retries) {
  int num_tries = 0;
  bool done = false;
  while (num_tries < max_retries && !done) {
    ++num_tries;
    // Send get request
    TitanicMessage get_msg(TitanicMessage::GET_TIT, request_id);
    zmqpp::message get = get_msg.to_zmq_msg();
    this->socket.send(get);

    // Receive response
    zmqpp::message zmq_response;
    this->socket.receive(zmq_response); // TODO set this to non blocking

    if (zmq_response.is_signal()) {
      zmqpp::signal s;
      zmq_response >> s;
      if (s == zmqpp::signal::ko) { // Still waiting for server
        usleep(RESEND_TIMEOUT);
      } else if (s == zmqpp::signal::stop) { // Server has no knowledge of request
        return -1;
      }
    } else { // Normal message
      int type;
      zmq_response >> type;
      shared_ptr<Message> m;
      if (type == msg_type::ANSWER)
        m = make_shared<AnswerMessage>(AnswerMessage(zmq_response));
      else
        m = make_shared<Message>(Message(zmq_response, (msg_type) type));

      cout << "RECEIVED MESSAGE " << m->to_string() << endl;
      response = m;
      done = true;
    }
  }

  return done ? 0 : 1;
}

// Returns 0 if server deletes the request, -1 if the received msg doesn't make sense
int Node::delete_request(string request_id) {
  int num_tries = 0;
  // Send delete request
  TitanicMessage del_msg(TitanicMessage::DEL_TIT, request_id);
  cout << del_msg.to_string() << endl;

  zmqpp::message zmq_msg = del_msg.to_zmq_msg();
  this->socket.send(zmq_msg);

  // Get Response
  zmqpp::signal sig;
  this->socket.receive(sig);
  if (sig == zmqpp::signal::ok)
    return 0;

  return -1;
}

void Node::make_request(Message *request, Message &response) {
  string req_id;

}


/* PUBLIC */

Node::Node(zmqpp::context &context, string id)
    : socket(context, zmqpp::socket_type::req), id(id) {

  this->socket.connect("tcp://127.0.0.1:" + to_string(CLIENT_PORT));
  this->socket.set(zmqpp::socket_option::identity, this->id);
}

Node::~Node() { this->socket.close(); }

int Node::subscribe(std::string topic_name) {
  SubMessage sub_msg = SubMessage(topic_name, this->id);
  shared_ptr<Message> response;
  string req_id;

  if (this->send_request(&sub_msg, req_id)) throw FailedRequestBegin(topic_name);
  if (this->get_request(req_id, response)) {
    this->delete_request(req_id);
    throw FailedRequestRetreive(topic_name);
  }
  if (this->delete_request(req_id)) throw FailedRequestDelete(topic_name);

  if (response->get_type() == msg_type::KO)
   throw AlreadySubscribed(topic_name);
  if (response->get_type() == msg_type::OK)
    return 0;

  return 1;
}

int Node::unsubscribe(std::string topic_name) {
  UnsubMessage unsub_msg = UnsubMessage(topic_name, this->id);
  shared_ptr<Message> response;
  string req_id;

  if (this->send_request(&unsub_msg, req_id)) throw FailedRequestBegin(topic_name);
  if (this->get_request(req_id, response)) {
    this->delete_request(req_id);
    throw FailedRequestRetreive(topic_name);
  }
  if (this->delete_request(req_id)) throw FailedRequestDelete(topic_name);

  if (response->get_type() == msg_type::KO)
   throw NotSubscribed(topic_name);

  if (response->get_type() == msg_type::OK)
    return 0;

  return 1;
}

int Node::get(string topic_name, string &content) {
  Message get_msg = GetMessage(topic_name, this->id);
  shared_ptr<Message> response;
  string req_id;

  if (this->send_request(&get_msg, req_id)) throw FailedRequestBegin(topic_name);
  if (this->get_request(req_id, response , INT32_MAX)) {
    this->delete_request(req_id);
    throw FailedRequestRetreive(topic_name);
  }
  if (this->delete_request(req_id)) throw FailedRequestDelete(topic_name);

  if (response->get_type() == msg_type::KO)
   throw NotSubscribed(topic_name);

  AnswerMessage* answer_response = (AnswerMessage*) &(*response);
  content = answer_response->get_body();
  return 0;
}

int Node::put(std::string topic_name, std::string content) {
  if (content == "")
    throw InvalidContent(topic_name);

  PutMessage put_msg = PutMessage(topic_name, content, this->id);
  shared_ptr<Message> response;
  string req_id;

  if (this->send_request(&put_msg, req_id)) throw FailedRequestBegin(topic_name);
  if (this->get_request(req_id, response)) throw FailedRequestRetreive(topic_name);
  if (this->delete_request(req_id)) throw FailedRequestDelete(topic_name);

  if (response->get_type() == msg_type::OK)
   return 0;
  return 1;
}
