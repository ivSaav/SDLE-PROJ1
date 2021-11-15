#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/broker.hpp"
#include "../include/common.hpp"
#include "../include/message/answer_msg.hpp"
#include "../include/message/put_msg.hpp"

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
        PutMessage msg(request);
        cout << msg << endl;
        this->topic_queue.put(msg.get_topic(), msg.get_body());
      } else {
        cout << "Invalid message" << endl;
        // TODO Handle this
      }

      cout << this->topic_queue << endl;

      answer_ack(this->s_publish);
    } else if (poller.events(this->s_subscribe)) {
      zmqpp::message request;
      this->s_subscribe.receive(request);
      // TODO Check if message is SUB, UNSUB or GET

      int type;
      request >> type;
      cout << "RECEIVED: " + Message::typeStrings[type] << endl;
      if (type == SUB) {
        SubMessage msg(request);
        cout << msg << endl;
        if (this->topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
          answer_nack(this->s_subscribe); // Already subscribed
        } else {
          this->topic_queue.subscribe(msg.get_id(), msg.get_topic());
          answer_ack(this->s_subscribe);
        }

      } else if (type == UNSUB) {
        UnsubMessage msg(request);
        cout << msg << endl;
        if (!this->topic_queue.is_subscribed(msg.get_id(), msg.get_topic())) {
          answer_nack(this->s_subscribe);
        } else {
          this->topic_queue.unsubscribe(msg.get_id(), msg.get_topic());
          answer_ack(this->s_subscribe);
        }

      } else if (type == GET) {
        GetMessage msg(request);
        cout << msg << endl;

        if (!this->topic_queue.is_subscribed(msg.get_id(), msg.get_topic()))
          answer_nack(this->s_subscribe);
        else {
          string answer_content = "";
          this->topic_queue.get(msg.get_id(), msg.get_topic(), answer_content);
          // If not in queue => answer_content = ""
          AnswerMessage ans(msg.get_topic(), answer_content, msg.get_id());
          cout << "\t" << ans << endl;
          answer(this->s_subscribe, ans);
        }
      } else {
        cout << "Invalid message" << endl;
      }
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
