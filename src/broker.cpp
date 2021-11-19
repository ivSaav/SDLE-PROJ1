#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/broker.hpp"
#include "../include/worker.hpp"
#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"

using namespace std;

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

      int type;
      request >> type;
      if (type == PUT) {
      } else {
        cout << "Invalid message" << endl;
        // TODO Handle this
      }

      cout << this->topic_queue << endl;

    } else if (poller.events(this->s_subscribe)) {
      zmqpp::message request;
      this->s_subscribe.receive(request);
      // TODO Check if message is SUB, UNSUB or GET

      int type;
      request >> type;
      cout << "RECEIVED: " + Message::typeStrings[type] << endl;
      if (type == SUB) {

      } else if (type == UNSUB) {

      } else if (type == GET) {
        }
      } else {
        cout << "Invalid message" << endl;
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
