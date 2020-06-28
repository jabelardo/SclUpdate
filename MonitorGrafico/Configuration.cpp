#include "Configuration.h"
#include "GetPot"
#include <fstream>

Configuration::Configuration()
    : address()
    , reloadTime(10)
{
    address.LocalHost();
}

wxIPV4address
Configuration::GetAddress() const
{
    return address;
}

int
Configuration::GetReloadTime() const
{
    return reloadTime;
}

bool
Configuration::ReadFile(wxString const& configFile)
{
    std::ifstream file;
    file.open(configFile.c_str());
    if (!file) {
        return false;
    }
    GetPot config(configFile.mb_str(wxConvUTF8));

    std::string service(config("CONFIG/PUERTO", "7890"));
    address.Service(wxString(service.c_str(), wxConvUTF8));

    reloadTime = config("CONFIG/TIEMPO_RECARGA", 5)  * 1000;

    return true;
}
