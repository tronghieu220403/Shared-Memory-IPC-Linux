#include "ipc/define.h"

namespace ipc
{
    std::vector<UCHAR> MessageStructure::Flat() const
    {
        std::vector<UCHAR> flat_data(sizeof(ipc::MessageHeader) + data.size());
        memcpy(&flat_data[0], &header, sizeof(ipc::MessageHeader));
        memcpy(&flat_data[sizeof(ipc::MessageHeader)], &data[0], data.size());
        return flat_data;
    }
}

