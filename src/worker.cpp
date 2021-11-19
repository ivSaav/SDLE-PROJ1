#include <iostream>
#include <unistd.h>
#include <zmqpp/zmqpp.hpp>

#include "../include/worker.hpp"
#include "../include/message/message.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/topic_queue.hpp"

using namespace std;

void answer(zmqpp::socket &sock, Message &msg) {
  zmqpp::message m = msg.to_zmq_msg();
  sock.send(m);
}

void answer(zmqpp::socket &sock, zmqpp::signal sig) {
  zmqpp::message ok_msg;
  ok_msg << sig;
  sock.send(ok_msg);
}

void answer_ack(zmqpp::socket &sock) { answer(sock, zmqpp::signal::ok); }

void answer_nack(zmqpp::socket &sock) { answer(sock, zmqpp::signal::ko); }


void handle_get(zmqpp::message &request, TopicQueue &topic_queue, zmqpp::socket s) {
  GetMessage msg(request);
  cout << msg << endl;

  if (!topic_queue.is_subscribed(msg.get_id(), msg.get_topic()))
    answer_nack(s);
  else {
    string answer_content = "";
    topic_queue.get(msg.get_id(), msg.get_topic(), answer_content);
    // If not in queue => answer_content = ""
    AnswerMessage ans(msg.get_topic(), answer_content, msg.get_id());
    cout << "\t" << ans << endl;
    answer(s, ans);
  }
}

void handle_put(zmqpp::message &request, TopicQueue &topic_queue, zmqpp::socket s) {
  PutMessage msg(request);
  cout << msg << endl;

  topic_queue.put(msg.get_topic(), msg.get_body());
  answer_ack(s);
}

void handle_sub(zmqpp::message &request, TopicQueue &topic_queue, zmqpp::socket s) {
  SubMessage msg(request);
  cout << msg << endl;
  if (topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    answer_nack(s); // Already subscribed
  } else {
    topic_queue.subscribe(msg.get_id(), msg.get_topic());
    answer_ack(s);
  }
}

void handle_unsub(zmqpp::message &request, TopicQueue &topic_queue, zmqpp::socket s) {
  UnsubMessage msg(request);
  cout << msg << endl;
  if (!topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    answer_nack(s);
  } else {
    topic_queue.unsubscribe(msg.get_id(), msg.get_topic());
    answer_ack(s);
  }
}

void handler(thread::id id) {
  zmqpp::context context;
  zmqpp::socket worker(context, zmqpp::socket_type::req);

  hash<thread::id> hasher;
  worker.set(zmqpp::socket_option::identity, hasher(id));
  worker.connect("tcp://localhost:5673"); // backend

  //  Tell backend we're ready for work
  worker.send("READY");

  while (1) {
    //  Read and save all frames until we get an empty frame
    //  In this example there is only 1 but it could be more
    //  Get request, send reply
    zmqpp::message msg;
    worker.receive(msg);
    string address, request, e; // e => empty frame
    msg << address << e << request;
    msg.extract(address, e, request);
    std::cout << "Worker:" << address << "From: " << address << "_" << request << std::endl;

    zmqpp::message msg1(address, "", "OK");
    worker.send(msg1);
  }
}

void Worker::run() {
  t = thread(handler, t.get_id());
}

void Worker::join() {
  t.join();
}
