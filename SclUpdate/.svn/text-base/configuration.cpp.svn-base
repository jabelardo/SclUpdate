#include "configuration.h"
#include "GetPot"
#include <fstream>
#include <ace/Log_Msg.h>
#include <ace/OS.h>

namespace SclUpdate
{

using namespace std;

Configuration::Configuration()
    : update_port()
    , monitor_port()
    , updates()
{
}

Configuration::Configuration(Configuration const& config)
    : update_port(config.update_port)
    , monitor_port(config.monitor_port)
    , updates(config.updates)
{
}

Configuration&
Configuration::operator=(Configuration const& config)
{
    if (&config == this) return *this;
    update_port = config.update_port;
    monitor_port = config.monitor_port;
    updates = config.updates;
    return *this;
}

Configuration::~Configuration()
{
}

ACE_CString
Configuration::get_update_file(char update_name) const
{
    UpdatesMap::const_iterator i = updates.find(update_name);
    if (i != updates.end()) {
        return i->second;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | archivo no encontrado %c\n", update_name));
    return "";
}

bool
Configuration::read_file(ACE_CString const& config_file)
{
    std::ifstream file;
    ACE_DEBUG((LM_DEBUG, "%T %P %t | cargando archivo %s\n", config_file.c_str()));

    file.open(config_file.c_str());
    if (!file) {
        return false;
    }
    updates.clear();

    GetPot config(config_file.c_str());

    update_port.set(ACE_INET_Addr(config("PUERTOS/ACTUALIZACION", 789)));
    monitor_port.set(ACE_INET_Addr(config("PUERTOS/MONITOR", 7890)));

    STRING_VECTOR sections = config.get_section_names();
    char const* act_tag = "ACTUALIZACION_";

    for (STRING_VECTOR::const_iterator i = sections.begin(); i != sections.end(); ++i) {

        if (i->find(act_tag) != 0) continue;

        char const* update_name_pos = i->c_str() + strlen(act_tag);
        if (*update_name_pos == '\0') continue;

        char update_name = *update_name_pos;

        config.set_prefix(i->c_str());

        ACE_CString archivo(config("ARCHIVO", "").c_str());

        if (archivo.length() == 0) continue;

        updates.insert(make_pair(update_name, archivo.substr(1, archivo.length() - 2)));
    }
     ACE_DEBUG((LM_DEBUG, "%T %P %t | cargando archivo %s return true\n", config_file.c_str()));

    return true;
}

ACE_INET_Addr
Configuration::get_update_port() const
{
    return update_port;
}

ACE_INET_Addr
Configuration::get_monitor_port() const
{
    return monitor_port;
}

ACE_CString
Configuration::get_update_names() const
{
    ACE_CString result(updates.size());

    for (UpdatesMap::const_iterator i = updates.begin(); i != updates.end(); ++i) {
        result += i->first;
    }
    return result;
}

} // namespace SclUpdate
