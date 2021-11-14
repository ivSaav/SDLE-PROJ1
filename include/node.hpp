#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

using namespace std;

class Node {
public:
  Node(zmqpp::context &context);
  ~Node();

  int get(std::string topic_name, std::string &msg);
  int put(std::string topic_name, std::string msg);
  int subscribe(std::string topic_name);
  int unsubscribe(std::string topic_name);

private:
  zmqpp::socket s_subscribe;
  zmqpp::socket s_publish;

  int send(zmqpp::socket &, zmqpp::message &);
};
