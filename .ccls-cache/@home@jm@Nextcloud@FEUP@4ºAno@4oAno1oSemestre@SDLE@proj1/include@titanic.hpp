#ifndef TITANIC_H
#define TITANIC_H

#include <functional>
#include <queue>
#include <string>
#include <zmqpp/zmqpp.hpp>

#define REQ_PATH "./requests/"

#include "common.hpp"
#include "message/titanic_messages.hpp"

using namespace std;

bool file_exists(string file_name);
bool file_has_conent(string file_name);
bool create_file(string file_name);
bool write_content_file(string file_name, string &content);
bool get_content_file(string file_name, string &content);

//💑 🚢 🌊 💥 💀
class Titanic {
private:
public:
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
