/***************************************************************
 * Name:      MainDialog.cpp
 * Purpose:   Code for Application Frame
 * Author:    Jose Abelardo Gutierrez (jabelardo@gmail.com)
 * Created:   2009-12-13
 * Copyright: Jose Abelardo Gutierrez ()
 * License:
 **************************************************************/

#include "MainDialog.h"
#include <wx/msgdlg.h>
#include <wx/app.h>
#include <wx/choicdlg.h>
#include "TaskBarIcon.h"
#include "Server.h"
#include "download.xpm"

//(*InternalHeaders(MainDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(MainDialog)
const long MainDialog::ID_STATS_GRID = wxNewId();
const long MainDialog::ID_STATUS_TEXT = wxNewId();
const long MainDialog::ID_BUTTON_ABOUT = wxNewId();
const long MainDialog::ID_BUTTON_SERVICE = wxNewId();
const long MainDialog::ID_BUTTON_QUIT = wxNewId();
//*)

BEGIN_EVENT_TABLE(MainDialog,wxDialog)
    //(*EventTable(MainDialog)
    //*)
     EVT_UPDATE_UI(ID_STATUS_TEXT, MainDialog::OnUpdateUIStatus)
END_EVENT_TABLE()

MainDialog::MainDialog(wxWindow* parent, wxWindowID id)
    : blockTryConnection(false)
{
    //(*Initialize(MainDialog)
    wxStaticBoxSizer* StaticBoxSizer1;

    Create(parent, wxID_ANY, _("Monitor de Actualizaciones SCL Plus"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
    SetClientSize(wxSize(640,400));
    SetMinSize(wxSize(-1,-1));
    SetMaxSize(wxSize(-1,-1));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Estadísticas de Descargas"));
    statsGrid = new wxGrid(this, ID_STATS_GRID, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATS_GRID"));
    statsGrid->SetMinSize(wxSize(370,230));
    statsGrid->SetMaxSize(wxSize(-1,-1));
    StaticBoxSizer1->Add(statsGrid, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    statusText = new wxStaticText(this, ID_STATUS_TEXT, _("Estatus"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATUS_TEXT"));
    BoxSizer2->Add(statusText, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    aboutButton = new wxButton(this, ID_BUTTON_ABOUT, _("Acerca de"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_ABOUT"));
    BoxSizer3->Add(aboutButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_TOP, 4);
    serviceButton = new wxButton(this, ID_BUTTON_SERVICE, _("Detener Servicio"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SERVICE"));
    BoxSizer3->Add(serviceButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_TOP, 4);
    BoxSizer3->Add(-1,-1,1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    quitButton = new wxButton(this, ID_BUTTON_QUIT, _("Salir"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_QUIT"));
    BoxSizer3->Add(quitButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_BOTTOM, 4);
    BoxSizer1->Add(BoxSizer3, 0, wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTON_ABOUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDialog::OnAbout);
    Connect(ID_BUTTON_SERVICE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDialog::OnServiceButtonClick);
    Connect(ID_BUTTON_QUIT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDialog::OnQuitButtonClick);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MainDialog::OnClose);
    //*)

    statsGrid->CreateGrid(1, 3);
    statsGrid->SetDefaultRowSize(25);
    statsGrid->SetDefaultColSize(95);
    statsGrid->SetColLabelValue(0, wxT("Versión"));
    statsGrid->SetColLabelValue(1, wxT("En Progreso"));
    statsGrid->SetColLabelValue(2, wxT("Completadas"));
    statsGrid->DisableCellEditControl();
    statsGrid->DisableDragGridSize();
    statsGrid->DisableDragColSize();
    statsGrid->DisableDragRowSize();
    statsGrid->EnableEditing(false);
    statsGrid->DeleteRows(0, statsGrid->GetNumberRows());

    server = new Server(*this);
    taskBarIcon = new TaskBarIcon(*this, *server);
    taskBarIcon->SetIcon(wxIcon(download_xpm), _("Monitor de Acualzaciones de SCL Plus"));
}

MainDialog::~MainDialog()
{
    //(*Destroy(MainDialog)
    //*)
    delete taskBarIcon;
    delete server;
}

void
MainDialog::NotifyChange(UpdateStatsMap const& stats)
{
    statsGrid->DeleteRows(0, statsGrid->GetNumberRows());
    int row = 0;
    for (UpdateStatsMap::const_iterator i = stats.begin(); i != stats.end(); ++i, ++row) {
        statsGrid->AppendRows();
        statsGrid->SetCellAlignment(wxALIGN_CENTRE, row, 0);
        statsGrid->SetCellAlignment(wxALIGN_RIGHT, row, 1);
        statsGrid->SetCellAlignment(wxALIGN_RIGHT, row, 2);
        statsGrid->SetCellValue(i->second.GetUpdateName(), row, 0);
        statsGrid->SetCellValue(i->second.GetActiveDownloads(), row, 1);
        statsGrid->SetCellValue(i->second.GetCompleteDownloads(), row, 2);
    }
}

void
MainDialog::SetConfiguration(Configuration const& config)
{
    this->config = config;
    server->SetConfiguration(config);
}

void
MainDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = _("Monitor gráfico del servicio de\nactualizaciones del SCL Plus.\n\n    Versión 1.0.0");
    wxMessageBox(msg, _("Acerca de ..."));
}

void
MainDialog::OnClose(wxCloseEvent& event)
{
    if (event.CanVeto()) {
        Hide();
        event.Veto();
    } else {
        Destroy();
    }
}

void
MainDialog::OnQuitButtonClick(wxCommandEvent& event)
{
    Destroy();
}

void
MainDialog::TryConnection()
{
    if (!server->TryConnection()) {
        blockTryConnection = true;
        wxString choices[] = {_("Iniciar Servicio"), _("Reintentar"), _("Salir")};
        wxSingleChoiceDialog dialog(this,
                                    _("No se pudo conectar con el servicio"),
                                    _("Adevertencia"),
                                    3, choices, 0,
                                    wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxOK | wxCENTRE);
        dialog.ShowModal();

        switch (dialog.GetSelection()) {
            case 0:
               server->StartService();
            case 1:
               blockTryConnection = false;
               TryConnection();
               break;
            case 2:
            default:
               Destroy();
               return;
        }
    }
}

void
MainDialog::OnUpdateUIStatus(wxUpdateUIEvent& event)
{
    wxString newStatusLabel = server->IsConnected() ? _("Conectado") : _("Desconectado");
    if (newStatusLabel != statusText->GetLabel()) {
        statusText->SetLabel(newStatusLabel);
    }

    serviceButton->Enable(server->IsConnected());
    quitButton->Enable(!blockTryConnection);

    if (!server->IsConnected()) {
        if (!blockTryConnection) {
            TryConnection();
        }
    }
}

void
MainDialog::OnServiceButtonClick(wxCommandEvent& event)
{
    server->StopService();

    wxMessageDialog dlg(this, wxT("El servicio fue detenido.\n ¿Desea salir de la aplicación?"),
                        wxT("¿Salir?"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);

    if (dlg.ShowModal() == wxID_YES) {
        blockTryConnection = true;
        wxCloseEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_QUIT);
        event.SetCanVeto(false);
        event.SetEventObject(this);
        GetEventHandler()->ProcessEvent(event);
    }
}
