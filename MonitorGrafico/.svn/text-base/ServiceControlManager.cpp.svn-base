#include "ServiceControlManager.h"
#include <stdexcept>
#include <sstream>
#include <windows.h>

ServiceControlManager::ServiceControlManager(std::string const& name)
    : name(name)
{
}

ServiceControlManager::~ServiceControlManager()
{
}

namespace
{

class ServiceControlCommand
{
    SC_HANDLE manager;

protected:
    SC_HANDLE service;

public:
    ServiceControlCommand(std::string const& name, DWORD access)
    {
        manager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
        if (!manager) {
            throw std::runtime_error(ErrorMessage(GetLastError()));
        }

        service = OpenService(manager, name.c_str(), access);
        if (!service) {
            CloseServiceHandle(manager);
            throw std::runtime_error(ErrorMessage(GetLastError()));
        }
    }

    virtual ~ServiceControlCommand()
    {
        CloseServiceHandle(service);
        CloseServiceHandle(manager);
    }

    static void
    SleepWaitHint(DWORD waitHint)
    {
        DWORD waitTime = waitHint / 10;
        if ( waitTime < 5000 )
            waitTime = 5000;
        else if ( waitTime > 10000 )
            waitTime = 10000;
        Sleep(waitTime);
    }

    static std::string
    ErrorMessage(DWORD dwErr)
    {
        LPVOID lpMsgBuf;
        std::ostringstream oss;
        try {
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          0, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, 0);

            oss << (LPCTSTR)lpMsgBuf;
        } catch (...) {}

        LocalFree(lpMsgBuf);
        return  oss.str();
    }

    SERVICE_STATUS_PROCESS
    GetServiceStatus()
    {
        SERVICE_STATUS_PROCESS status;
        DWORD bytesNeeded;
        if (!QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO,
            reinterpret_cast<LPBYTE>(&status), sizeof(status), &bytesNeeded)) {
            throw std::runtime_error(ErrorMessage(GetLastError()));
        }
        return status;
    }

    void
    WaitForStatusChange(DWORD statusToChange)
    {
        SERVICE_STATUS_PROCESS status = GetServiceStatus();
        DWORD startTickCount = GetTickCount();
        DWORD oldCheckPoint = status.dwCheckPoint;

        while (status.dwCurrentState == statusToChange)  {

            SleepWaitHint(status.dwWaitHint);
            status = GetServiceStatus();
            if (status.dwCurrentState != statusToChange) {
                break;
            }
            if (status.dwCheckPoint > oldCheckPoint) {
                startTickCount = GetTickCount();
                oldCheckPoint = status.dwCheckPoint;

            } else if (GetTickCount() - startTickCount > status.dwWaitHint) {
                throw std::runtime_error("Tiempo de espera agotado para el cambio de estado del servicio");
            }
        }
    }

    virtual DWORD Execute() = 0;
};


struct StartCommand : public ServiceControlCommand
{
    StartCommand(std::string const& name) : ServiceControlCommand(name, SERVICE_ALL_ACCESS) {}

    virtual ~StartCommand() {}

    virtual DWORD Execute()
    {
        SERVICE_STATUS_PROCESS status = GetServiceStatus();
        if (status.dwCurrentState != SERVICE_STOPPED && status.dwCurrentState != SERVICE_STOP_PENDING) {
            return -1;
        }
        if (status.dwCurrentState == SERVICE_STOP_PENDING) {
            WaitForStatusChange(SERVICE_STOP_PENDING);
        }
        if (!StartService(service, 0, 0)) {
            throw std::runtime_error(ErrorMessage(GetLastError()));
        }

        WaitForStatusChange(SERVICE_START_PENDING);

        return 0;
    }
};

struct StopCommand : public ServiceControlCommand
{
    StopCommand(std::string const& name) : ServiceControlCommand(name, SERVICE_ALL_ACCESS) {}

    virtual ~StopCommand() {}

    virtual DWORD Execute()
    {

        SERVICE_STATUS_PROCESS status = GetServiceStatus();
        if (status.dwCurrentState == SERVICE_STOPPED) {
            return -1;
        }

        if (status.dwCurrentState == SERVICE_STOP_PENDING) {
            WaitForStatusChange(SERVICE_STOP_PENDING);
            return -1;
        }

        SERVICE_STATUS serviceStatus;
        if (!ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus)) {
            throw std::runtime_error(ErrorMessage(GetLastError()));
        }

        DWORD timeout = 30000;
        DWORD startTime = GetTickCount();
        do {
            SERVICE_STATUS_PROCESS status = GetServiceStatus();
            if (status.dwCurrentState == SERVICE_STOPPED) {
                break;
            }
            Sleep(status.dwWaitHint);
            if (GetTickCount() - startTime > timeout) {
                throw std::runtime_error("Tiempo de espera agotado para el cambio de estado del servicio");
            }
        } while (status.dwCurrentState != SERVICE_STOPPED);

        return 0;
    }
};

}

void
ServiceControlManager::Start() const
{
    StartCommand command(name);
    command.Execute();
}

void
ServiceControlManager::Stop() const
{
    StopCommand command(name);
    command.Execute();
}
