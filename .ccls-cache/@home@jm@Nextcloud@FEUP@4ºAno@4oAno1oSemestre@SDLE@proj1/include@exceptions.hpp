#include <exception>
#include <iostream>
#include <sstream>

#include "message/message.hpp"

using namespace std;

class message_error : public runtime_error {
public:
  message_error(string msg, string t) : runtime_error(msg), topic(t) {}

  virtual const char *what() const throw() { return runtime_error::what(); }
  string getTopic() { return topic; }

private:
  string topic;
};

class AlreadySubscribed : public message_error {
public:
  AlreadySubscribed(string t) : message_error("Already subscribed", t) {}
};

class NotSubscribed : public message_error {
public:
  NotSubscribed(string t) : message_error("Not subscribed", t) {}
};

class InvalidContent : public message_error {
public:
  InvalidContent(string t)
      : message_error("Invalid content: Content can't be empty", t) {}
};

class InvalidMessage : public message_error {
public:
  InvalidMessage(string t, msg_type type)
      : message_error("Invalid Message", t), type(type) {}

  msg_type getType() { return type; }

private:
  msg_type type;
};
