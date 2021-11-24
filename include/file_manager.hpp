#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <fstream>
#include <filesystem>

#include "common.hpp"
#include "message/message.hpp"
#include "message/answer_msg.hpp"
#include "message/put_msg.hpp"

inline void create_dir() {
  std::filesystem::create_directories(REQUESTS_PATH);
}

inline void delete_file(string file_name) {
  string p = REQUESTS_PATH + file_name;
  remove(p.c_str());
}

inline bool file_exists(string file_name) {
  create_dir();
  ifstream file(REQUESTS_PATH + file_name);
  return file.is_open();
}

inline bool file_has_content(string file_name) {
  create_dir();
  ifstream file(REQUESTS_PATH + file_name);
  return file.peek() != ifstream::traits_type::eof();
}

inline void create_file(string file_name) {
  create_dir();
  ofstream file(REQUESTS_PATH + file_name);
}

inline bool write_content_file(string file_name, Message *message) {
  create_dir();
  ofstream file(REQUESTS_PATH + file_name);
  cereal::XMLOutputArchive oarchive(file);

  std::shared_ptr<Message> m;
  if (message->get_type() == msg_type::GET) {
    GetMessage tmp = *(GetMessage*) message;
    m = shared_ptr<Message>(new GetMessage(tmp));
  } else if (message->get_type() == msg_type::PUT) {
    PutMessage tmp = *(PutMessage*) message;
    m = shared_ptr<Message>(new PutMessage(tmp));
  } else if (message->get_type() == msg_type::SUB) {
    SubMessage tmp = *(SubMessage*) message;
    m = shared_ptr<Message>(new SubMessage(tmp));
  } else if (message->get_type() == msg_type::UNSUB) {
    UnsubMessage tmp = *(UnsubMessage*) message;
    m = shared_ptr<Message>(new UnsubMessage(tmp));
  } else if (message->get_type() == msg_type::ANSWER) {
    AnswerMessage tmp = *(AnswerMessage*) message;
    m = shared_ptr<Message>(new AnswerMessage(tmp));
  } else if (message->get_type() == msg_type::OK) {
    OkMessage tmp = *(OkMessage*) message;
    m = shared_ptr<Message>(new OkMessage(tmp));
  } else if (message->get_type() == msg_type::KO) {
    KoMessage tmp = *(KoMessage*) message;
    m = shared_ptr<Message>(new KoMessage(tmp));
  }

  oarchive(m);
  return file.good();
}

inline bool get_content_file(string file_name, shared_ptr<Message> &msg) {
  create_dir();
  ifstream file(REQUESTS_PATH + file_name);
  cereal::XMLInputArchive iarchive(file);

  iarchive(msg);
  return file.good();
}

#endif /* ifndef FILE_MANAGER_H */
