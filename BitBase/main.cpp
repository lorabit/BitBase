
#include "network_interface.hpp"

#include <list>

using namespace std;

int main(int argc, char *argv[])
{
    
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);
    list<int>::iterator iter = l.begin();
    printf("%d",*iter);
    iter++;
    iter++;
    printf("%d",*iter);
    iter--;
    printf("%d",*iter);
    
    
    
//    start_server();
    return 0;
}
