#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/worker.hpp"

using namespace std;

Broker::Broker(zmqpp::context &context)
    : socket(context, zmqpp::socket_type::rep) {
  socket.bind("tcp://*:" + to_string(PROXY_PORT));
}

void Broker::run() {
  zmqpp::poller poller;
  poller.add(this->socket);

  while (1) {
    poller.poll();
    if (poller.events(this->socket)) {
      cout << this->topic_queue << endl;
      Worker w;
      w.run(topic_queue, socket);
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
