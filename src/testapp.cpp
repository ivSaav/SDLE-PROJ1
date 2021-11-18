#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <fstream>

#include "../include/testapp.hpp"


using namespace std;


TestApp::TestApp(string filename){
    this->filename = filename;
}

void TestApp::run() {
  
}

void TestApp::setupOps(){
    ifstream file(this->filename);
    string line;

    while(getline(file,line)){
        cout << line;
    }

    file.close();

}

int main(int argc, char *argv[]) {
    if(argc < 2){
        cout << "./testapp filename";
    }
    cout << "Running testapp" << endl;
    string filename = argv[1];
    TestApp testapp(filename);
    testapp.setupOps();
    testapp.run();
    return 0;
}
