#include <iostream>
#include <vector>

#include "ipc/ipcsharedmem.h"
#include "process/process.h"

using namespace std;

ipc::IpcSharedMemory com;

void DumpHeapMemory()
{
    /*
    ofstream MyFile("dump.txt");
    UCHAR* ptr = (UCHAR *)com.GetHeapManager().GetStartPointer();
    for (int i = 0; i < 0x200; i+=8)
    {
        MyFile << "0x" << hex << i << ":\t";
        for (int j = 0; j < 2; j++)
        {
            MyFile << "0x" << hex << *(DWORD *)&ptr[i+j*4] << "\t";
        }
        MyFile << endl;
    }
    MyFile.close();
    */
}

void RecvThread()
{
    while(true)
    {
        // DumpHeapMemory();
        for (auto u: com.Receive())
        {
            cout << "Message from PID " << u.header.send_pid << ": ";
            for (auto c: u.data)
            {
                cout << c;
            }
            cout << endl;
        }
        sleep(1);
    }
}

int main()
{
    cout << "test_recv PID: " << ::getpid() << endl;
    thread recv_thread(&RecvThread);recv_thread.join();
    return 0;
}