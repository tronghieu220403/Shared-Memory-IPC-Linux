#include <iostream>
#include <vector>

#include "ipc/ipcsharedmem.h"
#include "process/process.h"

using namespace std;

ipc::IpcSharedMemory com;

void SendThread()
{
    process::Process p("process1");
    while(true)
    {
        string s;
        cin>>s;
        if (com.Send(p.GetPid(), std::vector<UCHAR>(s.begin(), s.end())) == true)
        {
            cout << "Sent successfully" << endl;
        }
    }
}

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
    thread recv_thread(&RecvThread);
    thread send_thread(&SendThread);

    recv_thread.join();
    send_thread.join();
    return 0;
}