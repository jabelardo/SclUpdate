#ifndef MONITORGRAFICO_SERVER_H
#define MONITORGRAFICO_SERVER_H

#include <wx/socket.h>
#include <wx/timer.h>
#include <wx/buffer.h>

#include <MonitorGrafico/Configuration.h>
#include <MonitorGrafico/UpdateStats.h>
#include <MonitorGrafico/ServiceControlManager.h>

class MainDialog;

class Server : public wxEvtHandler
{
    public:
        Server(MainDialog& mainDialog);
        virtual ~Server();

        bool TryConnection();
        void CloseConnection();
        bool IsConnected() const;

        void SetConfiguration(Configuration const& config);

        void StartService();
        void StopService();

    private:

        enum {
              SOCKET_ID  = 10001
            , TIMER_ID
        };

        ServiceControlManager service;
        Configuration config;
        wxSocketClient* socket;
        wxTimer* timer;

        enum ReadStatus {
              ReadIdle = 1
            , ReadSize
            , ReadPaiload
        };
        ReadStatus readStatus;
        wxMemoryBuffer inBuffer;
        wxUint16 msgSize;
        MainDialog& mainDialog;

        static wxUint32 const SIZEOF_MSG_SIZE = 2; // longitud del campo size

        void ConfigureSocket();
        void RequestConnection();
        void OnSocketEvent(wxSocketEvent& event);
        void OnSocketInput();
        void OnReadSize();
        void OnReadPaiload();
        bool Read(wxUint32 bytesToRead);
        void OnSocketLost();
        void OnSocketConnection();
        bool Write(void const* buffer, wxUint32 nbytes);

        void OnTimerEvent(wxTimerEvent& event);
        void RequestUpdate();

        DECLARE_EVENT_TABLE()
};

#endif // MONITORGRAFICO_SERVER_H
