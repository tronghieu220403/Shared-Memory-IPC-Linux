#include <iostream>
#include <vector>

#include "ipc/define.h"
#include "memory/sharedmemory.h"

using namespace std;

int main()
{
    memory::SharedMemory shm(IPC_SHM_NAME, IPC_SHM_SIZE);
    cout << hex << (size_t)shm.GetSharedMemoryPointer() << endl;
    vector<UCHAR> s = shm.Read(shm.GetSharedMemoryPointer(),12);
    
    for (int i = 0; i < s.size(); i++)
    {
        cout << (char)s[i];
    }
    cout << endl;

}