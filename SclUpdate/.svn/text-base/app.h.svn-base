#ifndef SCLUPDATE_APP_H
#define SCLUPDATE_APP_H

#include <ace/SString.h>
#include <ace/Singleton.h>
#include <ace/Mutex.h>

namespace SclUpdate
{

class NtServiceImpl;

/**
 * Clase principal de configuración y arranque del servidor
 */
class AppImpl
{
    public:

        AppImpl();
        ~AppImpl();

        /**
         * configura e inicia el servidor.
         */
        int run(int argc, ACE_TCHAR* argv[]);

    private:
        // este atributo es estático para poder ser utilizado por
        // el método ConsoleHandler, pero debe ser pensado como
        // si fuese un miembro del objeto, lo cual se cumple de
        // hecho al ser App un singleton
        static NtServiceImpl* nt_service;

        static BOOL __stdcall ConsoleHandler(DWORD ctrlType);

        int startup_opt;

        enum ArgOption {
              Run       = 1
            , Install
            , Remove
            , Start
            , Kill
            , SetType
            , Debug
            , Help
        };

        ArgOption option;
        ACE_TCHAR path[MAX_PATH];

        void parse_args(int argc, ACE_TCHAR* argv[]);

        int exec_option();

        int run();
        int install();
        int remove();
        int start();
        int kill();
        int set_type();
        int debug();
        int help();

        int load_config();
        int init_path();
};

typedef ACE_Singleton<AppImpl, ACE_Mutex> App;

} // namespace SclUpdate

#endif // SCLUPDATE_APP_H
