//
//  trie_block.cpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "trie_block.hpp"


TrieManager::TrieManager(PageManager * _page_manager){
    empty_block = new TrieBlock();
    page_manager = _page_manager;
}

TrieNode* TrieManager::root(){
    return find_node(TrieNodePosition(0,0));
}

TrieNode* TrieManager::find_node(TrieNodePosition pos){
    if(pos.page_id <= -1 || pos.index == -1) return NULL;
    TrieBlock* block = (TrieBlock*)page_manager->readPage(pos.page_id);
    return &block->nodes[pos.index];
}

TrieNode* TrieManager::find_node(string key){
    TrieNode* node = root();
    TrieNodePosition pos;
//    bool find = false;
    for(int i = 0; i < key.length(); i++){
        pos = node->children_pos[key[i]];
        if(pos.page_id == 0 && pos.index == 0) return NULL;
        node = find_node(pos);
//        find = false;
//        for(int j = 0; j < TRIENODE_LENGTH; j++){
//            if(node->children[j] == '\0') return NULL;
//            if(node->children[j] == key[i]){
//                node = find_node(node->children_pos[j]);
//                find = true;
//                break;
//            }
//        }
//        if(!find) return NULL;
    }
    return node;
}



void TrieManager::update_trie_node(TrieNodePosition& c_pos, char key, TrieNodePosition& pos){
    TrieBlock* block = (TrieBlock*)page_manager->readPage(c_pos.page_id);
//    printf("%d %d\n",pos.page_id,block->nodes[c_pos.index].children_pos[key].page_id);
//    TrieBlock n;
//    memcpy(&n, block, sizeof(TrieBlock));
    block->nodes[c_pos.index].children_pos[key] = pos;
//    memcpy(block, &n, sizeof(TrieBlock));
//    page_manager->writePageToDisk(&n, c_pos.page_id);
}

void TrieManager::update_trie_node(TrieNodePosition& c_pos, int value, int version){
    TrieBlock* block = (TrieBlock*)page_manager->readPage(c_pos.page_id);
    block->nodes[c_pos.index].value = value;
    block->nodes[c_pos.index].version = version;
//    page_manager->writePageToDisk(block, c_pos.page_id);
}

TrieNodePosition TrieManager::request_position(){
    TrieNodePosition pos = page_manager->catalog_manager->requestTrieNodePosition();
    if(pos.index == 0 &&  pos.page_id%1024 == 0){
        page_manager->truncate(pos.page_id);
    }
    return pos;
}


bool TrieManager::update_node(string key, int value, int version){
    TrieNode* node = root();
    TrieNodePosition node_pos = TrieNodePosition(0, 0);
    TrieNode* next;
    TrieNodePosition next_pos = TrieNodePosition(0, 0);
    
    for(int i = 0; i < key.length(); i++){
        next_pos = node->children_pos[key[i]];
        if(next_pos.page_id == 0 && next_pos.index == 0){
            next_pos = request_position();
            node->children_pos[key[i]] = next_pos;
//            update_trie_node(node_pos, key[i], next_pos);
            next = find_node(next_pos);
        }else
            next = find_node(next_pos);
        node = next;
        node_pos = next_pos;
//        for(int j = 0; j < TRIENODE_LENGTH; j++){
//            if(node->children[j] == '\0') return NULL;
//            if(node->children[j] == key[i]){
//                node = find_node(node->children_pos[j]);
//                find = true;
//                break;
//            }
//        }
    }
    update_trie_node(node_pos, value, version>0?version:(node->version+1));
    return true;
}
