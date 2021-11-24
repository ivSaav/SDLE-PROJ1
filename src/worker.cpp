#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <functional>
#include <zmqpp/zmqpp.hpp>

#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/message.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/file_manager.hpp"
#include "../include/topic_queue.hpp"
#include "../include/worker.hpp"

using namespace std;

void Worker::handle_get(zmqpp::message &request) {
  GetMessage msg(request);
  string hash_id = hash_message(&msg);

  cout << msg << endl;

  if (!topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    KoMessage ko(msg.get_id());
    write_content_file(hash_id, &ko);
  }
  else {
    string answer_content = "";
    topic_queue.get(msg.get_id(), msg.get_topic(), answer_content);
    // If not in queue => answer_content = ""
    AnswerMessage ans(msg.get_topic(), answer_content, msg.get_id());
    cout << "\t" << ans << endl;
    write_content_file(hash_id, &ans);
  }
}

void Worker::handle_put(zmqpp::message &request) {
  PutMessage msg(request);
  string hash_id = hash_message(&msg);
  cout << msg << endl;

  topic_queue.put(msg.get_topic(), msg.get_body());
  OkMessage ok(msg.get_id());
  write_content_file(hash_id, &ok);
}

void Worker::handle_sub(zmqpp::message &request) {
  SubMessage msg(request);
  string hash_id = hash_message(&msg);
  cout << msg << endl;

  if (topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    cout << "\tNOT OK SUB" << endl;
    KoMessage ko(msg.get_id());
    write_content_file(hash_id, &ko);
  } else {
    cout << "\tOK SUB" << endl;
    topic_queue.subscribe(msg.get_id(), msg.get_topic());
    OkMessage ok(msg.get_id());
    write_content_file(hash_id, &ok);
  }
}

void Worker::handle_unsub(zmqpp::message &request) {
  UnsubMessage msg(request);
  string hash_id = hash_message(&msg);

  cout << msg << endl;
  if (!topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
    cout << "\tNOT OK UNSUB" << endl;
    KoMessage ko(msg.get_id());
    write_content_file(hash_id, &ko);
  } else {
    cout << "\tOK UNSUB" << endl;
    OkMessage ok(msg.get_id());
    write_content_file(hash_id, &ok);
  }
}

void Worker::handler(zmqpp::message &request) {
  int type;
  request >> type;
  string topic_name;
  cout << "WORKING ON: " + Message::typeStrings[type] << endl;

  if (type == PUT) {
    handle_put(request);
  } else if (type == SUB) {
    handle_sub(request);
  } else if (type == UNSUB) {
    handle_unsub(request);
  } else if (type == GET) {
    handle_get(request);
  } else {
    cout << "Invalid message" << endl;
  }
}

void Worker::work() {
  zmqpp::context context;
  zmqpp::socket worker(context, zmqpp::socket_type::req);

  cout << "ID_ " << this->id << endl; 
  cout << "ID_ " << this->topic_queue;
  fflush(stdout);
  worker.set(zmqpp::socket_option::identity, this->id);
  worker.connect("tcp://localhost:" + to_string(WORKER_PORT)); // backend
  cout << this->topic_queue;

  while (1) {
    //  Tell backend we're ready for work
    worker.send("READY");

    //  Read and save all frames until we get an empty frame
    //  In this example there is only 1 but it could be more
    //  Get request, send reply
    zmqpp::message request;
    worker.receive(request);

    if (request.is_signal()) { // Request from dad to stop
      zmqpp::signal s;
      request >> s;
      if (s == zmqpp::signal::stop) {
        worker.close();
        context.terminate();
        return;
      }
    }

    zmqpp::message response;
    handler(request);
  }
}

void Worker::run() {
  this->t = thread(&Worker::work, this);
}

void Worker::join() { t.join(); }
