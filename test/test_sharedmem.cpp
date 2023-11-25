#include <iostream>
#include <vector>

#include "memory/sharedmemory.h"

using namespace std;

int main()
{
    memory::SharedMemory shm("hieu", 0x1000);
    shm.Create();
    cout << hex << (size_t)shm.GetSharedMemoryPointer() << endl;
    vector s = shm.Read(0,12);
    
    for (int i = 0; i < s.size(); i++)
    {
        cout << (char)s[i];
    }
    cout << endl;
    sleep(100);

}