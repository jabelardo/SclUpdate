#ifndef SCLUPDATE_MONITOR_SERVICE_H
#define SCLUPDATE_MONITOR_SERVICE_H

#include <ace/Svc_Handler.h>
#include <ace/Message_Block.h>
#include <ace/SOCK_Stream.h>
#include <ace/Reactor_Notification_Strategy.h>

namespace SclUpdate
{

class UpdateManager;

class MonitorService : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
    private:
        typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Super;

    public:
        MonitorService();

        virtual ~MonitorService();

        void set_update_manager(UpdateManager* update_manager);

        virtual int open(void* p = 0);

        virtual int handle_input(ACE_HANDLE h);

        virtual int handle_output(ACE_HANDLE h);

    private:
        static size_t const MESSAGE_SIZE = 1;
        ACE_Message_Block* input_buffer;
        UpdateManager* update_manager;
        ACE_Auto_Ptr<ACE_Reactor_Notification_Strategy> notifier;

        int process_input();
};

} // namespace SclUpdate

#endif // SCLUPDATE_MONITOR_SERVICE_H
