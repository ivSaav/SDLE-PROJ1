#include <iostream>
#include <string>

#include "../include/message/put_msg.hpp"
#include "../include/node.hpp"

void usage() { std::cout << "./main sub|pub"; }

int main(int argc, char *argv[]) {
  cout << "Running peer" << std::endl;
  zmqpp::context context;

  if (argc < 2) {
    usage();
    return 1;
  }
  string type = argv[1];
  Node peer1 = Node(context);

  if (type == "sub") {
    string content;
    peer1.get("topic", content);
  } else if (type == "pub") {
    string content = "cont";
    peer1.put("topic", content);
  }

  return 0;
}
