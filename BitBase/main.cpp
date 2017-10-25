


#include "network_interface.hpp"

#include <list>
#include "trie_block.hpp"
#include "page_manager.hpp"
#include "data_interface.hpp"
#include "benchmark/benchmark.h"
#include "catalog.hpp"
#include "hash_trie_manager.hpp"

#include <iostream>
#include <sys/time.h>


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
//    string key = "";
//    for(int j = 0; j < 8; j++) key += charSet[rand()%charSet.size()];
    while (state.KeepRunning()) {
        int p = rand();
//        printf("%d\n",p);
//        data_get(to_string(p));
        data_set(to_string(p), p);
        versioned_value test = data_get(to_string(p));
        if(test.value!=p){
            printf("--------------- error %d %d %d\n",p,test.value, test.version);
        }
        
        
//        key[rand()%key.size()] = charSet[rand()%charSet.size()];
//        data_get(key);
//        data_set(key,1);
        i++;
    }
    state.SetItemsProcessed(i);
}

BENCHMARK(BM_MultiThreaded)->Threads(1)->Repetitions(10)->Unit(benchmark::kMicrosecond)->UseRealTime();


void benchmarks(int argc, char *argv[]){
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}

void correct(){
    for(int i = 0; i < 10000; i ++){
            printf("%d\n",data_get(to_string(i)).value);
//        data_set(to_string(i), i);
    }
    
//    init_database();
//    data_set("49", 49);

//    printf("%d\n",data_get("49").value);
//    printf("%d\n",data_get("aaabbb").value);
//    printf("%d\n",data_get("aab").value);
//    printf("%d\n",data_get("aaaaaaaaf").value);
//    printf("%d\n",data_get("aaf").value);
}

void testSize(){
    printf("%d\n",(int)sizeof(TrieNode));
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

void int_key(int m, int count){
    int p = 0;
    for(int i = 0; i < count; i++){
        p = rand()%m;
        data_set(to_string(p), p);
    }
}

void edit_str(int n, int m, int count){
    string key = "";
    for(int j = 0; j < n; j++) key += charSet[rand()%m];
    for(int i = 0; i < count; i++){
//        cout << key <<endl;
        key[rand()%n] = charSet[rand()%m];
        data_set(key,1);
    }
}


void rand_str(int n, int m, int count){
    string key = "";
    for(int j = 0; j < n; j++) key += " ";
    for(int i = 0; i < count; i++){
        for(int j = 0; j < n; j++)
            key[j] = charSet[rand()%m];
        data_set(key,1);
    }
}

void test1(){
    srand ((unsigned)time(NULL));
    int n = 1000000;
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
        int_key(2048, n);
//    edit_str(8, 32, n);
//    rand_str(10, 32, n);
    gettimeofday(&tv_end, NULL);
    long time = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + tv_end.tv_usec - tv_begin.tv_usec;
    double qps = (double)n*1000/time;
    cout << "Queries: "<<n<<" \n";
    printf("Qps: %.6fk\n",qps);
    printf("Time: %.6fs\n",(double)time/1000000);
}

void splitbybit(){
    int char_len = 4;
    string s = "abcd";
    int d = (1 << (char_len+1)) -1;
    int offset = 0;
    while(offset < s.size()*8){
        int cur = (s[offset/8] >> (offset%8)) & d;
        printf("%d ",cur);
        offset += char_len;
    }
}


int main(int argc, char *argv[])
{
    
//    size_t page_size = (size_t) sysconf (_SC_PAGESIZE);
//    printf("%zu",page_size);
    init_database();
    test1();
    
//    inspect();
//        testSize();
//        benchmarks(argc, argv);
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
