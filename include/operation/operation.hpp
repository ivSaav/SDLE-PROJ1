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
  Operation() {}

  Operation(operation_type type) : type(type) {}

  operation_type get_type() { return this->type; }
  virtual void execute(Node node);

protected:
  operation_type type;
};

/* SUBCLASSES */

class GetOperation : public Operation {
public:
  GetOperation() : Operation(GET_OP) {}
  void execute(Node node, string topic_name, string &msg) {
    node.get(topic_name, msg);
  }

private:
};

class PutOperation : public Operation {
public:
  PutOperation() : Operation(PUT_OP) {}
  void execute(Node node, string topic_name, string msg) {
    node.put(topic_name, msg);
  }

private:
};

class SubOperation : public Operation {
public:
  SubOperation() : Operation(SUB_OP) {}
  void execute(Node node, string topic_name) { node.subscribe(topic_name); }

private:
};

class UnsubOperation : public Operation {
public:
  UnsubOperation() : Operation(UNSUB_OP) {}
  void execute(Node node, string topic_name) { node.unsubscribe(topic_name); }

private:
};

// Function argument equals milliseconds to sleep
class SleepOperation : public Operation {
public:
  SleepOperation() : Operation(SLEEP_OP) {}
  void execute(Node node, int time) { usleep(time * 1000); }

private:
};
