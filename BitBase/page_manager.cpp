//
//  page_manager.cpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "page_manager.hpp"
#include <stdio.h>
#include "common.h"
#include <stdlib.h>

using namespace std;


PageManager::PageManager(string _filename,int _cache_size){
    filename = _filename;
    cache_size = _cache_size;
}

int PageManager::openFile(){
    fd = open(filename.c_str(), O_ACCMODE);
    if(fd==-1){
        printf("Error opening file %s",filename.c_str());
    }
    return fd;
}

bool PageManager::closeFile(){
    int ret = close(fd);
    if(ret == -1){
        printf("Error closing fd %d",fd);
        return false;
    }
    return true;
}

void* PageManager::readPage(int page_id){
    unordered_map<int, list<CacheNode>::iterator>::iterator map_iter = page_buffer_map.find(page_id);
    if(map_iter!=page_buffer_map.end()){
        list<CacheNode>::iterator list_iter = map_iter->second;
        CacheNode node = (*list_iter);
        page_buffer.erase(list_iter);
        page_buffer.push_back(node);
        page_buffer_map[page_id] = page_buffer.end();
        return node.data;
    }
    if(page_buffer.size()==cache_size){
        free(page_buffer.back().data);
        page_buffer_map.erase(page_id);
        page_buffer.erase(page_buffer.end());
    }
    CacheNode node = CacheNode(readPageFromDisk(page_id), page_id);
    page_buffer.push_back(node);
    page_buffer_map[page_id] = page_buffer.end();
    return node.data;
}

void* PageManager::readPageFromDisk(int page_id){
    void* data = mmap(NULL, PAGE_SIZE, PROT_READ, MAP_PRIVATE, fd, PAGE_SIZE*page_id);
    return data;
}

bool PageManager::writePageToDisk(void * data, int page_id){
    size_t ret = write(fd, data, PAGE_SIZE*page_id);
    if(ret == -1){
        printf("Error writing file %s",filename.c_str());
        return false;
    }
    return true;
}



