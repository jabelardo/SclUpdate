#include "monitor_acceptor.h"

namespace SclUpdate
{

MonitorAcceptor::MonitorAcceptor(UpdateManager* update_manager)
    : update_manager(update_manager)
{
}

MonitorAcceptor::~MonitorAcceptor()
{
}

int
MonitorAcceptor::make_svc_handler(MonitorService*& service)
{
    if (Super::make_svc_handler(service) == -1) {
        return -1;
    }
    service->set_update_manager(update_manager);
    return 0;
}

} // namespace SclUpdate
