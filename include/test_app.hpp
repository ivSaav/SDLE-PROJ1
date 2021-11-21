#pragma once
#include <zmqpp/zmqpp.hpp>
#include <string>
#include "../include/operation/operation.hpp"
using namespace std;

class TestApp {
public:
  TestApp(string filename);
  void run(zmqpp::context &context);
  void setupOps();

private:
  string filename;
  vector<Operation*> ops;
};
