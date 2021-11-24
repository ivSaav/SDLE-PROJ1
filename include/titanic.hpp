#ifndef TITANIC_H
#define TITANIC_H

#include <queue>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <fstream>
#include <filesystem>

#include "common.hpp"
#include "message/message.hpp"
#include "message/titanic_messages.hpp"
#include "file_manager.hpp" // For functions to save

using namespace std;

//💑 🚢 🌊 💥 💀
class Titanic {
private:
public:
  void handle(zmqpp::message &req, zmqpp::socket &client,
              queue<TitanicMessage> &requests) {
    TitanicMessage msg(req);

    if (msg.isDel()) {
      cout << msg.to_string() << endl;
      if (file_exists(msg.getId()))
        delete_file(msg.getId());

      client.send(zmqpp::signal::ok);
    } else if (msg.isReq()) {
      cout << "REQ" << endl;
      cout << "SENDING TO WORKER " << msg.to_string() << endl;

      string hash_id = hash_message(msg);
      if (file_exists(hash_id)){ // Request already made
        zmqpp::message response(hash_id, zmqpp::signal::ko);
        client.send(response);
      } else {
        cout << "accepted request " << hash_id;
        create_file(hash_id);
        requests.push(msg);
        zmqpp::message response(hash_id, zmqpp::signal::ok);
        client.send(response);
      }
    } else if (msg.isGet()) {
      cout << msg.to_string() << endl;

      string id = msg.getId();
      cout << "getting request " << id;
      if (file_exists(id)) {  // Request was made
        if (file_has_content(id)) { // Request was processed
          Message response;
          get_content_file(id, response);
          Message *p = &response; // For polymorphic virtual funct
          cout << "FINISHED" << p->to_string() << endl;
          zmqpp::message m = p->to_zmq_msg();
          client.send(m);
        } else {
          client.send(zmqpp::signal::ko);
        }
      } else {
        client.send(zmqpp::signal::stop);
      }
    } else
      cout << "INVALID CLIENT REQUEST" << endl;
  }
};
#endif /* ifndef TITANIC_H */
