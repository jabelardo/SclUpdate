#ifndef SCLUPDATE_MONITORACCEPTOR_H
#define SCLUPDATE_MONITORACCEPTOR_H

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <SclUpdate/monitor_service.h>

namespace SclUpdate
{

class MonitorAcceptor : public ACE_Acceptor<MonitorService, ACE_SOCK_ACCEPTOR>
{
        typedef ACE_Acceptor<MonitorService, ACE_SOCK_ACCEPTOR> Super;

    public:
        MonitorAcceptor(UpdateManager* update_manager);

        virtual ~MonitorAcceptor();

        virtual int make_svc_handler(MonitorService*& service);

    private:
        UpdateManager* update_manager;
};

} // namespace SclUpdate

#endif // SCLUPDATE_MONITORACCEPTOR_H
