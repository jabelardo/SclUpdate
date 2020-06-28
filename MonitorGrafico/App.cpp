/***************************************************************
 * Name:      App.cpp
 * Purpose:   Code for Application Class
 * Author:    Jose Abelardo Gutierrez (jabelardo@gmail.com)
 * Created:   2009-12-13
 * Copyright: Jose Abelardo Gutierrez ()
 * License:
 **************************************************************/

#include "App.h"
#include "MainDialog.h"
#include <wx/image.h>
#include <wx/msgdlg.h>
#include "TaskBarIcon.h"
#include "Server.h"
#include "download.xpm"

IMPLEMENT_APP(App);

bool
App::OnInit()
{
    wxInitAllImageHandlers();

    if (!config.ReadFile(wxT("MonitorGrafico.conf"))) {
        wxMessageBox(wxT("No se pudo cargar el archivo MonitorGrafico.conf"), wxT("Error"));
        return false;
    }

    dialog = new MainDialog(0);
    dialog->SetConfiguration(config);
    SetTopWindow(dialog);
    dialog->Show(true);

    return true;
}

int
App::OnExit()
{
    return wxApp::OnExit();
}
