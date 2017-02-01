//
//  data_interface.cpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "data_interface.hpp"
#include <vector>
#include <unordered_map>

using namespace std;

unordered_map<string, int> memmap;

void data_set(string key, int value){
    memmap[key] = value;
}


versioned_value data_get(string key){
    if(memmap.find(key) == memmap.end())
        return versioned_value(-1,-1);
    return versioned_value(0, memmap[key]);
}


bool data_cas(string key, versioned_value value){
    memmap[key] = value.value;
    return true;
}


string process_request(string request){
    vector<string> cmd;
    int last = 0;
    for(int i = 0; i < request.size(); i++)
        if(request[i] == '|'){
            cmd.push_back(request.substr(last, i - last));
            last = i+1;
        }
    cmd.push_back(request.substr(last, request.size() - last));
    if(cmd[0] == "get"){
        versioned_value value = data_get(cmd[1]);
        return to_string(value.value)+"|"+to_string(value.version);
    }
    if(cmd[0] == "set"){
        data_set(cmd[1], stoi(cmd[2]));
        return "OK";
    }
    if(cmd[0] == "cas"){
        if(data_cas(cmd[1], versioned_value(stoi(cmd[3]), stoi(cmd[2])))){
            return "OK";
        }
        return "F";
    }
    return "F";
}
