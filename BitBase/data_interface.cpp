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
#include "hash_trie_manager.hpp"

using namespace std;

unordered_map<string, int> memmap;

std::mutex memmap_mutex;

PageManager shared_page_manager(DBFILE, 65536);
TrieManager trie_manager(&shared_page_manager);

int init_database(){
    return shared_page_manager.openFile();
}

void clean_database(){
    shared_page_manager.truncate(0);
}

void inspect(){
//    TrieNode* node = trie_manager.find_node(TrieNodePosition(4, 0));
//    printf("%d - %d %d\n",node->value,node->next.page_id, node->next.index);
//    for(int i = 0; i < TRIENODE_LENGTH; i++)
//        printf("%c-%d %d ; ",node->children[i], node->children_pos[i].page_id, node->children_pos[i].index);
//    shared_page_manager.release(4);
}

void data_set(string key, int value){
    trie_manager.update_node(key, value, -1);
}


versioned_value data_get(string key){
    return trie_manager.get_value(key);
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
