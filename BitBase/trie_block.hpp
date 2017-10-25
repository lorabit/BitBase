//
//  trie_block.hpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef trie_block_hpp
#define trie_block_hpp

#include "common.h"

#include <string>
#include "versioned_value.hpp"


using namespace std;


struct TrieNode {
    int value;
    int version;
    char children[TRIENODE_LENGTH];
    TrieNodePosition next;
    TrieNodePosition children_pos[TRIENODE_LENGTH];
    TrieNode(){
//        for(int i = 0; i < TRIENODE_LENGTH; i++){
//            children_pos[i].index=0;
//            children_pos[i].page_id=0;
//        }
    }
};

struct TrieBlock {
    TrieNode nodes[TRIEBLOCK_LENGTH];
//    char padding[PAGE_SIZE - TRIEBLOCK_LENGTH*sizeof(TrieNode)];
    TrieBlock(){
        for(int i = 0; i < TRIEBLOCK_LENGTH; i++)
            nodes[i] = TrieNode();
    }
};

#include "page_manager.hpp"

class PageManager;

class TrieManager{
private:
    PageManager* page_manager;
    TrieNode* root();
    TrieNode* find_node(TrieNodePosition pos);
    TrieNode* find_node(string key, TrieNodePosition & pos);
    TrieNodePosition request_position();
public:
    TrieManager(PageManager* pageManager);
    versioned_value get_value(string key);
    bool update_node(string key, int value, int version);
    void update_trie_node(TrieNodePosition& c_pos, char key, TrieNodePosition& pos);
    void update_trie_node(TrieNodePosition& c_pos, int value, int version);
    TrieBlock* empty_block;
};


#endif /* trie_block_hpp */
