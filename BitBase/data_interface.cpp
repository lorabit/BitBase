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
#include "page_manager.hpp"
#include <cstdlib>
#include <mutex>

using namespace std;

unordered_map<string, int> memmap;

std::mutex memmap_mutex;

PageManager shared_page_manager("/Users/lorabit/bitbase.bin", 512);
TrieManager trie_manager(&shared_page_manager);

int init_database(){
    return shared_page_manager.openFile();
}

void data_set(string key, int value){
    lock_guard<std::mutex> guard(memmap_mutex);
    trie_manager.update_node(key, value, -1);
//    struct timespec sleeptime;
//    sleeptime.tv_sec = 0;
//    sleeptime.tv_nsec = 10000000*(rand()%5);
//    nanosleep(&sleeptime, NULL);
//    void * data = shared_page_manager.readPage(0);
//    for(int i=0;i<100;i++){
//        shared_page_manager.writePageToDisk(data, 0);
//    }
}


versioned_value data_get(string key){
    TrieNode* node = trie_manager.find_node(key);
    if(node == NULL) return versioned_value(-1, -1);
    return versioned_value(node->version, node->value);
    //    if(memmap.find(key) == memmap.end())
//        return versioned_value(-1,-1);
//    return versioned_value(0, memmap[key]);
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
