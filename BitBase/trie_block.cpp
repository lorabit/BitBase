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
    bool find = false;
    for(int i = 0; i < key.length(); i++){
        find = false;
        int j = 0;
        while(j<TRIENODE_LENGTH || node->next.index+node->next.page_id>0){
            if(j == TRIENODE_LENGTH){
                j = 0;
                pos = node->next;
                node = find_node(pos);
            }
            if(node->children[j] == '\0') return NULL;
            if(node->children[j] == key[i]){
                pos = node->children_pos[j];
                node = find_node(pos);
                find = true;
                break;
            }
            j++;
        }
        if(!find) return NULL;
    }
    return node;
}



void TrieManager::update_trie_node(TrieNodePosition& c_pos, char key, TrieNodePosition& pos){
    TrieBlock* block = (TrieBlock*)page_manager->readPage(c_pos.page_id);
    for(int i = 0; i < TRIENODE_LENGTH; i++)
        if(block->nodes[c_pos.index].children[i] == key){
            block->nodes[c_pos.index].children_pos[i] = pos;
            break;
        }
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
        int j = 0;
        bool created = false;
        while(j<TRIENODE_LENGTH || node->next.index+node->next.page_id>0){
            if(j == TRIENODE_LENGTH){
                j = 0;
                node_pos = node->next;
                node = find_node(node_pos);
            }
            if(node->children[j] == '\0'){
                next_pos = request_position();
                next = find_node(next_pos);
                node->children[j] = key[i];
                node->children_pos[j] = next_pos;
                created = true;
                break;
            }
            if(node->children[j] == key[i]){
                next_pos = node->children_pos[j];
                next = find_node(next_pos);
                created = true;
                break;
            }
            j++;
        }
        if (created == false) {
            node->next = request_position();
            node = find_node(node->next);
            next_pos = request_position();
            next = find_node(next_pos);
            node->children[0] = key[i];
            node->children_pos[0] = next_pos;
        }
        node_pos = next_pos;
        node = next;
    }
    update_trie_node(node_pos, value, version>0?version:(node->version+1));
    return true;
}
