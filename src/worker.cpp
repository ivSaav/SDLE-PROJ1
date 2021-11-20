#include <iostream>
#include <unistd.h>
#include <zmqpp/zmqpp.hpp>

#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/message.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"

using namespace std;

void Worker::handle_get(zmqpp::message &request, zmqpp::message &response) {
  GetMessage msg(request);
  cout << msg << endl;

  if (!topic_queue.is_subscribed(msg.get_id(), msg.get_topic()))
    response.push_back(zmqpp::signal::ko);
  else {
    string answer_content = "";
    topic_queue.get(msg.get_id(), msg.get_topic(), answer_content);
    // If not in queue => answer_content = ""
    AnswerMessage ans(msg.get_topic(), answer_content, msg.get_id());
    cout << "\t" << ans << endl;
    ans.append_to_zmq_msg(response);
  }
}

void Worker::handle_put(zmqpp::message &request, zmqpp::message &response) {
  PutMessage msg(request);
  cout << msg << endl;

  topic_queue.put(msg.get_topic(), msg.get_body());
  response.push_back(zmqpp::signal::ok);
}

void Worker::handle_sub(zmqpp::message &request, zmqpp::message &response) {
  SubMessage msg(request);
  cout << msg << endl;
  if (topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    response.push_back(zmqpp::signal::ko); // Already subscribed
  } else {
    topic_queue.subscribe(msg.get_id(), msg.get_topic());
    response.push_back(zmqpp::signal::ok);
  }
}

void Worker::handle_unsub(zmqpp::message &request, zmqpp::message &response) {
  UnsubMessage msg(request);
  cout << msg << endl;
  if (!topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    response.push_back(zmqpp::signal::ko);
  } else {
    topic_queue.unsubscribe(msg.get_id(), msg.get_topic());
    response.push_back(zmqpp::signal::ok);
  }
}

void Worker::handler(zmqpp::message &request, zmqpp::message &response) {
  int type;
  request >> type;
  string topic_name;
  cout << "RECEIVED: " + Message::typeStrings[type] << endl;

  if (type == PUT) {
    handle_put(request, response);
  } else if (type == SUB) {
    handle_sub(request, response);
  } else if (type == UNSUB) {
    handle_unsub(request, response);
  } else if (type == GET) {
    handle_get(request, response);
  } else {
    cout << "Invalid message" << endl;
  }
}

void Worker::work(thread::id id) {
  zmqpp::context context;
  zmqpp::socket worker(context, zmqpp::socket_type::req);

  hash<thread::id> hasher;
  string worker_id = to_string(hasher(id));
  worker.set(zmqpp::socket_option::identity, worker_id);
  worker.connect("tcp://localhost:" + to_string(WORKER_PORT)); // backend

  //  Tell backend we're ready for work
  cout << "RDY: " << worker_id << endl;
  worker.send("READY");

  while (1) {
    //  Read and save all frames until we get an empty frame
    //  In this example there is only 1 but it could be more
    //  Get request, send reply
    zmqpp::message request;
    worker.receive(request);
    string address, e; // e => empty frame
    request >> address >> e;
    std::cout << "Worker:" << worker_id << "From: " << address << "_"
              << "_" << e << std::endl;

    zmqpp::message response(address, "");
    handler(request, response);
    worker.send(response);
  }
}

void Worker::run() {
  t = thread(&Worker::work, this, t.get_id());
}

void Worker::join() { t.join(); }
