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

typedef enum { PUT_OP = 0, GET_OP = 1, SUB_OP = 2, UNSUB_OP = 3, SLEEP_OP = 4 } operation_type;

/* Operation CLASS */

class Operation {

public:
  Operation(){};
  virtual ~Operation(){};

  Operation(operation_type type) : type(type) {}

  operation_type get_type() { return this->type; }
  virtual void execute(Node &node) = 0;

protected:
  operation_type type;
};


/* SUBCLASSES */

class GetOperation : public Operation {
public:
  GetOperation(string topic_name) : Operation(GET_OP), topic_name(topic_name), msg(string("")) {
  }
  void execute(Node& node) {
    node.get(topic_name,msg);
    cout << "GOT: " << msg;
  }

private:
string topic_name;
string msg;

};

class PutOperation : public Operation {
public:
  PutOperation(string topic_name, int n_times, string &msg) : Operation(PUT_OP), topic_name(topic_name), n_times(n_times), msg(msg) {}
  void execute(Node& node) {
    cout << "msg to send: " << msg << endl;
    for(int i = 0; i < n_times; i++){
      string to_send = msg + " " + to_string(i);
      node.put(topic_name,to_send);
    }
  }

private:
  string topic_name;
  string msg;
  int n_times;
};

class SubOperation : public Operation {
public:
  SubOperation(string topic_name) : Operation(SUB_OP), topic_name(topic_name) {}
  void execute(Node& node) {node.subscribe(topic_name);}

private:
  string topic_name;
};

class UnsubOperation : public Operation {
public:
  UnsubOperation(string topic_name) : Operation(UNSUB_OP), topic_name(topic_name) {}
  void execute(Node& node) {
    cout << "unsubscribing from " << topic_name << endl;
    node.unsubscribe(topic_name);
  }

private:
  string topic_name;
};

// Function argument equals milliseconds to sleep
class SleepOperation : public Operation {
public:
  SleepOperation(int time) : Operation(SLEEP_OP), time(time) {}
  void execute(Node& node) {
    cout << "started sleeping" << endl;
    usleep(this->time*1000);
    cout << "finished sleeping" << endl;
  }

private:
  int time;
};
