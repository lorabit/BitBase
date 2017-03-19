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


versioned_value TrieManager::get_value(string key){
    TrieNodePosition pos;
    TrieNode* node = find_node(key,pos);
    if(node == NULL) return versioned_value(-1, -1);
    versioned_value ret = versioned_value(node->version, node->value);
    page_manager->release(pos.page_id);
    return ret;
}

TrieNode* TrieManager::find_node(string key, TrieNodePosition & pos){
    TrieNode* node = root();
    TrieNodePosition next_pos;
    bool find = false;
    for(int i = 0; i < key.length(); i++){
        find = false;
        int j = 0;
        while(j<TRIENODE_LENGTH || node->next.index+node->next.page_id>0){
            if(j == TRIENODE_LENGTH){
                j = 0;
                next_pos = node->next;
                page_manager->release(pos.page_id);
                pos = next_pos;
                node = find_node(pos);
            }
            if(node->children[j] == '\0') {
                page_manager->release(pos.page_id);
                return NULL;
            }
            if(node->children[j] == key[i]){
                next_pos = node->children_pos[j];
                page_manager->release(pos.page_id);
                pos = next_pos;
                node = find_node(pos);
                find = true;
                break;
            }
            j++;
        }
        if(!find) {
            page_manager->release(pos.page_id);
            return NULL;
        }
    }
    return node;
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
//    printf("\n");
//    if(key == "178"){
//        printf("\n");
//    }
    for(int i = 0; i < key.length(); i++){
        int j = 0;
        bool created = false;
        while(j<TRIENODE_LENGTH || node->next.index+node->next.page_id>0){
            if(j == TRIENODE_LENGTH){
                j = 0;
                next_pos = node->next;
                page_manager->release(node_pos.page_id);
                node_pos = next_pos;
                node = find_node(node_pos);
            }
//            printf("%c",node->children[j]);
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
            next_pos = request_position();
            node->next = next_pos;
            next = find_node(next_pos);
            next->value = node->value;
            next->version = node->version;
//            next->next.index = 0;
//            next->next.page_id = 0;
            node = next;
            page_manager->release(node_pos.page_id);
            node_pos = next_pos;
            next_pos = request_position();
            next = find_node(next_pos);
            node->children[0] = key[i];
            node->children_pos[0] = next_pos;
            if(key[i] == '\0' || (node->next.page_id == node_pos.page_id && node->next.index == node_pos.index)){
                perror("err");
            }
        }
//        printf("\n");
        page_manager->release(node_pos.page_id);
        node_pos = next_pos;
        node = next;
    }
    node->version =version>0?version:(node->version+1);
    node->value = value;
    page_manager->release(node_pos.page_id);
    return true;
}
