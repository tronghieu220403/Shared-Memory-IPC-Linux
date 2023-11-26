#include <iostream>
#include <vector>

#include "ipc/ipcsharedmem.h"
#include "process/process.h"

using namespace std;

ipc::IpcSharedMemory com;

void RecvThread()
{
    while(true)
    {
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