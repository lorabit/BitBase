//
//  hash_trie_manager.cpp
//  BitBase
//
//  Created by lorabit on 18/03/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "hash_trie_manager.hpp"
#include <iostream>

#include <functional>


HashTrieManager::HashTrieManager(PageManager * _page_manager){
    page_manager = _page_manager;
}

HashTrieNode* HashTrieManager::root(){
    return find_node(TrieNodePosition(0,0));
}

HashTrieNode* HashTrieManager::find_node(TrieNodePosition pos){
    if(pos.page_id <= -1 || pos.index == -1) return NULL;
    HashTrieBlock* block = (HashTrieBlock*)page_manager->readPage(pos.page_id);
    return &block->nodes[pos.index];
}

TrieNodePosition HashTrieManager::request_position(){
    TrieNodePosition pos = page_manager->catalog_manager->requestTrieNodePosition();
    if(pos.index == 0 &&  pos.page_id%1024 == 0){
        page_manager->truncate(pos.page_id);
    }
    return pos;
}


versioned_value HashTrieManager::get_value(string key){
    TrieNodePosition pos;
    HashTrieNode* node = find_node(key,pos);
    if(node == NULL) return versioned_value(-1, -1);
    versioned_value ret = versioned_value(node->version, node->value);
    page_manager->release(pos.page_id);
    return ret;
}

string hash_str(string str){
    std::hash<std::string> str_hash;
    size_t hash_value = str_hash(str);
    string ret = "";
    while(hash_value > 0){
        ret = " " + ret;
        ret[0] = hash_value % 256;
        hash_value /= 256;
    }
    return ret;
}

HashTrieNode* HashTrieManager::find_node(string key, TrieNodePosition & pos){
    key = hash_str(key);
    HashTrieNode* node = root();
    TrieNodePosition next_pos;
    
    //string to binary digit
    int strlen=8, evelen=4;
    int curlen=0;
    int cur_num=0;
    
    int d= (1 << (evelen)) -1;
    
    int num = strlen*key.length()/evelen;
    if(strlen*key.length()%evelen != 0) num++;
    int i=key.length()-1;
    
    for(int j=0; j<num; j++){
        if(i>=0 && curlen<evelen){
            cur_num = cur_num + (int(key[i]) << curlen);
            curlen += 8;
            i--;
        }
        
        int bin_key = (cur_num & d);
        cur_num = (cur_num >> evelen);
        curlen -= evelen;
        
//        cout  << "bin key : " << bin_key << endl;
        
        if(node->children[bin_key].index +node->children[bin_key].page_id == 0) {
            page_manager->release(pos.page_id);
            return NULL;
        }
        else
        {
            next_pos = node->children[bin_key];
            page_manager->release(pos.page_id);
            pos = next_pos;
            node = find_node(pos);
        }
    }
    
    return node;
}

bool HashTrieManager::update_node(string key, int value, int version){
    key = hash_str(key);
    HashTrieNode* node = root();
    TrieNodePosition node_pos = TrieNodePosition(0, 0);
    TrieNodePosition next_pos;
    
    //string to binary digit
    int strlen=8, evelen=4;
    int curlen=0;
    int cur_num=0;
    
    int d= (1 << (evelen)) -1;
    
    int num = strlen*key.length()/evelen;
    if(strlen*key.length()%evelen != 0) num++;
    int i=key.length()-1;
    
    for(int j=0; j<num; j++){
        if(i>=0 && curlen<evelen){
            cur_num = cur_num + (int(key[i]) << curlen);
            curlen += 8;
            i--;
        }
        
        int bin_key = (cur_num & d);
        cur_num = (cur_num >> evelen);
        curlen -= evelen;
        
//        printf("%d\n", bin_key);
        if(node->children[bin_key].index +node->children[bin_key].page_id == 0)
        {
            next_pos = request_position();
            node -> children[bin_key] = next_pos;
//            page_manager->msync(node_pos.page_id);
        }else{
            next_pos = node->children[bin_key];
        }
        //printf("%d, %d\n", next_pos.page_id, next_pos.index);
        
        page_manager->release(node_pos.page_id);
        node_pos = next_pos;
        node = find_node(node_pos);
    }
    
    node->version =version>0?version:(node->version+1);
    node->value = value;
//    page_manager->msync(node_pos.page_id);
    page_manager->release(node_pos.page_id);
    
    return true;
}
