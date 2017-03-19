//
//  page_manager.hpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef page_manager_hpp
#define page_manager_hpp

#include "common.h"
#include <string>
#include <stdio.h>
#include "fcntl.h"
#include "unistd.h"
#include "sys/mman.h"
#include <vector>
#include <list>
#include <unordered_map>
#include "catalog.hpp"

//#include "sys/_types/_iovec_t.h"


struct CacheNode {
    void * data;
    int page_id;
    int reference_count;
    CacheNode(void* _data, int _page_id): data(_data), page_id(_page_id), reference_count(0){}
};

using namespace std;

class CatalogManager;

class PageManager{
private:
    int fd;
    string filename;
    int cache_size;
    list<CacheNode> page_buffer;
    unordered_map<int, list<CacheNode>::iterator> page_buffer_map;
    int version;
    void* readPageFromDisk(int pageid);
    void move_to_rear(const list<CacheNode>::iterator & list_iter);
    
    
    
public:
    CatalogManager * catalog_manager;
    void truncate(int page_id);
    PageManager(string filename, int cache_size);
    int openFile();
    bool closeFile();
    void* readPage(int pageid);
    void release(int page_id);
    bool writePageToDisk(void* data, int pageid);
};

#endif /* page_manager_hpp */
