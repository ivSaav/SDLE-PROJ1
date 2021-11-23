#ifndef TITANIC_H
#define TITANIC_H

#include <zmqpp/zmqpp.hpp>
#include <string>
#include <queue>

#define REQ_PATH "./requests/"

#include "message/titanic_messages.hpp"
#include "common.hpp"

using namespace std;

//💑🚢🌊💥💀
class Titanic {
private:

public:
  void handle(zmqpp::message &req, zmqpp::socket &client, queue<TitanicGetMessage> &requests) {
    TitanicMessage msg(req);


    if (msg.isDel()) {
      cout << msg.to_string() << endl;
    } else if (msg.isGet()) {
      TitanicGetMessage m(req, msg.getId());

      cout << "SENDING TO WORKER " << m.to_string() << endl;
      requests.push(m);

      client.send(zmqpp::signal::ok);
    } else if (msg.isPut()) {
      cout << msg.to_string() << endl;
    }

    
  }

};
#endif /* ifndef TITANIC_H */
