#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std;

// Code from stack answer https://stackoverflow.com/a/16286297
vector<string> splitString(const string& str,const string& sep) {
    char* cstr = const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current = strtok(cstr, sep.c_str());
    while(current != nullptr){
        arr.emplace_back(current);
        current = strtok(nullptr, sep.c_str());
    }
    return arr;
}

int main() {
    string commands;
    istringstream Commands("fl200_fr200_bl200_br200_ln1500\ncl\nfl-100_fr-100_bl-100_br-100_ln3000\ncl");

    while (getline(Commands, commands)) {
        vector<string> commandAr = splitString(commands, "_");
        map<string, int> dists {};

        for (const string& command : commandAr) {
            int dist = (!command.substr(2).empty()) ? stoi(command.substr(2)) : 0;
            string commandSt = command.substr(0, 2);
            
            dists.insert({commandSt, dist});
        }

        for (const auto& [command, speed] : dists)
            if (command != "ln")
                cout << command << " : " << speed << endl;

        if (dists.find("ln") != dists.end())  {
            cout << "Delay: " << dists.at("ln") << endl;

            this_thread::sleep_for(chrono::milliseconds(dists.at("ln")));

            for (const auto& [command, speed] : dists)
                if (command != "ln")
                    cout << command << " : " << 0 << endl;
        } else
            cout << "No delay" << endl;
        
        cout << "-----------" << endl;
    }
}