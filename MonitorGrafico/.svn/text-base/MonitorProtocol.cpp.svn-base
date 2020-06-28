#include "MonitorProtocol.h"
#include "crc16.h"

int
Unserialize(wxMemoryBuffer& buffer, UpdateStatsMap& stats)
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

    if (buffer.GetDataLen() < 5) {
        return -1;
    }

    int bufferPos = 0;
    char* bufferStart = reinterpret_cast<char*>(buffer.GetData());

    wxUint16 msgSize = *reinterpret_cast<wxUint16*>(bufferStart + bufferPos);
    bufferPos += sizeof(wxUint16);

    wxUint8 countVers = bufferStart[bufferPos];
    bufferPos += 1;

    if (buffer.GetDataLen() != 3 + countVers * 5 + 2) {
        return -2;
    }

    UpdateStatsMap result(countVers);

    for (wxUint8 i = 0; i < countVers; ++i) {
        char updateName = bufferStart[bufferPos];
        bufferPos += 1;

        wxUint16 activeDownloads = *reinterpret_cast<wxUint16*>(bufferStart + bufferPos);
        bufferPos += sizeof(wxUint16);

        wxUint16 completeDownloads = *reinterpret_cast<wxUint16*>(bufferStart + bufferPos);
        bufferPos += sizeof(wxUint16);

        result[updateName] = UpdateStats(updateName, activeDownloads, completeDownloads);
    }

    if (msgSize != bufferPos) {
        return -3;
    }

    wxUint16 crc16 = *reinterpret_cast<wxUint16*>(bufferStart + bufferPos);
    bufferPos += sizeof(wxUint16);

    if (bufferPos - 2 != msgSize) {
        return -5;
    }

    wxUint16 crc16Calc = CRC16_BlockChecksum(bufferStart, msgSize);

    if (crc16 != crc16Calc) {
        return -5;
    }

    stats = result;
    return 0;
}
