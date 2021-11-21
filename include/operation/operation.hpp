#pragma once

#include <iostream>
#include <unistd.h>
#include <zmqpp/zmqpp.hpp>

#include "../node.hpp"

using namespace std;

// PUT MSG
// SUB MSG
// GET MSG
// UNSUB MSG

typedef enum { PUT = 0, GET = 1, SUB = 2, UNSUB = 3, SLEEP = 4 } operation_type;

/* Operation CLASS */

class Operation {

public:
  Operation(){};

  Operation(operation_type type)
      : type(type){}

  operation_type get_type() { return this->type; }
  virtual void execute(Node* node) = 0;


protected:
  operation_type type;

};


/* SUBCLASSES */

class GetOperation : public Operation {
public:
  GetOperation(string topic_name, string &msg) : Operation(GET), topic_name(topic_name), msg(msg) {}
  void execute(Node* node) {
    node->get(topic_name,msg);
    cout << "GOT: " << msg;
  }

private:
string topic_name;
string& msg;

};

class PutOperation : public Operation {
public:
  PutOperation(string topic_name, string &msg) : Operation(PUT), topic_name(topic_name), msg(msg) {}
  void execute(Node* node) {node->put(topic_name,msg);}

private:
  string topic_name;
  string& msg;
};

class SubOperation : public Operation {
public:
  SubOperation(string topic_name) : Operation(SUB), topic_name(topic_name) {}
  void execute(Node* node) {node->subscribe(topic_name);}

private:
  string topic_name;
};

class UnsubOperation : public Operation {
public:
  UnsubOperation(string topic_name) : Operation(UNSUB), topic_name(topic_name) {}
  void execute(Node* node) {node->unsubscribe(topic_name);}

private:
  string topic_name;
};

// Function argument equals milliseconds to sleep
class SleepOperation : public Operation {
public:
  SleepOperation(int time) : Operation(SLEEP), time(time) {}
  void execute(Node* node) {usleep(this->time*1000);}

private:
  int time;
};
