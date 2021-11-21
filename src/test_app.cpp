#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <bits/stdc++.h>
#include <fstream>

#include "../include/test_app.hpp"


using namespace std;


TestApp::TestApp(string filename){
    this->filename = "../../config/"+filename;
}

void TestApp::run() {
  
}

operation_type stringToEnum(string operation_type){
    if(operation_type == "PUT") return operation_type::PUT;
    else if(operation_type == "GET") return operation_type::GET;
    else if(operation_type == "SUB") return operation_type::SUB;
    else if(operation_type == "UNSUB") return operation_type::UNSUB;
    else return operation_type::SLEEP;
}

void TestApp::setupOps(){
    ifstream file(this->filename);
    string line;
    
    // zmqpp::context context;
    // Node peer = Node(context, "0");

    while(getline(file,line)){
        stringstream aux(line);
        string word;
        string split_line[4];
        operation_type type;
        string filler;
        int i = 0;
        while(i < 4 && aux.good()){
            aux >> split_line[i];
            if(i == 0){
                type = stringToEnum(split_line[i]);
            }
            i++;
        }
        switch(type){
            case PUT:
                this->ops.push_back(new PutOperation(split_line[1],split_line[2]));
                break;
            case GET:
                this->ops.push_back(new GetOperation(split_line[1],filler));
                break;
            case SUB:
                this->ops.push_back(new SubOperation(split_line[1]));
                break;
            case UNSUB:
                this->ops.push_back(new UnsubOperation(split_line[1]));
                break;
            case SLEEP:
                string time = split_line[1];
                this->ops.push_back(new SleepOperation(stoi(time)));
                break;
        }
    }

    for(Operation* v: this->ops){
        cout << v->get_type() << endl;
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
