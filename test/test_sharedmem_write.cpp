#include <iostream>
#include <vector>

#include "ipc/define.h"
#include "memory/sharedmemory.h"


using namespace std;

int main()
{
    memory::SharedMemory shm(IPC_SHM_NAME, IPC_SHM_SIZE);
    cout << hex << (size_t)shm.GetSharedMemoryPointer() << endl;
    string content(IPC_SHM_SIZE, '\0');
    if (shm.Write(shm.GetSharedMemoryPointer(),&content[0], content.size()) == false)
    {
        cout << "Can not write";
    }
    
    cout << endl;
    // sleep(100);

}