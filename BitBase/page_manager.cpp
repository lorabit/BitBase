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
    if(catalog_manager->page->page_id == 0)
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
    printf("truncate %lld\n",off_t(page_id/1024+1)*1024*PAGE_SIZE);
    ftruncate(fd, off_t(page_id/1024+1)*1024*PAGE_SIZE);
//    void * new_space = mmap(NULL, 1024*PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, off_t(page_id/1024)*1024*PAGE_SIZE);
//    memset(new_space, 0, 1024*PAGE_SIZE);
//    munmap(new_space, 1024*PAGE_SIZE);
}


void PageManager::move_to_rear(const list<CacheNode>::iterator & list_iter){
        CacheNode node = (*list_iter);
        page_buffer.erase(list_iter);
        page_buffer.push_back(node);
        page_buffer_map[node.page_id] = --page_buffer.end();
}


void PageManager::msync(int page_id){
    unordered_map<int, list<CacheNode>::iterator>::iterator map_iter = page_buffer_map.find(page_id);
    if(map_iter!=page_buffer_map.end()){
        list<CacheNode>::iterator list_iter = map_iter->second;
        ::msync(list_iter->data, PAGE_SIZE, MAP_SHARED);
    };
}

void* PageManager::readPage(int page_id){
//    lock_guard<std::mutex> guard(read_page_mutex);
    unordered_map<int, list<CacheNode>::iterator>::iterator map_iter = page_buffer_map.find(page_id);
    if(map_iter!=page_buffer_map.end()){
        list<CacheNode>::iterator list_iter = map_iter->second;
        void * ret = list_iter->data;
        list_iter->reference_count ++;
//        printf("%d = %p %d\n",page_id, list_iter->data, list_iter->reference_count);
        move_to_rear(list_iter);
        return ret;
    }
    if(page_buffer.size()==cache_size){
        while(page_buffer.size()==cache_size){
            if(page_buffer.front().page_id == page_id){
                move_to_rear(page_buffer.begin());
                break;
            }
            if(page_buffer.front().reference_count == 0){
//                printf("%d = d(%p) %d\n",page_buffer.front().page_id, page_buffer.front().data, page_buffer.front().reference_count);
                
                munmap(page_buffer.front().data, PAGE_SIZE);
                page_buffer_map.erase(page_buffer.front().page_id);
                page_buffer.erase(page_buffer.begin());
                break;
            }else{
                move_to_rear(page_buffer.begin());
            }
            
        }
    }
    CacheNode node = CacheNode(readPageFromDisk(page_id), page_id);
    node.reference_count++;
//    printf("%d = n(%p) %d\n",page_id, node.data, node.reference_count);
    page_buffer.push_back(node);
    page_buffer_map[page_id] = --page_buffer.end();
    return node.data;
}

void PageManager::release(int page_id){
    unordered_map<int, list<CacheNode>::iterator>::iterator map_iter = page_buffer_map.find(page_id);
    if(map_iter!=page_buffer_map.end()){
        list<CacheNode>::iterator list_iter = map_iter->second;
        list_iter->reference_count --;
//        printf("%d = %d\n",page_id, list_iter->reference_count);
    }
}

void* PageManager::readPageFromDisk(int page_id){
//    printf("%d\n",PAGE_SIZE);
    void* data = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, off_t(PAGE_SIZE)*page_id);
    if (data == MAP_FAILED)
        handle_error("mmap");
//    printf("Read\n");
    return data;
}

bool PageManager::writePageToDisk(void * data, int page_id){
//    lock_guard<std::mutex> guard(write_page_mutex);
    size_t ret = pwrite(fd, data, PAGE_SIZE, off_t(PAGE_SIZE)*page_id);
    if(ret == -1){
        printf("Error writing file %s\n",filename.c_str());
        return false;
    }
//    printf("Write\n");
    return true;
}



