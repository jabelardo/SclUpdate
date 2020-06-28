#ifndef MONITORGRAFICO_TASKBARICON_H
#define MONITORGRAFICO_TASKBARICON_H

#include <wx/taskbar.h>

class MainDialog;
class Server;

class TaskBarIcon : public wxTaskBarIcon
{
    public:
        TaskBarIcon(MainDialog& mainDialog, Server& server);
        virtual ~TaskBarIcon();
        virtual wxMenu* CreatePopupMenu();

    protected:

    private:
        MainDialog& mainDialog;
        Server& server;

        enum {
              PU_RESTORE = 10001
            , PU_HIDE
            , PU_EXIT
            , PU_STOP
            , PU_START
        };

        void OnMenuRestore(wxCommandEvent&);
        void OnMenuHide(wxCommandEvent&);
        void OnMenuExit(wxCommandEvent&);
        void OnMenuStop(wxCommandEvent&);
        void OnMenuStart(wxCommandEvent&);
        void OnLeftMouse(wxTaskBarIconEvent&);

        DECLARE_EVENT_TABLE()
};

#endif // MONITORGRAFICO_TASKBARICON_H
