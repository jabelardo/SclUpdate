#include "monitor_protocol.h"
#include "crc16.h"

namespace SclUpdate
{

ACE_Message_Block*
serialize(UpdateStatsMap& statistics)
{
    /* Fromato del mensaje:
     * 2 bytes LE tamaño del mensaje (incluye el crc16 pero no a él mismo)
     * 1 byte número de versiones
     * despues se repiten n veces:
     *      1 byte version
     *      2 bytes LE active_downloads
     *      2 bytes LE complete_downloads
     * 2 bytes de crc16 del bloque anterior (incluye el tamaño pero no a él mismo)
     */
    size_t block_size = 3 + statistics.size() * 5 + 2;

    ACE_Message_Block* mb = 0;
    ACE_NEW_RETURN(mb, ACE_Message_Block(block_size), 0);

    ACE_UINT16 msg_size  = block_size - 2;
    ACE_UINT8 count_vers = statistics.size();

    *reinterpret_cast<ACE_UINT16*>(mb->wr_ptr()) = msg_size;
    mb->wr_ptr(sizeof(ACE_UINT16));

    *reinterpret_cast<ACE_UINT8*>(mb->wr_ptr()) = count_vers;
    mb->wr_ptr(sizeof(ACE_UINT8));

    for (UpdateStatsMap::iterator i = statistics.begin(); i != statistics.end(); ++i) {

        *mb->wr_ptr() = i->first;
        mb->wr_ptr(1);

        *reinterpret_cast<ACE_UINT16*>(mb->wr_ptr()) = i->second.get_active_downloads();
        mb->wr_ptr(sizeof(ACE_UINT16));

        *reinterpret_cast<ACE_UINT16*>(mb->wr_ptr()) = i->second.get_complete_downloads();
        mb->wr_ptr(sizeof(ACE_UINT16));
    }

    ACE_UINT16 crc16 = CRC16_BlockChecksum(mb->rd_ptr(), msg_size);

    *reinterpret_cast<ACE_UINT16*>(mb->wr_ptr()) = crc16;
    mb->wr_ptr(sizeof(ACE_UINT16));

    return mb;
}

} // namespace SclUpdate
