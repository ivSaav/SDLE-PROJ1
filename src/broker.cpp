#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"

using namespace std;

void answer_ack(zmqpp::socket &sock) {
  zmqpp::message ok_msg;
  ok_msg << zmqpp::signal::ok;
  sock.send(ok_msg);
}

Broker::Broker(zmqpp::context &context)
    : s_publish(context, zmqpp::socket_type::rep),
      s_subscribe(context, zmqpp::socket_type::rep) {
  s_publish.bind("tcp://*:" + to_string(PUB_PORT));
  s_subscribe.bind("tcp://*:" + to_string(SUB_PORT));
}

void Broker::run() {
  zmqpp::poller poller;
  poller.add(this->s_publish);
  poller.add(this->s_subscribe);

  while (1) {
    poller.poll();
    if (poller.events(this->s_publish)) {
      zmqpp::message request;
      this->s_publish.receive(request);

      string topic, content = "NONE";
      request >> topic;
      cout << "PUB topic: " << topic << " content: " << content << endl;

      answer_ack(this->s_publish);
    } else if (poller.events(this->s_subscribe)) {
      zmqpp::message request;
      this->s_subscribe.receive(request);

      string topic;
      request >> topic;
      cout << "SUB topic: " << endl;

      answer_ack(this->s_subscribe);
    }
  }
}

int main() {
  cout << "Running broker" << std::endl;
  zmqpp::context context;
  Broker broker(context);
  broker.run();

  //// create and bind a server socket
  // zmqpp::socket pubs(context, zmqpp::socket_type::rep);
  // pubs.bind("tcp://*:9001");

  // while (1) {
  // zmqpp::message put_req;
  // pubs.receive(put_req);

  // string topic, content;
  // put_req >> topic;
  // cout << topic << endl;

  // zmqpp::message ok_msg;
  // ok_msg << zmqpp::signal::ok;
  // pubs.send(ok_msg);
  //}

  // while (1) {
  // zmqpp::message request;
  // server.receive(request);
  // std::cout << "Remaining " << request.remaining() << std::endl;
  // const int *data;
  // request >> data;

  // int num_neg = 0;
  // for (int i = 0; i < 10000; ++i) {
  // if (data[i] == -1)
  // num_neg++;
  //}
  // std::cout << "NUM NEG " << num_neg << std::endl;

  // zmqpp::message response;
  // response << "Response";
  // server.send(response);
  //}
  return 0;
}
