/***************************************************************
 * Name:      MainDialog.h
 * Purpose:   Defines Application Frame
 * Author:    Jose Abelardo Gutierrez (jabelardo@gmail.com)
 * Created:   2009-12-13
 * Copyright: Jose Abelardo Gutierrez ()
 * License:
 **************************************************************/

#ifndef MONITORGRAFICO_MAINDIALOG_H
#define MONITORGRAFICO_MAINDIALOG_H

//(*Headers(MonitorGraficoDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/scrolwin.h>
#include <wx/grid.h>
//*)

#include <MonitorGrafico/Configuration.h>
#include <MonitorGrafico/UpdateStats.h>

class TaskBarIcon;
class Server;

class MainDialog: public wxDialog
{
    public:

        MainDialog(wxWindow* parent, wxWindowID id = -1);
        virtual ~MainDialog();

        void SetConfiguration(Configuration const& config);

        void NotifyChange(UpdateStatsMap const& stats);

    private:

        //(*Handlers(MainDialog)
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuitButtonClick(wxCommandEvent& event);
        void OnServiceButtonClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(MainDialog)
        static const long ID_STATS_GRID;
        static const long ID_STATUS_TEXT;
        static const long ID_BUTTON_ABOUT;
        static const long ID_BUTTON_SERVICE;
        static const long ID_BUTTON_QUIT;
        //*)

        //(*Declarations(MainDialog)
        wxStaticText* statusText;
        wxBoxSizer* BoxSizer2;
        wxButton* quitButton;
        wxGrid* statsGrid;
        wxButton* aboutButton;
        wxBoxSizer* BoxSizer1;
        wxBoxSizer* BoxSizer3;
        wxButton* serviceButton;
        //*)

        TaskBarIcon* taskBarIcon;
        Server* server;
        Configuration config;
        bool blockTryConnection;

        void OnUpdateUIStatus(wxUpdateUIEvent& event);
        void TryConnection();

        DECLARE_EVENT_TABLE()
};

#endif // MONITORGRAFICO_MAINDIALOG_H
