#pragma once
#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

#include "../include/common.hpp"

using namespace std;

class Node {
public:
  Node(zmqpp::context &context, string id);
  ~Node();

  int get(std::string topic_name, std::string &msg);
  int put(std::string topic_name, std::string msg);
  int subscribe(std::string topic_name);
  int unsubscribe(std::string topic_name);
  void print(); // Print topic queue in broker, For debugging

private:

  int send_request(Message *m, string &request_id, int max_retries=1);
  int get_request(string request_id, shared_ptr<Message> &response, int max_retries=3);
  int delete_request(string id);
  void make_request(Message *request, Message &response);

  int seq_num=0;
  zmqpp::socket socket;
  string id;
};
