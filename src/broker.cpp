#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

using namespace std;

int main() {
  std::cout << "Running broker" << std::endl;

  zmqpp::context context;
  // create and bind a server socket
  zmqpp::socket pubs(context, zmqpp::socket_type::rep);
  pubs.bind("tcp://*:9001");

  while (1) {
    zmqpp::message put_req;
    pubs.receive(put_req);

    string topic, content;
    put_req >> topic;
    cout << topic << endl;

    zmqpp::message ok_msg;
    ok_msg << zmqpp::signal::ok;
    pubs.send(ok_msg);
  }

  // while (1) {
  // zmqpp::message request;
  // server.receive(request);
  // std::cout << "Remaining " << request.remaining() << std::endl;
  // const int *data;
  // request >> data;

  // int num_neg = 0;
  // for (int i = 0; i < 10000; ++i) {
  // if (data[i] == -1)
  // num_neg++;
  //}
  // std::cout << "NUM NEG " << num_neg << std::endl;

  // zmqpp::message response;
  // response << "Response";
  // server.send(response);
  //}
  return 0;
}
