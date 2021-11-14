#include <iostream>
#include <string>

#include "../include/message/put_msg.hpp"
#include "../include/node.hpp"

void usage() { std::cout << "./main topic_name number_of_msgs"; }

int main(int argc, char *argv[]) {
  cout << "Running peer" << std::endl;
  zmqpp::context context;

  Node peer1 = Node(context);
  string content = "cont";
  peer1.put("topic", content);

  // zmqpp::socket server(context, zmqpp::socket_type::req);
  // server.connect("tcp://127.0.0.1:9000");

  // if (argc < 2) {
  // usage();
  // return 1;
  //}

  // std::string topic = argv[1];
  // int n = std::stoi(argv[2]);

  //// Send a single message from server to client
  // for (int i = 0; i < n; ++i) {
  // zmqpp::message request;
  // std::vector<int> file(10000);
  // for (int i = 0; i < 10000; ++i) {
  // file[i] = -1;
  // std::cout << file[i] << " ";
  //}
  // std::cout << std::endl;

  // request.push_back(file.data(), 10000 * sizeof(int));
  // server.send(request);

  // zmqpp::message response;
  // server.receive(response);

  // std::string content;
  // response >> content;

  // std::cout << "Got response: " << content << std::endl;
  // sleep(1);
  //}
  return 0;
}
