/***************************************************************
 * Name:      App.h
 * Purpose:   Defines Application Class
 * Author:    Jose Abelardo Gutierrez (jabelardo@gmail.com)
 * Created:   2009-12-13
 * Copyright: Jose Abelardo Gutierrez ()
 * License:
 **************************************************************/

#ifndef MONITORGRAFICO_APP_H
#define MONITORGRAFICO_APP_H

#include <wx/app.h>
#include <MonitorGrafico/Configuration.h>

class MainDialog;

class App : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();

    private:
        MainDialog* dialog;
        Configuration config;
};

#endif // MONITORGRAFICO_APP_H
