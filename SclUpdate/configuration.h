#ifndef SCLUPDATE_CONFIGURATION_H
#define SCLUPDATE_CONFIGURATION_H

#include <ace/SString.h>
#include <ace/INET_Addr.h>
#include <map>

namespace SclUpdate
{

class Configuration
{
public:
    Configuration();
    Configuration(Configuration const& config);
    Configuration& operator=(Configuration const& config);

    ~Configuration();

    bool read_file(ACE_CString const& config_file);

    ACE_INET_Addr get_update_port() const;
    ACE_INET_Addr get_monitor_port() const;
    ACE_CString get_update_file(char update_name) const;

    ACE_CString get_update_names() const;

private:
    ACE_INET_Addr update_port;
    ACE_INET_Addr monitor_port;

    typedef std::map<char, ACE_CString> UpdatesMap;

    UpdatesMap updates;
};

} // namespace SclUpdate

#endif // SCLUPDATE_CONFIGURATION_H
