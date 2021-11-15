#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"
#include "../include/message/put_msg.hpp"

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

      // TODO Check if message is PUT
      int type;
      request >> type;
      if (type == PUT) {
        PutMessage msg(request);
        cout << msg << endl;
      } else {
        cout << "Invalid message" << endl;
        // TODO Handle this
      }

      answer_ack(this->s_publish);
    } else if (poller.events(this->s_subscribe)) {
      zmqpp::message request;
      this->s_subscribe.receive(request);
      // TODO Check if message is SUB, UNSUB or GET

      int type;
      request >> type;
      if (type == SUB) {
        SubMessage msg(request);
        cout << msg << endl;
      } else if (type == UNSUB) {
        UnsubMessage msg(request);
        cout << msg << endl;
      } else if (type == GET) {
        GetMessage msg(request);
        cout << msg << endl;
      } else {
        cout << "Invalid message" << endl;
      }

      answer_ack(this->s_subscribe);
    }
  }
}

int main() {
  cout << "Running broker" << std::endl;
  zmqpp::context context;
  Broker broker(context);
  broker.run();
  return 0;
}
