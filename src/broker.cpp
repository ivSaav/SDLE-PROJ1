#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <signal.h>
#include <zmqpp/socket.hpp>
#include <zmqpp/socket_types.hpp>
#include <zmqpp/zmqpp.hpp>
// Serialization
#include <cereal/archives/json.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/titanic.hpp"

using namespace std;

static volatile int s_interrupted = 0;
static void s_signal_handler(int signal_value) { s_interrupted = 1; }

static void s_catch_signals(void) {
  struct sigaction action;
  action.sa_handler = s_signal_handler;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  sigaction(SIGINT, &action, NULL);
  // sigaction (SIGTERM, &action, NULL);
}

Broker::Broker(zmqpp::context &context)
    : frontend(context, zmqpp::socket_type::rep),
      backend(context, zmqpp::socket_type::router), state(this->topic_queue) {

  frontend.bind("tcp://*:" + to_string(CLIENT_PORT));
  backend.bind("tcp://*:" + to_string(WORKER_PORT));
}

void Broker::cleanUp() {
  for (Worker *w : workers) {
    string worker_addr = w->getId();
    zmqpp::message w_req(worker_addr, "", zmqpp::signal::stop);
    backend.send(w_req);
    w->join();
    delete (w);
  }

  this->state.save();
  this->backend.close();
  this->frontend.close();
}

void Broker::run() {
  for (int i = 0; i < NUM_WORKERS; ++i) {
    // Load state
    this->state.load();
    Worker *w = new Worker(this->topic_queue, this->state, to_string(i));
    workers.push_back(w);
    workers.at(i)->run();
  }

  s_catch_signals();

  queue<string> worker_queue;           // Contains available workers
  queue<TitanicMessage> requests_queue; // Contains pending requests

  while (1) {
    zmqpp::poller poller;
    poller.add(backend);

    // Don't accept new requests if we don't have workers or
    // when we want to exit
    if (worker_queue.size() && s_interrupted == 0)
      poller.add(frontend);
    else
      poller.remove(frontend);

    poller.poll();
    if (s_interrupted == 1 && worker_queue.size() == NUM_WORKERS) {
      this->cleanUp();
      break;
    }

    if (poller.has(frontend) && poller.events(frontend)) {
      zmqpp::message msg;
      frontend.receive(msg);
      if (msg.is_signal()) { // Debug print msg
        zmqpp::signal sig;
        msg >> sig;
        if (sig == zmqpp::signal::test)
          cout << topic_queue << endl;
        frontend.send(zmqpp::signal::ok);
      } else {
        Titanic t;
        t.handle(msg, frontend, requests_queue);
      }
    }

    if (poller.events(backend)) {

      //  Queue worker address for LRU routing
      // e is used to read the empty string between msgs
      string w_address, client_addr, e;
      zmqpp::message rcv;
      backend.receive(rcv);
      rcv >> w_address >> e >> client_addr;
      worker_queue.push(w_address);
    }

    // If we have available workers or pending requests
    if (worker_queue.size() > 0 && requests_queue.size() > 0) {

      //  Now get next client request, route to LRU worker
      //  Client request is [address][empty][request]
      std::string client_addr, worker_addr, e;
      TitanicMessage &t_msg = requests_queue.front();
      zmqpp::message msg = t_msg.getMessage()->to_zmq_msg();
      // msg >> client_addr >> e;

      // Pick worker
      worker_addr = worker_queue.front(); // worker_queue [0];
      worker_queue.pop();

      // Add routing to new message
      zmqpp::message w_req(worker_addr, "");
      // Append request to new message
      while (msg.remaining()) {
        string content;
        msg >> content;
        w_req.push_back(content);
      }
      backend.send(w_req);
      requests_queue.pop();

      // Save state
    }
  }
}

int main() {
  cout << "Running broker" << std::endl;
  zmqpp::context context;
  Broker broker(context);
  broker.run();
  context.terminate();

  return 0;
}
