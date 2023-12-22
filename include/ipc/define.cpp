#include "ipc/define.h"

namespace ipc
{
    std::vector<UCHAR> Message::Flat() const
    {
        std::vector<UCHAR> flat_data(sizeof(ipc::MessageHeader) + data.size());
        memcpy(&flat_data[0], &header, sizeof(ipc::MessageHeader));
        memcpy(&flat_data[sizeof(ipc::MessageHeader)], &data[0], data.size());
        return flat_data;
    }

    DWORD Message::CalculateChecksum()
    {
        clock_t last_t;
        last_t = clock();
        return (((DWORD)last_t) * rand()%1000000)%1000000;
    }

}

