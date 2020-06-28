#include "update_manager.h"
#include <ace/OS.h>

namespace SclUpdate
{

using namespace std;

UpdateManager::UpdateManager(Configuration const& config)
    : config(config)
{
}

UpdateManager::~UpdateManager()
{
    for (FilesMap::iterator i =  files.begin(); i != files.end(); ++i) {
        i->second->release();
    }
}

int
UpdateManager::open(const ACE_INET_Addr& address, size_t bytes_to_read, bool pass_addresses, int backlog,
                    int reuse_addr, ACE_Proactor* proactor, bool validate_new_connection,
                    int reissue_accept, int number_of_initial_accepts)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, statistics_mutex, -1);

    ACE_CString update_names = config.get_update_names();
    for (size_t i = 0; i < update_names.length(); ++i) {
        UpdateStats stats(update_names[i]);
        statistics.insert(make_pair(update_names[i], stats));
    }

    return Super::open(address, bytes_to_read, pass_addresses, backlog, reuse_addr, proactor,
                       validate_new_connection, reissue_accept, number_of_initial_accepts);

}

int
UpdateManager::get_statistics(UpdateStatsMap& stats_map) const
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, statistics_mutex, -1);
    stats_map = statistics;
    return 0;
}

UpdateService*
UpdateManager::make_handler()
{
    UpdateService* result = Super::make_handler();
    if (result != 0) {
        result->set_config(config);
        result->set_update_manager(this);
    }
    return result;
}

int
UpdateManager::increase_active_downloads(char update_name)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, statistics_mutex, -1);

    UpdateStatsMap::iterator i =  statistics.find(update_name);
    if (i == statistics.end()) return -1;
    i->second.increase_active_downloads();
    return 0;
}

int
UpdateManager::decrease_active_downloads(char update_name)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, statistics_mutex, -1);

    UpdateStatsMap::iterator i =  statistics.find(update_name);
    if (i == statistics.end()) return -1;
    i->second.decrease_active_downloads();
    return 0;
}

int
UpdateManager::increase_complete_downloads(char update_name)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, statistics_mutex, -1);

    UpdateStatsMap::iterator i =  statistics.find(update_name);
    if (i == statistics.end()) return -1;
    i->second.increase_complete_downloads();
    return 0;
}

int
UpdateManager::get_file_buffer(ACE_CString const& filename, ACE_Message_Block *& output_buffer)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, file_mutex, -1);

    FilesMap::iterator i =  files.find(filename);
    if (i == files.end()) {
        return load_file_buffer(filename, output_buffer);
    }
    output_buffer = i->second->duplicate();
    return 0;
}

int
UpdateManager::load_file_buffer(ACE_CString const& filename, ACE_Message_Block *& output_buffer)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mon, file_mutex, -1);

    ACE_HANDLE fd = ACE_OS::open(filename.c_str(), O_RDONLY);
    if (fd == ACE_INVALID_HANDLE) {
        ACE_ERROR_RETURN((LM_ERROR,
                          "%T %P %t | No se pudo abrir el archivo %s para actualizar cliente %m\n",
                          filename.c_str()),
                          -1);
        return -1;
    }
    size_t fsize = ACE_OS::filesize(fd);
    ACE_NEW_RETURN(output_buffer, ACE_Message_Block(fsize), -1);
    ssize_t bytes;
    while ((bytes = ACE_OS::read(fd, output_buffer->wr_ptr(), fsize)) > 0) {
        output_buffer->wr_ptr(bytes);
    }
    ACE_OS::close(fd);
    if (output_buffer->length() != fsize) {
        output_buffer->release();
        return -1;
    }
    files.insert(make_pair(filename, output_buffer->duplicate()));
    return 0;
}

} // namespace SclUpdate
