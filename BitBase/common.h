//
//  common.h
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef common_h
#define common_h

#define CACHE_SIZE  6553600
#define PAGE_SIZE   4096

#define DBFILE  "/Users/lorabit/bitbase.bin"

//#define TRIEBLOCK_LENGTH    32
//#define TRIENODE_LENGTH     12
//
//#define TRIEBLOCK_LENGTH    64
//#define TRIENODE_LENGTH     5

#define TRIENODE_LENGTH     16
#define TRIEBLOCK_LENGTH    30


//#define TRIEBLOCK_LENGTH    113
//#define TRIENODE_LENGTH     2

//#define TRIEBLOCK_LENGTH    146
//#define TRIENODE_LENGTH     1

//#define TRIEBLOCK_LENGTH    PAGE_SIZE/sizeof(HashTrieNode)

struct TrieNodePosition{
    int page_id;
    int index;
    TrieNodePosition(int _page_id, int _index):page_id(_page_id), index(_index){};
    TrieNodePosition():page_id(0), index(0){};
};



#endif /* common_h */
