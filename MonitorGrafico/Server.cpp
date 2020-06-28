#include "Server.h"
#include "MonitorProtocol.h"
#include "MainDialog.h"
#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <stdexcept>

Server::Server(MainDialog& mainDialog)
    : service("SclUpdate")
    , config()
    , socket(0)
    , timer(new wxTimer(this, TIMER_ID))
    , readStatus(ReadIdle)
    , inBuffer()
    , msgSize(0)
    , mainDialog(mainDialog)
{
}

Server::~Server()
{
    CloseConnection();
    delete timer;
}

void
Server::SetConfiguration(Configuration const& config)
{
    this->config = config;
    timer->Start(config.GetReloadTime());
}

/** @brief Start
  *
  * @todo: document this function
  */
void
Server::StartService()
{
    try {
        service.Start();
    } catch (std::runtime_error const& e) {
        wxMessageBox(wxString(e.what(), wxConvUTF8), wxT("Error"));
    }
}

/** @brief Stop
  *
  * @todo: document this function
  */
void
Server::StopService()
{
    try {
        service.Stop();
    } catch (std::runtime_error const& e) {
        wxMessageBox(wxString(e.what(), wxConvUTF8), wxT("Error"));
    }
}

void
Server::CloseConnection()
{
    if (socket != 0) {
        socket->Notify(false);
        socket->Close();
        socket->Destroy();
        socket = 0;
    }
}

void
Server::ConfigureSocket()
{
    CloseConnection();
    socket = new wxSocketClient;
    socket->SetEventHandler(*this, SOCKET_ID);
    socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    socket->Notify(true);
}

void
Server::RequestConnection()
{
    ConfigureSocket();
    socket->Connect(config.GetAddress(), false);
}

bool
Server::IsConnected() const
{
    return (socket == 0) ? false : socket->IsConnected();
}

void
Server::OnSocketEvent(wxSocketEvent& event)
{
    if (event.GetSocket() == socket) {
        switch (event.GetSocketEvent()) {
            case wxSOCKET_INPUT      : OnSocketInput(); break;
            case wxSOCKET_LOST       : OnSocketLost(); break;
            case wxSOCKET_CONNECTION : OnSocketConnection(); break;
            default                  : break;
        }
    }
}

/** @brief OnSocketInput
  *
  * @todo: document this function
  */
void
Server::OnSocketInput()
{
    if (readStatus == ReadIdle || readStatus == ReadSize) {
        OnReadSize();
    } else {
        OnReadPaiload();
    }
}

void
Server::OnReadSize()
{
    if (readStatus == ReadIdle) {
        readStatus = ReadSize;
        inBuffer.SetDataLen(0);
        msgSize = 0;
    }

    wxUint32 bytesToRead = SIZEOF_MSG_SIZE - inBuffer.GetDataLen();

    if (!Read(bytesToRead)) {
        return;
    }

    if (inBuffer.GetDataLen() == SIZEOF_MSG_SIZE) {
        char* bufferData = reinterpret_cast<char*>(inBuffer.GetData());
        msgSize = *reinterpret_cast<wxUint16*>(bufferData);
        inBuffer.SetBufSize(msgSize + SIZEOF_MSG_SIZE);
        readStatus = ReadPaiload;
    }
}

bool
Server::Read(wxUint32 bytesToRead)
{
    socket->Read(inBuffer.GetAppendBuf(bytesToRead), bytesToRead);

    if (socket->Error()) {
        readStatus = ReadIdle;
        return false;
    }

    wxUint32 bytesRead = socket->LastCount();

    if (bytesRead != 0) {
        inBuffer.UngetAppendBuf(bytesRead);
    }
    return true;
}

void
Server::OnReadPaiload()
{
    wxUint32 bytesToRead = msgSize + SIZEOF_MSG_SIZE - inBuffer.GetDataLen();

    if (!Read(bytesToRead)) {
        return;
    }

    if (inBuffer.GetDataLen() == msgSize + SIZEOF_MSG_SIZE) {
        UpdateStatsMap stats;
        int result = Unserialize(inBuffer, stats);
        if (result == 0) {
            mainDialog.NotifyChange(stats);
        } else {
            wxLogError(L"Error de comunicación %i", result);
        }
        readStatus = ReadIdle;
    }
}

/** @brief OnSocketLost
  *
  * @todo: document this function
  */
void
Server::OnSocketLost()
{
    CloseConnection();
}

/** @brief OnSocketConnection
  *
  * @todo: document this function
  */
void
Server::OnSocketConnection()
{
    socket->WaitOnConnect();
}

bool
Server::Write(void const* buffer, wxUint32 nbytes)
{
    if (socket == 0) return false;
    wxSocketFlags oldFlags = socket->GetFlags();
    socket->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);
    socket->Write(buffer, nbytes);
    socket->SetFlags(oldFlags);
    return socket->Error();
}

bool
Server::TryConnection()
{
    if (!IsConnected()) {
        RequestConnection();
    }
    for (int i = 0; i < 10 && !IsConnected(); ++i) {
        wxTheApp->Yield(true);
        wxMilliSleep(100);
    }
    return IsConnected();
}

void
Server::OnTimerEvent(wxTimerEvent& event)
{
    if (IsConnected() && readStatus == ReadIdle) {
        RequestUpdate();
    }
}

void
Server::RequestUpdate()
{
    char updateID = '1';
    Write(&updateID, sizeof(updateID));
}

BEGIN_EVENT_TABLE(Server, wxEvtHandler)
    EVT_SOCKET(SOCKET_ID,   Server::OnSocketEvent)
    EVT_TIMER(TIMER_ID,     Server::OnTimerEvent)
END_EVENT_TABLE()
