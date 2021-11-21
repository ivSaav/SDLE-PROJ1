#include <cstdlib>
#include <iostream>
#include <queue>
#include <zmqpp/zmqpp.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/worker.hpp"

using namespace std;

Broker::Broker(zmqpp::context &context)
    : frontend(context, zmqpp::socket_type::router),
    backend(context, zmqpp::socket_type::router) {
  frontend.bind("tcp://*:" + to_string(CLIENT_PORT));
  backend.bind("tcp://*:" + to_string(WORKER_PORT));
}

void Broker::run() {
  vector<Worker*> workers;
  for (int i=0; i<3; ++i) {
    Worker *w = new Worker(this->topic_queue, to_string(i));
    workers.push_back(w);
    workers.at(i)->run();
  }

  queue<string> worker_queue; // Contains available workers

  while (1) {
    zmqpp::poller poller;
    poller.add(backend);

    if (worker_queue.size())
      poller.add(frontend);
    else
      poller.remove(frontend);

    poller.poll();

    if (poller.events(backend)) {

      //  Queue worker address for LRU routing
      // e is used to read the empty string between msgs
      string w_address, client_addr, e;
      zmqpp::message rcv;
      backend.receive(rcv);
      rcv >> w_address >> e >> client_addr;
      worker_queue.push(w_address);

      //  If client reply, send rest back to frontend
      if (client_addr.compare("READY") != 0) {
        rcv >> e;

        zmqpp::message w_rep(client_addr, "");
        // Append request to new message
        while (rcv.remaining()) {
          string content;
          rcv >> content;
          w_rep.push_back(content);
        }
        frontend.send(w_rep);
      }
    } else if (poller.has(frontend) && poller.events(frontend)) {

      //  Now get next client request, route to LRU worker
      //  Client request is [address][empty][request]
      std::string client_addr, worker_addr, e;
      zmqpp::message msg;
      frontend.receive(msg);
      msg >> client_addr >> e;

      // Pick worker
      worker_addr = worker_queue.front(); // worker_queue [0];
      worker_queue.pop();

      // Add routing to new message
      zmqpp::message w_req(worker_addr, "", client_addr, "");
      // Append request to new message
      while (msg.remaining()) {
        string content;
        msg >> content;
        w_req.push_back(content);
      }
      backend.send(w_req);
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
