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

#define TRIEBLOCK_LENGTH    32
#define TRIENODE_LENGTH     13

struct TrieNodePosition{
    int page_id;
    int index;
};

struct TrieNode {
    int value;
    int version;
    char children[TRIENODE_LENGTH];
    TrieNodePosition children_pos[TRIENODE_LENGTH];
};

struct TrieBlock {
    TrieNode nodes[TRIEBLOCK_LENGTH];
};


#endif /* trie_block_hpp */
