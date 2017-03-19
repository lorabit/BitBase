//
//  hash_trie_manager.cpp
//  BitBase
//
//  Created by lorabit on 18/03/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "hash_trie_manager.hpp"

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

HashTrieNode* HashTrieManager::find_node(string key, TrieNodePosition & pos){
    HashTrieNode* node = root();
    return node;
}

versioned_value HashTrieManager::get_value(string key){
    TrieNodePosition pos;
    HashTrieNode* node = find_node(key,pos);
    if(node == NULL) return versioned_value(-1, -1);
    versioned_value ret = versioned_value(node->version, node->value);
    page_manager->release(pos.page_id);
    return ret;
}

bool HashTrieManager::update_node(string key, int value, int version){
    return true;
}
