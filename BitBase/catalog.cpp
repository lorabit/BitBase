//
//  catalog.cpp
//  BitBase
//
//  Created by lorabit on 13/02/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "catalog.hpp"
#include <mutex>
#include <stdlib.h>

CatalogManager::CatalogManager(string _filename){
    filename = _filename;
    fd = open(filename.c_str(), O_RDWR);
    if(fd==-1){
        perror("Error opening file");
        return;
    }
//    page = new CatalogPage();
    //CatalogPage* r_
    page = (CatalogPage*)mmap(NULL, sizeof(CatalogPage), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//    memcpy(page, r_page, sizeof(CatalogPage));
}

std::mutex request_mutex;
vector<TrieNodePosition> CatalogManager::requestTrieNodePosition(int n){
    vector<TrieNodePosition> ret(n);
    lock_guard<std::mutex> guard(request_mutex);
    int index = page->index;
    int page_id = page->page_id;
    for(int i = 0; i < n; i++){
        index ++;
        if(index == TRIEBLOCK_LENGTH){
            index = 0;
            page_id ++;
        }
        ret[i] = TrieNodePosition(page_id, index);
    }
    page->page_id = page_id;
    page->index = index;
//    pwrite(fd, page, sizeof(CatalogPage), 0);
    return ret;
}

TrieNodePosition CatalogManager::requestTrieNodePosition(){
    lock_guard<std::mutex> guard(request_mutex);
    int index = page->index;
    int page_id = page->page_id;
    index ++;
    if(index == TRIEBLOCK_LENGTH){
        index = 0;
        page_id ++;
    }
    page->page_id = page_id;
    page->index = index;
    //pwrite(fd, page, sizeof(CatalogPage), 0);
    return TrieNodePosition(page_id, index);
}
