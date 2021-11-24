#ifndef TITANIC_H
#define TITANIC_H

#include <queue>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <fstream>
#include <filesystem>

#include "common.hpp"
#include "message/titanic_messages.hpp"
#include "file_manager.hpp" // For functions to save

using namespace std;

//💑 🚢 🌊 💥 💀
class Titanic {
private:
public:
  void test() {
    //create_file("test");
    //write_content_file("test", "asd");
    //string c;
    //cout << "GET: " << get_content_file("test", c) << ": " << c;
    SubMessage g("t1", "id1");

    write_content_file("test", &g);

    Message m;

    get_content_file("test", m);

    cout << m.to_string() << endl;

    fflush(stdout);
  }
  void handle(zmqpp::message &req, zmqpp::socket &client,
              queue<TitanicMessage> &requests) {
    TitanicMessage msg(req);

    if (msg.isDel()) {
      cout << msg.to_string() << endl;
      if (file_exists(to_string(msg.getId())))
        delete_file(to_string(msg.getId()));

      client.send(zmqpp::signal::ok);
    } else if (msg.isReq()) {
      cout << "REQ" << endl;
      cout << "SENDING TO WORKER " << msg.to_string() << endl;

      hash<string> hasher;
      int hash_id = hasher(msg.to_string());
      if (file_exists(to_string(hash_id))){ // Request already made
        client.send(zmqpp::signal::ko);
      } else {
        create_file(to_string(hash_id));
        requests.push(msg);
        zmqpp::message response(hash_id, zmqpp::signal::ok);
        client.send(response);
      }
    } else if (msg.isGet()) {
      cout << msg.to_string() << endl;

      string id = to_string(msg.getId());
      if (file_exists(id)) {  // Request was made
        if (file_has_content(id)) { // Request was processed
          Message response;
          get_content_file(id, response);
          zmqpp::message m = response.to_zmq_msg();
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
