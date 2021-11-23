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
      cout << "DEL" << endl;
      cout << msg.to_string() << endl;
    } else if (msg.isReq()) {
      cout << "REQ" << endl;
      cout << "SENDING TO WORKER " << msg.to_string() << endl;

      hash<string> hasher;
      int hash_id = hasher(msg.to_string());
      requests.push(msg);
      zmqpp::message response(hash_id, zmqpp::signal::ok);
      client.send(response);
    } else if (msg.isGet()) {
      cout << "GET" << endl;
      cout << msg.to_string() << endl;
    } else
      cout << "INVALID CLIENT REQUEST" << endl;
  }
};
#endif /* ifndef TITANIC_H */
