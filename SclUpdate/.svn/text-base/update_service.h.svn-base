#ifndef SCLUPDATE_UPDATE_SERVICE_H
#define SCLUPDATE_UPDATE_SERVICE_H

#include <ace/Asynch_IO.h>
#include <SclUpdate/configuration.h>

namespace SclUpdate
{

class UpdateManager;

class UpdateService : public  ACE_Service_Handler
{
    private:
        typedef  ACE_Service_Handler Super;

    public:
        UpdateService();

        virtual ~UpdateService();

        void set_config(Configuration const& config);

        void set_update_manager(UpdateManager* update_manager);

        virtual void open(ACE_HANDLE h, ACE_Message_Block&);

        virtual void handle_read_stream(ACE_Asynch_Read_Stream::Result const& result);

        virtual void handle_write_stream(ACE_Asynch_Write_Stream::Result const& result);

    private:
        static size_t const MESSAGE_SIZE = 5;

        ACE_Asynch_Read_Stream reader;
        ACE_Asynch_Write_Stream writer;
        UpdateManager* update_manager;
        Configuration config;
        char update_name;
        bool request_ok_sent;

        int process_input(ACE_Message_Block& input_buffer);
        int enqueue_file(ACE_CString const& filename);
        void handle_write_file(ACE_Asynch_Write_Stream::Result const& result);
        void handle_write_request_ok(ACE_Asynch_Write_Stream::Result const& result);

        bool write(ACE_Message_Block& output_buffer);
        bool read(ACE_Message_Block& input_buffer);

};

} // namespace SclUpdate

#endif // SCLUPDATE_UPDATE_SERVICE_H
