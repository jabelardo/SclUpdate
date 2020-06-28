#ifndef SCLUPDATE_MONITOR_PROTOCOL_H
#define SCLUPDATE_MONITOR_PROTOCOL_H

#include <ace/Message_Block.h>
#include <SclUpdate/update_manager.h>

namespace SclUpdate
{

ACE_Message_Block* serialize(UpdateStatsMap& statistics);

} // namespace SclUpdate

#endif // SCLUPDATE_MONITOR_PROTOCOL_H
