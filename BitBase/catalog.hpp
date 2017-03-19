//
//  catalog.hpp
//  BitBase
//
//  Created by lorabit on 13/02/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef catalog_hpp
#define catalog_hpp

#include <stdio.h>
#include <string>
#include "trie_block.hpp"

using namespace std;

struct CatalogPage {
    int page_id;
    int index;
};

class CatalogManager{
private:
    int fd;
    string filename;
    
public:
    CatalogPage * page;
    CatalogManager(string filename);
    vector<TrieNodePosition> requestTrieNodePosition(int n);
    TrieNodePosition requestTrieNodePosition();
    
};


#endif /* catalog_hpp */
