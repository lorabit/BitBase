//
//  hash_trie_manager.hpp
//  BitBase
//
//  Created by lorabit on 18/03/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef hash_trie_manager_hpp
#define hash_trie_manager_hpp

#include "page_manager.hpp"

#include <stdio.h>


struct HashTrieNode{
    int value;
    int version;
    TrieNodePosition children[TRIENODE_LENGTH];
};

struct HashTrieBlock{
    HashTrieNode nodes[TRIEBLOCK_LENGTH];
    char padding[PAGE_SIZE - TRIEBLOCK_LENGTH*sizeof(HashTrieNode)];
};


class HashTrieManager{
private:
    PageManager* page_manager;
    HashTrieNode* root();
    HashTrieNode* find_node(TrieNodePosition pos);
    HashTrieNode* find_node(string key, TrieNodePosition & pos);
    TrieNodePosition request_position();
    
public:
    HashTrieManager(PageManager* pageManager);
    versioned_value get_value(string key);
    bool update_node(string key, int value, int version);
    void update_trie_node(TrieNodePosition& c_pos, char key, TrieNodePosition& pos);
    void update_trie_node(TrieNodePosition& c_pos, int value, int version);
    

    
};


#endif /* hash_trie_manager_hpp */
