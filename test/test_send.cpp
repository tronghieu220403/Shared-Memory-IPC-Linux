#include <iostream>
#include <vector>

#include "ipc/ipcsharedmem.h"
#include "process/process.h"

using namespace std;

ipc::IpcSharedMemory com;

void SendThread()
{
    cout << "This process PID: " << ::getpid() << endl;

    process::Process p("test_recv");
    while(true)
    {
        if (p.IsActive() == false)
        {
            while(true)
            {
                cout << "Updating PID..." << endl;
                if (p.UpdatePid() == 0)
                {
                    sleep(1);
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
        cout << "test_recv PID: " << p.GetPid() << endl;
        string s;
        cin>>s;
        if (com.Send(p.GetPid(), std::vector<UCHAR>(s.begin(), s.end())) == true)
        {
            cout << "Sent successfully" << endl;
        }
        else
        {
            cout << "Sent failded" << endl; 
        }
    }
}

int main()
{
    thread send_thread(&SendThread); send_thread.join();
    return 0;
}