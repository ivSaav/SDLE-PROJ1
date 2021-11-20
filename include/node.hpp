#pragma once
#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/common.hpp"

#define RESEND_TIMEOUT 500000 // Interval between GET msg's in micros

using namespace std;

class Node {
public:
  Node(zmqpp::context &context, string id);
  ~Node();

  int get(std::string topic_name, std::string &msg);
  int put(std::string topic_name, std::string msg);
  int subscribe(std::string topic_name);
  int unsubscribe(std::string topic_name);

private:
  zmqpp::socket socket;
  string id;
};
