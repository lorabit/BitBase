//
//  trie_block.hpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef trie_block_hpp
#define trie_block_hpp

#include <stdio.h>
#include <string>

using namespace std;


#define TRIEBLOCK_LENGTH    1
#define TRIENODE_LENGTH     256

struct TrieNodePosition{
    int page_id;
    int index;
    TrieNodePosition(int _page_id, int _index):page_id(_page_id), index(_index){};
    
    TrieNodePosition():page_id(0), index(0){};
};

struct TrieNode {
    int value;
    int version;
//    char children[TRIENODE_LENGTH];
    TrieNodePosition children_pos[TRIENODE_LENGTH];
    TrieNode(){
        for(int i = 0; i < TRIENODE_LENGTH; i++){
            children_pos[i].index=0;
            children_pos[i].page_id=0;
        }
    }
};

struct TrieBlock {
    TrieNode nodes[TRIEBLOCK_LENGTH];
    char _[4096 - 2056];
    TrieBlock(){
        for(int i = 0; i < TRIEBLOCK_LENGTH; i++)
            nodes[i] = TrieNode();
    }
};

#include "page_manager.hpp"

#define PAGE_SIZE   4096

class PageManager;

class TrieManager{
private:
    PageManager* page_manager;
    TrieNodePosition request_position();
public:
    TrieManager(PageManager* pageManager);
    TrieNode* root();
    TrieNode* find_node(TrieNodePosition pos);
    TrieNode* find_node(string key);
    bool update_node(string key, int value, int version);
    void update_trie_node(TrieNodePosition& c_pos, char key, TrieNodePosition& pos);
    void update_trie_node(TrieNodePosition& c_pos, int value, int version);
    TrieBlock* empty_block;
};


#endif /* trie_block_hpp */
