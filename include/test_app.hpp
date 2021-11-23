#pragma once
#include <zmqpp/zmqpp.hpp>
#include <string>
#include "../include/operation/operation.hpp"
using namespace std;

#define CONFIG_DIR "../../config/"

class TestApp {
public:
  TestApp(string filename);
  void run(zmqpp::context &context, string node_id);
  int setupOps();

private:
  string filename;
  vector<Operation*> ops;
};
