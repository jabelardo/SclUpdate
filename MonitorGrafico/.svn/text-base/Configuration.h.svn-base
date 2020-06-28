#ifndef MONITORGRAFICO_CONFIGURATION_H
#define MONITORGRAFICO_CONFIGURATION_H

#include <wx/string.h>
#include <wx/socket.h>

class Configuration
{
public:
    Configuration();

    bool ReadFile(wxString const& configFile);

    wxIPV4address GetAddress() const;
    int GetReloadTime() const;

private:
    wxIPV4address address;
    int reloadTime;
};


#endif // MONITORGRAFICO_CONFIGURATION_H
