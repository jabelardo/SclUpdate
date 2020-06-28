#include "app.h"
#include "nt_service.h"

#include <ace/Get_Opt.h>
#include <ace/streams.h>

// Default for the -i (install) option
#define DEFAULT_SERVICE_INIT_STARTUP     SERVICE_AUTO_START

namespace SclUpdate
{

NtServiceImpl* AppImpl::nt_service;

AppImpl::AppImpl()
    : startup_opt(0)
    , option(Run)
{
    ACE::init();
    nt_service = NtService::instance();
    ACE_DEBUG((LM_DEBUG, "%T %P %t | AppImpl creado\n"));
}

AppImpl::~AppImpl()
{
    if (nt_service != 0) {
        delete nt_service;
        nt_service = 0;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | AppImpl destruido\n"));
    ACE::fini();
}

int
AppImpl::init_path()
{
    ::GetModuleFileName(GetModuleHandle(0), path, MAX_PATH);

    ACE_TCHAR* ptr = path + ACE_OS::strlen(path);
    while (*ptr != TEXT('/') && *ptr != TEXT('\\') && *ptr != TEXT(':')) {
        --ptr;
        if (ptr == path) {
            break;
        }
    }
    *(++ptr) = '\0';
    ACE_OS::chdir(path);
    return 0;
}

int
AppImpl::run(int argc, ACE_TCHAR* argv[])
{
    if (init_path() != 0) {
    }

    nt_service->name(NtServiceImpl::NAME, NtServiceImpl::DESCRIPTION);

    if (nt_service->load_config() != 0) {
        ACE_ERROR_RETURN((LM_ERROR, "%T %P %t | %p\n", "NtServiceImpl load_config"), 300);
    }

    parse_args(argc, argv);
    return exec_option();
}

int
AppImpl::exec_option()
{
    switch (option) {
        case Help:      return help();
        case Install:   return install();
        case Remove:    return remove();
        case Start:     return start();
        case Kill:      return kill();
        case SetType:   return set_type();
        case Debug:     return debug();

        case Run:
        default:
            break;
    }
    return run();
}

int
AppImpl::run()
{
    ACE_CString logfile = ACE_CString(NtServiceImpl::NAME) + ACE_CString(".log");
    ofstream* output_file = new ofstream(logfile.c_str(), ios::out);
    if (output_file && output_file->rdstate() == ios::goodbit) {
        ACE_LOG_MSG->msg_ostream(output_file, true);
    }
    ACE_LOG_MSG->open(NtServiceImpl::NAME, ACE_Log_Msg::STDERR | ACE_Log_Msg::OSTREAM, 0);
    ACE_DEBUG((LM_DEBUG, "%T %P %t | Iniciando servicio.\n"));

    if (0 == nt_service->run()) {
        ACE_ERROR_RETURN((LM_ERROR, "%T %P %t | %p\n", "No se pudo iniciar el servicio"), 1);
    }

    ACE_DEBUG((LM_DEBUG, "%T %P %t | Servicio iniciado.\n"));
    return 0;

}

/** @brief install
  *
  * @todo: document this function
  */
int
AppImpl::install()
{
    if (-1 == nt_service->insert(startup_opt)) {
        ACE_ERROR((LM_ERROR, "%T %P %t | %p\n", "Instalar"));
        return -1;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | Instalar realizado\n"));
    return 0;
}

/** @brief remove
  *
  * @todo: document this function
  */
int
AppImpl::remove()
{
    if (-1 == nt_service->remove()) {
        ACE_ERROR((LM_ERROR, "%T %P %t | %p\n", "Remover"));
        return -1;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | Remover realizado\n"));
    return 0;
}

/** @brief start
  *
  * @todo: document this function
  */
int
AppImpl::start()
{
    if (-1 == nt_service->start_svc()) {
        ACE_ERROR((LM_ERROR, "%T %P %t | %p\n", "Iniciar"));
        return -1;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | Iniciar realizado\n"));
    return 0;
}

/** @brief kill
  *
  * @todo: document this function
  */
int
AppImpl::kill()
{
    if (-1 == nt_service->stop_svc()) {
        ACE_ERROR((LM_ERROR, "%T %P %t | %p\n", "Detener"));
        return -1;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | Detener realizado\n"));
    return 0;
}

/** @brief set_type
  *
  * @todo: document this function
  */
int
AppImpl::set_type()
{
    if (-1 == nt_service->startup(startup_opt)) {
        ACE_ERROR((LM_ERROR, "%T %P %t | %p\n", "Cambiar Tipo de Inicio"));
        return -1;
    }
    ACE_DEBUG((LM_DEBUG, "%T %P %t | Cambiar Tipo de Inicio realizado\n"));
    return 0;
}

// Define una función para manejar Ctrl+C y salir limpiamente del programa.
BOOL __stdcall
AppImpl::ConsoleHandler(DWORD ctrlType)
{
    ACE_UNUSED_ARG(ctrlType);
    nt_service->handle_control(SERVICE_CONTROL_STOP);
    return TRUE;
}

/** @brief debug
  *
  * @todo: document this function
  */
int
AppImpl::debug()
{
    SetConsoleCtrlHandler(&AppImpl::ConsoleHandler, 1);
    return nt_service->svc();
}

int
AppImpl::help()
{
    ACE_DEBUG((LM_INFO, ACE_TEXT(
                "Uso: %s"
                " -h -i[N] -r -s -k -tN -c -d\n"
                "  -h: Muestra esta ayuda\n"
                "  -i: Instala este programa como un Servicio NT [Tipo de Inicio opcional]\n"
                "  -r: Remueve este programa del Administrador de Servicios\n"
                "  -s: Inicia el servcio\n"
                "  -k: Detiene el servicio\n"
                "  -t: Cambia el Tipo de Inicio del programa instalado\n"
                "  -d: Modo depuracion; ejecuta el programa como una aplicacion normal\n"
                "Donde N: 2 = Automatico, 3 = Manual, y 4 = Deshabilitado\n"),
                NtServiceImpl::NAME));
    return 1;
}

void
AppImpl::parse_args(int argc, ACE_TCHAR* argv[])
{
    ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("hi:rskt:d"));
    int c;

    while ((c = get_opt()) != -1) {
        switch(c) {
            case 'h':
                option = Help;
                break;
            case 'i':
                startup_opt = ACE_OS::atoi(get_opt.opt_arg());
                option = (startup_opt > 0) ? Install : Help;
                break;
            case 'r':
                option = Remove;
                break;
            case 's':
                option = Start;
                break;
            case 'k':
                option = Kill;
                break;
             case 't':
                startup_opt = ACE_OS::atoi(get_opt.opt_arg());
                option = (startup_opt > 0) ? SetType : Help;
                break;
            case 'd':
                option = Debug;
                break;
            default:
                // aquí manejamos el caso de -i sin argumento
                if (ACE_OS::strcmp(get_opt.argv()[get_opt.opt_ind() - 1], ACE_TEXT("-i")) == 0) {
                    option = Install;
                    startup_opt = DEFAULT_SERVICE_INIT_STARTUP;
                } else {
                    option =  Help;
                }
                break;
        }
    }
}

} // namespace SclUpdate
