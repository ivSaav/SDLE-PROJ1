#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <queue>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
// Serialization
#include <cereal/archives/binary.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"

using namespace std;

static volatile int s_interrupted = 0;
static void s_signal_handler (int signal_value)
{
    s_interrupted = 1;
}

static void s_catch_signals (void)
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    // sigaction (SIGTERM, &action, NULL);
}

Broker::Broker(zmqpp::context &context)
    : frontend(context, zmqpp::socket_type::router),
    backend(context, zmqpp::socket_type::router) {
  frontend.bind("tcp://*:" + to_string(CLIENT_PORT));
  backend.bind("tcp://*:" + to_string(WORKER_PORT));
}

void Broker::cleanUp() {
  for (Worker *w: workers) {
    string worker_addr = w->getId();
    zmqpp::message w_req(worker_addr, "", zmqpp::signal::stop);
    backend.send(w_req);
    w->join();
    delete(w);
  }

  this->backend.close();
  this->frontend.close();
}

void Broker::run() {
  for (int i=0; i<NUM_WORKERS; ++i) {
    Worker *w = new Worker(this->topic_queue, to_string(i));
    workers.push_back(w);
    workers.at(i)->run();
  }

  s_catch_signals ();

  queue<string> worker_queue; // Contains available workers

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
    } 

    if (poller.has(frontend) && poller.events(frontend)) {

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

void Broker::save_state() {
  ofstream os(STATE_FILE, ios::binary);
  cereal::BinaryOutputArchive archive(os);
  
  archive(this->topic_queue);
}

void Broker::load_state() {
  ifstream is(STATE_FILE, ios::binary);
  if(is.is_open()) {
    cereal::BinaryInputArchive archive( is );
    
    archive( this->topic_queue );
    cout << "Loaded state" << endl;
  }
}

int main() {
  cout << "Running broker" << std::endl;
  zmqpp::context context;
  Broker broker(context);
  broker.load_state();
  broker.test();
  broker.save_state();
  // context.terminate();
  return 0;
}
