#include <ace/OS_main.h>
#include <ace/Log_Msg.h>

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

#include "app.h"

int
ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    return SclUpdate::App::instance()->run(argc, argv);
}

#else

int
ACE_TMAIN(int, ACE_TCHAR*[])
{
    ACE_DEBUG((LM_INFO, "Este programa requiere Windows NT"));
    return 0;
}

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
