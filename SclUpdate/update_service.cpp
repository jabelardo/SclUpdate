#include "update_service.h"
#include "update_manager.h"
#include <ace/OS.h>

namespace SclUpdate
{

UpdateService::UpdateService()
    : reader()
    , writer()
    , update_manager(0)
    , config()
    , update_name(0)
    , request_ok_sent(false)
{
}

UpdateService::~UpdateService()
{
    if (handle() != ACE_INVALID_HANDLE) {
        ACE_OS::closesocket(handle());
    }
}

void
UpdateService::set_config(Configuration const& config)
{
    this->config = config;
}

void
UpdateService::set_update_manager(UpdateManager* update_manager)
{
    this->update_manager = update_manager;
}

void
UpdateService::open(ACE_HANDLE h, ACE_Message_Block&)
{
    handle (h);
    if (reader.open(*this) != 0 || writer.open(*this) != 0) {
        ACE_ERROR((LM_ERROR, "%p\n", "UpdateService open"));
        delete this;
        return;
    }

    char const* request_ok = "REQUEST OK";
    ACE_Message_Block *output_buffer;
    ACE_NEW_NORETURN(output_buffer, ACE_Message_Block(ACE_OS::strlen(request_ok)));
    if (output_buffer->copy(request_ok, ACE_OS::strlen(request_ok))!= 0) {
        ACE_ERROR((LM_ERROR, "%p\n", "UpdateService::open"));
        output_buffer->release();
        delete this;
        return;
    }
    if (!write(*output_buffer)) {
        delete this;
    }
}

bool
UpdateService::write(ACE_Message_Block& output_buffer)
{
    if (writer.write(output_buffer, output_buffer.length()) != 0) {
        ACE_ERROR((LM_ERROR, "%p\n", "UpdateService::write"));
        output_buffer.release();
        return false;
    }
    return true;
}

bool
UpdateService::read(ACE_Message_Block& input_buffer)
{
    if (reader.read(input_buffer, input_buffer.space()) != 0) {
        ACE_ERROR((LM_ERROR, "%p\n", "UpdateService::read"));
        input_buffer.release();
        return false;
    }
    return true;
}

int
UpdateService::enqueue_file(ACE_CString const& filename)
{
    ACE_Message_Block* output_buffer = 0;
    if (update_manager->get_file_buffer(filename, output_buffer) == -1) {
        ACE_ERROR((LM_ERROR, "%p\n", "get_file_buffer"));
        return -1;
    }
    if (!write(*output_buffer)) {
        return -1;
    }
    update_manager->increase_active_downloads(update_name);
    return 0;
}

int
UpdateService::process_input(ACE_Message_Block& input_buffer)
{
    if (ACE_OS::strncmp("OK", input_buffer.rd_ptr(), 2) == 0) {
        update_name = input_buffer.rd_ptr()[2];
        ACE_CString file = config.get_update_file(update_name);

        if (file.length() != 0) {
            if (enqueue_file(file) == -1) {
                return -1;
            }
        }
    }
    return 0;
}

void
UpdateService::handle_read_stream(ACE_Asynch_Read_Stream::Result const& result)
{
    ACE_Message_Block& input_buffer = result.message_block();

    if (!result.success() || result.bytes_transferred() == 0) {
        ACE_ERROR((LM_INFO, "%T %P %t | cliente de actualizacion desconectado (fd = %d)\n", handle()));
        input_buffer.release();
        delete this;
        return;
    }
    if (input_buffer.length() < MESSAGE_SIZE) {
        if (!read(input_buffer)) {
            delete this;
        }
    } else {
        if (process_input(input_buffer) == -1) {
            ACE_ERROR((LM_INFO, "%T %P %t | cliente de actualizacion process_input error (fd = %d)\n", handle()));
            input_buffer.release();
            delete this;
            return;
        }
        input_buffer.release();
    }
}

void
UpdateService::handle_write_stream(ACE_Asynch_Write_Stream::Result const& result)
{
    if (request_ok_sent) {
        handle_write_file(result);
    } else {
        handle_write_request_ok(result);
    }
}

void
UpdateService::handle_write_request_ok(ACE_Asynch_Write_Stream::Result const& result)
{
    ACE_Message_Block& output_buffer = result.message_block();

    if (!result.success() || result.bytes_transferred() == 0) {
        output_buffer.release();
        delete this;
        return;
    }
    if (output_buffer.length() > 0) {
        if (!write(output_buffer)) {
            delete this;
            return;
        }
    }

    result.message_block().release();
    request_ok_sent = true;

    ACE_Message_Block *input_buffer;
    ACE_NEW_NORETURN(input_buffer, ACE_Message_Block(MESSAGE_SIZE));
    if (!read(*input_buffer)) {
        delete this;
    }
}

void
UpdateService::handle_write_file(ACE_Asynch_Write_Stream::Result const& result)
{
    ACE_Message_Block& output_buffer = result.message_block();

    if (!result.success() || result.bytes_transferred() == 0) {
        ACE_ERROR((LM_INFO, "%T %P %t | cliente de actualizacion desconectado (fd = %d)\n", handle()));
        output_buffer.release();
        update_manager->decrease_active_downloads(update_name);
        delete this;
        return;
    }
    if (output_buffer.length() > 0) {
        if (!write(output_buffer)) {
            update_manager->decrease_active_downloads(update_name);
            delete this;
            return;
        }
    }
    ACE_ERROR((LM_INFO, "%T %P %t | cliente de actualizacion envio completo (fd = %d)\n", handle()));
    result.message_block().release();
    update_manager->decrease_active_downloads(update_name);
    update_manager->increase_complete_downloads(update_name);

    ACE_Message_Block *input_buffer;
    ACE_NEW_NORETURN(input_buffer, ACE_Message_Block(1));
    if (!read(*input_buffer)) {
        delete this;
    }
}

} // namespace SclUpdate
