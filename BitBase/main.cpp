
#include "network_interface.hpp"

#include <list>
#include "page_manager.hpp"
#include "trie_block.hpp"

using namespace std;

int main(int argc, char *argv[])
{
//    printf("%d\n",sizeof(TrieBlock));
    
//    PageManager bitbase = PageManager("/Users/lorabit/bitbase.bin", 2);
//    bitbase.openFile();
//    
//    TrieBlock block;
//    block.nodes[0].value = 1;
//    block.nodes[0].status = 1;
//    block.nodes[0].children[0].c = 'a';
//    block.nodes[0].children[0].pos.page_id = 0;
//    block.nodes[0].children[0].pos.index = 1;
//    
//    
//    bitbase.writePageToDisk(&block, 0);
//    
//    TrieBlock* b= (TrieBlock*)bitbase.readPage(0);
    
    
//    bitbase.closeFile();
    
    start_server();
    return 0;
}
