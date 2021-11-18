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
  Operation(){}

  Operation(operation_type type)
      : type(type){}

  operation_type get_type() { return this->type; }
  virtual void execute(string topic_name, string &msg);
  virtual void execute(string topic_name);
  virtual void execute(int time);


protected:
  operation_type type;

};



/* SUBCLASSES */

class GetOperation : public Operation {
public:
  GetOperation(zmqpp::context context, string node_id) : Operation(GET) {
    this->node = new Node(context, node_id);
  }
  void execute(string topic_name, string &msg) {this->node->get(topic_name,msg);}
private:
  Node *node;
  
};

class PutOperation : public Operation {
public:
  PutOperation(zmqpp::context context, string node_id) : Operation(PUT) {
    this->node = new Node(context, node_id);
  }
  void execute(string topic_name, string &msg) {this->node->put(topic_name,msg);}
private:
  Node *node;
  
};

class SubOperation : public Operation {
public:
  SubOperation(zmqpp::context context, string node_id) : Operation(SUB) {
    this->node = new Node(context, node_id);
  }
  void execute(string topic_name) {this->node->subscribe(topic_name);}
private:
  Node *node;
  
};

class UnsubOperation : public Operation {
public:
  UnsubOperation(zmqpp::context context, string node_id) : Operation(UNSUB) {
    this->node = new Node(context, node_id);
  }
  void execute(string topic_name) {this->node->unsubscribe(topic_name);}
private:
  Node *node;
  
};

// Function argument equals milliseconds to sleepc
class SleepOperation : public Operation {
public:
  SleepOperation() : Operation(SLEEP) {
  }
  void execute(int time) {usleep(time*1000);}

private:
};
