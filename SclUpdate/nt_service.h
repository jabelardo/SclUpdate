#ifndef SCLUPDATE_SERVICE_H
#define SCLUPDATE_SERVICE_H

#include <ace/config-lite.h>

#if defined(ACE_WIN32) && !defined(ACE_LACKS_WIN32_SERVICES)

#include <ace/NT_Service.h>
#include <SclUpdate/configuration.h>

namespace SclUpdate
{

class NtServiceImpl : public ACE_NT_Service
{
    public:

        NtServiceImpl();
        virtual ~NtServiceImpl();

        static char const* DESCRIPTION;
        static char const* NAME;

        int load_config();

        int run();

        /**
         * Sobrecargamos el método <handle_control> para manejar las
         * solicitudes de finalización del programa
         */
        virtual void handle_control(DWORD control_code);

        /**
         * Sobrecargamos el método <handle_exception> para que un código
         * de control 'stop' pueda sacar al reactor de su espera
         */
         virtual int  handle_exception(ACE_HANDLE h);

         virtual int svc();

    private:
        typedef ACE_NT_Service Super;

        bool running;
        Configuration config;
};

typedef ACE_Singleton<NtServiceImpl, ACE_Mutex> NtService;

} // namespace SclUpdate


#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */

#endif // SCLUPDATE_SERVICE_H
