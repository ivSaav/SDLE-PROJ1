#include <iostream>
#include <string>

#include "../include/better_q.hpp"
#include "../include/exceptions.hpp"
#include "../include/message/put_msg.hpp"
#include "../include/node.hpp"
#include "../include/worker.hpp"
#include <unistd.h>

void usage() {
  std::cout
      << "./main id (sub topic)|(put topic content)|(unsub topic)|(get topic)";
}

int main(int argc, char *argv[]) {
  cout << "Running peer" << std::endl;
  zmqpp::context context;

  string id = argv[1];
  string type = argv[2];
  Node peer1(context, id);

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
    } else if (type == "print") {
      peer1.print();
    } else
      usage();

  } catch (message_error e) {
    cout << e.what() << endl;
  }

  return 0;
}
