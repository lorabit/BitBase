


#include "network_interface.hpp"

#include <list>
#include "trie_block.hpp"
#include "page_manager.hpp"
#include "data_interface.hpp"
#include "benchmark/benchmark.h"
#include "catalog.hpp"

using namespace std;

void test(char  *a){
    printf("%p\n",a);
    printf("%d",a[2]);
    a[2] = 1;
}

string charSet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_- ";

static void BM_MultiThreaded(benchmark::State& state) {
    int i= 0;
    string key = "";
    for(int j = 0; j < 32; j++) key += "a";
    while (state.KeepRunning()) {
//        vector<TrieNodePosition> pos = cm->requestTrieNodePosition(1);

        key[rand()%key.size()] = charSet[rand()%charSet.size()];
        data_set(key,rand());
        i++;
//        state.SetIterationTime(1.1);
    }
    state.SetItemsProcessed(i);
    if (state.thread_index == 0) {
      
    }
}

BENCHMARK(BM_MultiThreaded)->Threads(1)->Repetitions(10)->Unit(benchmark::kMicrosecond)->UseRealTime();



int main(int argc, char *argv[])
{

    printf("%d\n",(int)sizeof(TrieBlock));
    return 0;
    init_database();
//    data_set("aaa", 5);
//    data_set("aab", 15);
//    data_set("aaaaaaaaf", 19);
//    printf("%d\n",data_get("aaa").value);
//    printf("%d\n",data_get("aaabbb").value);
//    printf("%d\n",data_get("aab").value);
//    printf("%d\n",data_get("aaaaaaaaf").value);
    
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    
//    char a[5];
//    printf("%p\n",&a);
//    a[2] = 5;
//    test(a);
//    printf("%d\n",a[2]);
//    printf("%d\n",sizeof(TrieBlock));
//    PageManager bitbase = PageManager("/Users/lorabit/bitbase.bin", 2);
//    bitbase.openFile();

//    TrieBlock block;
//    block.nodes[0].value = 1;
//    block.nodes[0].status = 1;
//    block.nodes[0].children[0].c = 'a';
//    block.nodes[0].children[0].pos.page_id = 0;
//    block.nodes[0].children[0].pos.index = 1;


//    bitbase.writePageToDisk(&block, 0);

//    TrieBlock* b= (TrieBlock*)bitbase.readPage(0);
    
    
//    bitbase.closeFile();
//    init_database();
//    start_server();
    return 0;
}
