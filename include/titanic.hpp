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
  #ifdef DEBUG
    cout << "RECEIVED: " << msg.to_string() << endl;
  #endif


    if (msg.isDel()) {
    #ifdef DEBUG
      cout << "DELETING " << msg.getId() << endl;
    #endif
      if (file_exists(msg.getId()))
        delete_file(msg.getId());

      client.send(zmqpp::signal::ok);
    } else if (msg.isReq()) {
      string hash_id = hash_message(msg);

    #ifdef DEBUG
      cout << "HANDLING SOMETHING" << endl;
    #endif

      if (file_exists(hash_id)){ // Request already made
        zmqpp::message response(hash_id, zmqpp::signal::ko);
        client.send(response);
      } else {
        create_file(hash_id);
        cout << "SENDING TO WORKER " << msg.to_string() << endl;
        requests.push(msg);
        zmqpp::message response(hash_id, zmqpp::signal::ok);
        client.send(response);
      }
    } else if (msg.isGet()) {
      #ifdef DEBUG
      #endif
      string id = msg.getId();
      if (file_exists(id)) {  // Request was made
        if (file_has_content(id)) { // Request was processed
          shared_ptr<Message> response;
          get_content_file(id, response);
        #ifdef DEBUG
          cout << "REPLY GET WITH: " << response->to_string() << endl;
        #endif
          zmqpp::message m = response->to_zmq_msg();

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
