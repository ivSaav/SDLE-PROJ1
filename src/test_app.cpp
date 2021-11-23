#include <cstdlib>
#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <bits/stdc++.h>
#include <fstream>

#include "../include/test_app.hpp"

using namespace std;

TestApp::TestApp(string filename)
{
    this->filename = CONFIG_DIR + filename;
}

void TestApp::run(zmqpp::context &context, string node_id)
{
    cout << "ID: " << node_id << " > Started execution" << endl;
    Node peer(context, node_id);
    for (Operation *op : this->ops)
    {
        op->execute(peer);
        delete (op);
    }
    cout << "ID: " << node_id << " > Finished execution" << endl;

    return;
}

void configUsage(string filename,int line){
    cout << "Config file " << filename << "has an error on line " << to_string(line) <<  endl;
}

operation_type stringToEnum(string operation_type)
{
    if (operation_type == "PUT")
        return operation_type::PUT_OP;
    else if (operation_type == "GET")
        return operation_type::GET_OP;
    else if (operation_type == "SUB")
        return operation_type::SUB_OP;
    else if (operation_type == "UNSUB")
        return operation_type::UNSUB_OP;
    else
        return operation_type::SLEEP_OP;
}

int TestApp::setupOps()
{
    ifstream file(this->filename);
    string line;

    if (!file.is_open())
    {
        cout << "ERROR: Failed to read file! Exiting." << endl;
        return -1;
    }

    cout << "FILENAME: " << filename << endl;
    int counter = 1;
    while (getline(file, line))
    {
        if (line.size() != 0)
        {
            stringstream aux(line);
            string word, split_line[4];
            operation_type type;

            int i = 0;
            while (i < 4 && aux.good())
            {
                aux >> split_line[i];
                if (i == 0)
                {
                    type = stringToEnum(split_line[i]);
                }
                else if (i == 2)
                {
                    string msg;
                    getline(aux, msg);
                    split_line[3] = msg;
                }
                i++;
            }

            switch (type)
            {
            case GET_OP:
            {
                if(split_line[1] == "" || split_line[2] == ""){
                    configUsage(this->filename,counter);
                    return 1;
                }
                string n_times = split_line[2];
                this->ops.push_back(new GetOperation(split_line[1], stoi(n_times)));
                break;
            }
            case SUB_OP:
            {
                if(split_line[1] == ""){
                    configUsage(this->filename,counter);
                    return 1;
                }
                this->ops.push_back(new SubOperation(split_line[1]));
                break;
            }
            case UNSUB_OP:
            {
                if(split_line[1] == ""){
                    configUsage(this->filename,counter);
                    return 1;
                }
                this->ops.push_back(new UnsubOperation(split_line[1]));
                break;
            }
            case SLEEP_OP:
            {
                if(split_line[1] == ""){
                    configUsage(this->filename,counter);
                    return 1;
                }
                string time = split_line[1];
                this->ops.push_back(new SleepOperation(stoi(time)));
                break;
            }
            case PUT_OP:
            {
                if(split_line[1] == "" || split_line[2] == "" || split_line[3] == ""){
                    configUsage(this->filename,counter);
                    return 1;
                }
                cout << "content: " << split_line[2] << endl;
                string n_times = split_line[2];
                this->ops.push_back(new PutOperation(split_line[1], stoi(n_times), split_line[3]));
                break;
            }
            }
        }
        counter++;
    }

    file.close();
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "./testapp filename node_id\n";
    }

    zmqpp::context context;

    cout << "Running Testapp" << endl;
    string filename = argv[1];
    TestApp testapp(filename);
    if (testapp.setupOps() == 0)
    {
        testapp.run(context, argv[2]);
        fflush(stdout);
        context.terminate();
    }
    return 0;
}
