#include "nt_service.h"
#include "update_manager.h"
#include "monitor_acceptor.h"

#if defined(ACE_WIN32) && !defined(ACE_LACKS_WIN32_SERVICES)

#include <ace/Reactor.h>
#include <ace/WIN32_Proactor.h>
#include <ace/Proactor.h>

namespace SclUpdate
{

char const* NtServiceImpl::DESCRIPTION = ACE_TEXT("Actualización de Scl Plus");
char const* NtServiceImpl::NAME = ACE_TEXT("SclUpdate");

NtServiceImpl::NtServiceImpl()
    : running(false)
{
    reactor(ACE_Reactor::instance());
}

NtServiceImpl::~NtServiceImpl()
{
}

ACE_NT_SERVICE_DEFINE(SclUpdateService, NtServiceImpl, NtServiceImpl::DESCRIPTION);

int
NtServiceImpl::run()
{
    ACE_NT_SERVICE_RUN(SclUpdateService, this, ret);
    return ret;
}

/** Este método es llamado cuando el servicio recibe una solicitud de control.
 *  Maneja las solicitudes de 'stop' y 'shutdown' llamando terminate().
 *  Todas las demas solicitudes son manejadas por el método del padre.
 */
void
NtServiceImpl::handle_control(DWORD control_code)
{
    if (control_code == SERVICE_CONTROL_SHUTDOWN || control_code == SERVICE_CONTROL_STOP) {

        report_status(SERVICE_STOP_PENDING);

        ACE_DEBUG((LM_INFO, "%T %P %t | Control 'stop' solicitado\n"));

        running = false;

        reactor()->notify(this, ACE_Event_Handler::EXCEPT_MASK);

    } else {
        Super::handle_control(control_code);
    }
}

/** Este método es llamado desde la notificación del método <handle_control>.
 *  Su objetivo es despertar al reactor de su estado de espera.
 */
int
NtServiceImpl::handle_exception(ACE_HANDLE)
{
  return 0;
}

int
NtServiceImpl::load_config()
{
    ACE_CString config_file = ACE_CString(NtServiceImpl::NAME) + ACE_CString(".conf");

    ACE_DEBUG((LM_DEBUG, "%T %P %t | cargando archivo %s\n", config_file.c_str()));
    if (!config.read_file(config_file)) {
        ACE_ERROR_RETURN((LM_ERROR,
                          "%T %P %t | No se puede leer el archivo %s\n",
                          config_file.c_str()),
                          -100);
    }
    return 0;
}

int
NtServiceImpl::svc()
{

    // As an NT service, we come in here in a different thread than the
    // one which created the reactor.  So in order to do anything, we
    // need to own the reactor. If we are not a service, report_status
    // will return -1.
    if (report_status(SERVICE_RUNNING) == 0) {
        reactor()->owner(ACE_Thread::self());
    }

    running = true;

    ACE_WIN32_Proactor proactor_impl(0, 1);
    ACE_Proactor proactor(&proactor_impl);
    ACE_Proactor::instance(&proactor);
    ACE_Reactor::instance()->register_handler(&proactor_impl, proactor_impl.get_handle());

    UpdateManager update_manager(config);
    if (update_manager.open(config.get_update_port()) == -1) {
        ACE_ERROR_RETURN((LM_ERROR, "%T %P %t | %p\n", "update_manager.open"), 400);
    }

    MonitorAcceptor monitor_acceptor(&update_manager);
    if (monitor_acceptor.open(config.get_monitor_port(), reactor()) == -1) {
        ACE_ERROR_RETURN((LM_ERROR, "%T %P %t | %p\n", "monitor_acceptor.open"), 500);
    }


    while (running) {
        reactor()->handle_events();
    }
   ACE_Reactor::instance()->remove_handler(proactor_impl.get_handle(), ACE_Event_Handler::DONT_CALL);

    return 0;
}

} // namespace SclUpdate

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */

