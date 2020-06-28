#ifndef MONITORGRAFICO_SERVICECONTROLMANAGER_H
#define MONITORGRAFICO_SERVICECONTROLMANAGER_H

#include <string>

class ServiceControlManager
{
    public:
        ServiceControlManager(std::string const& name);
        ~ServiceControlManager();

        void Start() const;
        void Stop() const;

        enum ServiceStatus {
              Running
            , Stopped
            , Paused
            , PausePending
            , ContinuePending
            , StartPending
            , StopPending
            , Unknown
        };

    private:
        std::string const name;
};

#endif // MONITORGRAFICO_SERVICECONTROLMANAGER_H
