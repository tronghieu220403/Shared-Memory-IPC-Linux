#include <iostream>
#include <vector>

#include "memory/sharedmemory.h"

using namespace std;

int main()
{
    memory::SharedMemory shm("hieu", 0x1000);
    shm.Create();
    cout << hex << (size_t)shm.GetSharedMemoryPointer() << endl;
    string s = "hieu" + std::to_string(rand());
    cout << s << endl;
    shm.Write(0, &s[0], s.size());
    sleep(100);

}