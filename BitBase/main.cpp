


#include "network_interface.hpp"

#include <list>
#include "trie_block.hpp"
#include "page_manager.hpp"
#include "data_interface.hpp"
#include "benchmark/benchmark.h"
#include "catalog.hpp"
#include "hash_trie_manager.hpp"



using namespace std;

void test(char  *a){
    printf("%p\n",a);
    printf("%d",a[2]);
    a[2] = 1;
}

//string charSet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_- ";

string charSet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static void BM_MultiThreaded(benchmark::State& state) {
    int i= 0;
    string key = "";
    for(int j = 0; j < 8; j++) key += charSet[rand()%charSet.size()];
    while (state.KeepRunning()) {
        int p = rand();
//        printf("%d\n",p);
        data_set(to_string(p), p);
//        versioned_value test = data_get(to_string(p));
//        if(test.value!=p){
//            printf("--------------- error %d %d %d\n",p,test.value, test.version);
//        }
        
        
//        key[rand()%key.size()] = charSet[rand()%charSet.size()];
//        data_get(key);
//        data_set(key,1);
        i++;
    }
    state.SetItemsProcessed(i);
    if (state.thread_index == 0) {
      
    }
}

BENCHMARK(BM_MultiThreaded)->Threads(1)->Repetitions(10)->Unit(benchmark::kMicrosecond)->UseRealTime();


void benchmarks(int argc, char *argv[]){
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}

void correct(){
//    init_database();
    data_set("49", 49);

    printf("%d\n",data_get("49").value);
    printf("%d\n",data_get("aaabbb").value);
    printf("%d\n",data_get("aab").value);
    printf("%d\n",data_get("aaaaaaaaf").value);
    printf("%d\n",data_get("aaf").value);
}

void testSize(){
    printf("%d\n",(int)sizeof(HashTrieBlock));
}

void interactive(){
    while(true){
        int p;
        scanf("%d",&p);
        data_set(to_string(p), p);
        versioned_value test = data_get(to_string(p));
        if(test.value!=p){
            printf("error %d %d\n",p,test.value);
        }else{
            printf("OK %d %d\n",p,test.version);
        
        }
    }
}


int main(int argc, char *argv[])
{
    
    init_database();
    
//    inspect();
//        testSize();
        benchmarks(argc, argv);
//    interactive();
//    correct();
    

//
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
