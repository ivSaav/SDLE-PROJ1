#pragma once
#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/common.hpp"

using namespace std;

class Node {
public:
  Node(zmqpp::context &context, string id)
      : s_publish(context, zmqpp::socket_type::req),
        s_subscribe(context, zmqpp::socket_type::req), id(id) {

    this->s_subscribe.connect("tcp://127.0.0.1:" + to_string(SUB_PORT));
    this->s_publish.connect("tcp://127.0.0.1:" + to_string(PUB_PORT));
  }
  ~Node();

  int get(std::string topic_name, std::string &msg);
  int put(std::string topic_name, std::string msg);
  int subscribe(std::string topic_name);
  int unsubscribe(std::string topic_name);

private:
  zmqpp::socket s_subscribe;
  zmqpp::socket s_publish;
  string id;
};
