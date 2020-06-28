#include "monitor_service.h"
#include "update_manager.h"
#include "monitor_protocol.h"
#include "monitor_acceptor.h"

namespace SclUpdate
{

/** @brief MonitorService
  *
  * @todo: document this function
  */
MonitorService::MonitorService()
    : input_buffer(0)
    , update_manager(0)
    , notifier()
{
}

void
MonitorService::set_update_manager(UpdateManager* update_manager)
{
    this->update_manager = update_manager;
}

/** @brief ~MonitorService
  *
  * @todo: document this function
  */
MonitorService::~MonitorService()
{
    if (input_buffer) {
        input_buffer = input_buffer->release();
    }
}

/** @brief open
  *
  * @todo: document this function
  */
int
MonitorService::open(void* p)
{
    ACE_INET_Addr peer_addr;
    if (peer().get_remote_addr(peer_addr) != 0) {
        return -1;
    }
    if (ACE_OS::strcmp(peer_addr.get_host_addr(), "127.0.0.1") != 0) {
        ACE_DEBUG((LM_INFO, "%T %P %t | Cliente de monitoreo rechazado, ip = %s\n", peer_addr.get_host_addr()));
        return -1;
    }
    if (Super::open(p) != 0) {
        return -1;
    }
    ACE_DEBUG((LM_INFO, "%T %P %t | Cliente de monitoreo conectado, ip = %s\n", peer_addr.get_host_addr()));

    ACE_Reactor_Notification_Strategy* new_notifier = 0;
    ACE_NEW_RETURN(new_notifier, ACE_Reactor_Notification_Strategy(reactor(), this, ACE_Event_Handler::WRITE_MASK), -1);
    notifier.reset(new_notifier);
    msg_queue()->notification_strategy(notifier.get());

    return 0;
}

/** @brief handle_output
  *
  * @todo: document this function
  */
int
MonitorService::handle_output(ACE_HANDLE h)
{
    ACE_Message_Block* mb = 0;
    ACE_Time_Value nowait(ACE_OS::gettimeofday());
    if (-1 != getq(mb, &nowait)) {
        ssize_t send_cnt = peer().send(mb->rd_ptr(), mb->length());
        if (send_cnt != -1) {
            mb->rd_ptr(send_cnt);
        }
        if (mb->length() > 0) {
            ungetq(mb);
        } else {
            mb->release();
        }
    }
    if (msg_queue()->is_empty()) {
        reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);
    } else {
        reactor ()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);
    }
    return 0;
}

int
MonitorService::process_input()
{
    char msg[] = {'\0','\0'};
    ACE_OS::memcpy(msg, input_buffer->rd_ptr(), 1);

    input_buffer->wr_ptr(input_buffer->rd_ptr());

    if (update_manager == 0) {
        return -1;
    }
    if (ACE_OS::strcmp(msg, "1") != 0) {
        ACE_ERROR_RETURN((LM_INFO, "%T %P %t | cliente de monitoreo mensaje desconocido %s\n", msg), -1);
    }
    UpdateStatsMap statistics;
    if (update_manager->get_statistics(statistics) == -1) {
        return -1;
    }
    ACE_Message_Block* mb = 0;
    if ((mb = serialize(statistics)) == 0) {
        return -1;
    }
    ACE_Time_Value nowait(ACE_OS::gettimeofday());
    if (putq(mb, &nowait) == -1) {
        mb->release();
        return -1;
    }
    return 0;
}

int
MonitorService::handle_input(ACE_HANDLE h)
{
    if (input_buffer == 0) {
        ACE_NEW_RETURN(input_buffer, ACE_Message_Block(MESSAGE_SIZE), -1);
    }

    ssize_t read = peer().recv(input_buffer->wr_ptr(), MESSAGE_SIZE);

    switch (read) {
    case -1:
        ACE_ERROR_RETURN((LM_INFO, "%T %P %t | %p error en lectura\n", "cliente de monitoreo"), -1);

    case 0:
        ACE_ERROR_RETURN((LM_INFO, "%T %P %t | cliente de monitoreo desconectado (fd = %d)\n", get_handle()), -1);

    default:
        input_buffer->wr_ptr(read);
        if (input_buffer->length() == MESSAGE_SIZE) {
            return process_input();
        }
    }
    return 0;
}

} // namespace SclUpdate
