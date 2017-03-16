//
//  page_manager.cpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "page_manager.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include "trie_block.hpp"

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;


PageManager::PageManager(string _filename,int _cache_size){
    filename = _filename;
    cache_size = _cache_size;
}

int PageManager::openFile(){
    fd = open(filename.c_str(), O_RDWR);
    if(fd==-1){
        perror("Error opening file");
    }
    catalog_manager = new CatalogManager(filename+".catalog");
    truncate(0);
    return fd;
}

bool PageManager::closeFile(){
    int ret = close(fd);
    if(ret == -1){
        printf("Error closing fd %d\n",fd);
        return false;
    }
    return true;
}

std::mutex read_page_mutex;
std::mutex write_page_mutex;

void PageManager::truncate(int page_id){
    ftruncate(fd, (page_id/1024+2)*1024*PAGE_SIZE);
}

void* PageManager::readPage(int page_id){
    lock_guard<std::mutex> guard(read_page_mutex);
    unordered_map<int, list<CacheNode>::iterator>::iterator map_iter = page_buffer_map.find(page_id);
    if(map_iter!=page_buffer_map.end()){
        list<CacheNode>::iterator list_iter = map_iter->second;
        CacheNode node = (*list_iter);
        page_buffer.erase(list_iter);
        page_buffer.push_back(node);
        page_buffer_map[page_id] = --page_buffer.end();
        return node.data;
    }
    if(page_buffer.size()==cache_size){

//        free(page_buffer.back().data);
        munmap(page_buffer.back().data, PAGE_SIZE);
        page_buffer_map.erase(page_buffer.back().page_id);
        page_buffer.erase(--page_buffer.end());
    }
    CacheNode node = CacheNode(readPageFromDisk(page_id), page_id);
    page_buffer.push_back(node);
    page_buffer_map[page_id] = --page_buffer.end();
    return node.data;
}

void* PageManager::readPageFromDisk(int page_id){
//    printf("%d\n",PAGE_SIZE);
    void* data = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PAGE_SIZE*(page_id+1));
    if (data == MAP_FAILED)
        handle_error("mmap");
    data = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PAGE_SIZE*page_id);
    if (data == MAP_FAILED)
        handle_error("mmap");
//    printf("Read\n");
    return data;
}

bool PageManager::writePageToDisk(void * data, int page_id){
    lock_guard<std::mutex> guard(write_page_mutex);
    size_t ret = pwrite(fd, data, PAGE_SIZE, PAGE_SIZE*page_id);
    if(ret == -1){
        printf("Error writing file %s\n",filename.c_str());
        return false;
    }
//    printf("Write\n");
    return true;
}



