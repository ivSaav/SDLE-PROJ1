#include <iostream>
#include <string>

#include "../include/TopicQueue.hpp"
#include "../include/exceptions.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/node.hpp"

void usage() {
  std::cout
      << "./main id (sub topic)|(put topic content)|(unsub topic)|(get topic)";
}

int main(int argc, char *argv[]) {
  cout << "Running peer" << std::endl;
  zmqpp::context context;

  if (argc < 3) {
    usage();
    return 1;
  }

  string id = argv[1];
  string type = argv[2];
  Node peer1 = Node(context, id);

  try {
    if (type == "sub") {
      if (argc < 4)
        usage();

      string topic = argv[3];
      peer1.subscribe(topic);
    } else if (type == "put") {
      if (argc < 5)
        usage();

      string topic = argv[3];
      string content = argv[4];
      peer1.put(topic, content);
    } else if (type == "unsub") {
      if (argc < 4)
        usage();

      string topic = argv[3];
      peer1.unsubscribe(topic);
    } else if (type == "get") {
      if (argc < 4)
        usage();

      string topic = argv[3];
      string res;
      peer1.get(topic, res);
      cout << "GOT: " << res;
    } else
      usage();

  } catch (message_error e) {
    cout << e.what() << endl;
  }

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
