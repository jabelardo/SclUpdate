#include "TaskBarIcon.h"
#include "MainDialog.h"
#include "Server.h"
#include <wx/menu.h>

TaskBarIcon::TaskBarIcon(MainDialog& mainDialog, Server& server)
    : mainDialog(mainDialog)
    , server(server)
{
}

TaskBarIcon::~TaskBarIcon()
{
}

void
TaskBarIcon::OnMenuRestore(wxCommandEvent& )
{
    mainDialog.Show(true);
}

void
TaskBarIcon::OnMenuHide(wxCommandEvent& )
{
    mainDialog.Show(false);
}

void
TaskBarIcon::OnMenuExit(wxCommandEvent& )
{
    mainDialog.Close(true);
}

void
TaskBarIcon::OnMenuStart(wxCommandEvent& )
{
//    server.Start();
}

void
TaskBarIcon::OnMenuStop(wxCommandEvent& )
{
//    server.Stop();
}

void
TaskBarIcon::OnLeftMouse(wxTaskBarIconEvent&)
{
    if (mainDialog.IsShown()) {
        mainDialog.Show(false);
    } else {
        mainDialog.Show(true);
    }
}

wxMenu*
TaskBarIcon::CreatePopupMenu()
{
    wxMenu* menu = new wxMenu;
//    if (server.IsRunning()) {
//        menu->Append(PU_STOP, _T("&Detener servidor"));
//    } else {
//        menu->Append(PU_START, _T("&Iniciar servidor"));
//    }
//    menu->AppendSeparator();
    if (mainDialog.IsShown()) {
        menu->Append(PU_HIDE, _T("&Minimizar"));
    } else {
        menu->Append(PU_RESTORE, _T("&Restaurar"));
    }
    menu->AppendSeparator();
    menu->Append(PU_EXIT, _T("&Cerrar"));
    return menu;
}

BEGIN_EVENT_TABLE(TaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, TaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_HIDE,    TaskBarIcon::OnMenuHide)
    EVT_MENU(PU_EXIT,    TaskBarIcon::OnMenuExit)
    EVT_MENU(PU_STOP,    TaskBarIcon::OnMenuStop)
    EVT_MENU(PU_START,   TaskBarIcon::OnMenuStart)
    EVT_TASKBAR_LEFT_DOWN(TaskBarIcon::OnLeftMouse)
END_EVENT_TABLE()
