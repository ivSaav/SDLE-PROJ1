#include <iostream>
#include <string>

#include "../include/TopicQueue.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/node.hpp"

void usage() { std::cout << "./main (sub)|(pub content)"; }

int main(int argc, char *argv[]) {
  cout << "Running peer" << std::endl;
  zmqpp::context context;

  if (argc < 2) {
    usage();
    return 1;
  }

  string type = argv[1];
  Node peer1 = Node(context, "peer1");

  if (type == "sub") {
    peer1.subscribe("topic");
    string content;
    fflush(stdout);
    peer1.get("topic", content);
    cout << "GOT: " << content;
  } else if (type == "pub") {
    if (argc < 3) {
      usage();
      return 1;
    }
    string content = argv[2];
    peer1.put("topic", content);
  } else
    usage();

  // TopicQueue q;

  // q.put("test", "content1");
  // q.put("test", "content2");
  // q.put("test", "content3");
  // q.put("test", "content4");
  // q.put("test", "content5");
  // q.put("test", "content6");

  // string c;
  // q.subscribe("peer1", "test");
  // q.subscribe("peer2", "test");

  // bool st;
  // st = q.get("peer1", "test", c);
  // st = q.get("peer1", "test", c);
  // st = q.get("peer1", "test", c);
  // st = q.get("peer1", "test", c);
  // st = q.get("peer1", "test", c);
  // st = q.get("peer2", "test", c);
  // st = q.get("peer2", "test", c);
  // st = q.get("peer2", "test", c);

  // q.unsubscribe("peer2", "test");
  // q.trimQueue("test");
  // cout << q << endl;

  return 0;
}
