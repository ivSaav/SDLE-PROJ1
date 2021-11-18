#pragma once
#include <zmqpp/zmqpp.hpp>
#include <string>
#include "../include/operation/operation.hpp"
using namespace std;

class TestApp {
public:
  TestApp(string filename);
  void run();
  void setupOps();

private:
  string filename;
  vector<Operation> ops;
  zmqpp::context context;
};
