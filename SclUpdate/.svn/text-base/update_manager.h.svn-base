#ifndef SCLUPDATE_UPDATE_MANAGER_H
#define SCLUPDATE_UPDATE_MANAGER_H

#include <ace/Asynch_Acceptor.h>
#include <ace/Recursive_Thread_Mutex.h>
#include <SclUpdate/update_service.h>
#include <SclUpdate/update_stats.h>
#include <SclUpdate/configuration.h>

namespace SclUpdate
{

class UpdateManager : public ACE_Asynch_Acceptor<UpdateService>
{
    private:
        typedef ACE_Asynch_Acceptor<UpdateService> Super;

    public:
        UpdateManager(Configuration const& config);

        virtual ~UpdateManager();

        virtual int open(ACE_INET_Addr const& address,
                         size_t bytes_to_read = 0,
                         bool pass_addresses = false,
                         int backlog = ACE_DEFAULT_ASYNCH_BACKLOG,
                         int reuse_addr = 1,
                         ACE_Proactor* proactor = 0,
                         bool validate_new_connection = false,
                         int reissue_accept = 1,
                         int number_of_initial_accepts = -1);

        virtual UpdateService *make_handler();

        int get_statistics(UpdateStatsMap& stats_map) const;

        int get_file_buffer(ACE_CString const& filename, ACE_Message_Block *& output_buffer);

        int increase_active_downloads(char update_name);
        int decrease_active_downloads(char update_name);
        int increase_complete_downloads(char update_name);

    private:
        Configuration config;
        UpdateStatsMap statistics;
        mutable ACE_Recursive_Thread_Mutex statistics_mutex;

        typedef std::map<ACE_CString, ACE_Message_Block*> FilesMap;
        FilesMap files;
        ACE_Recursive_Thread_Mutex file_mutex;

        int load_file_buffer(ACE_CString const& filename, ACE_Message_Block *& output_buffer);
};

} // namespace SclUpdate

#endif // SCLUPDATE_UPDATE_MANAGER_H
