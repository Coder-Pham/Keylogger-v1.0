#ifndef HELPER_H
#define HELPER_H

#include <ctime>
#include <string>   //CONVERT DATA TO STRING AND SEND THEM TO LOG FILE
#include <sstream>  //CONVERT DATA TYPE TO STRING
#include <fstream>

// Create my own namespace in order to group function type onward
namespace Helper //my own namespace
{
    template <class T> //generate function to accept generic type

    std::string ToString(const T &); //CONVERT ANYTHING TO STRING, Later use to fill the log file

    struct DateTime
    {
        DateTime()  //Create current system local time on machine
        {
            time_t ms;
            time (&ms); //Get the local time

            struct tm* info = localtime(&ms);

            D = info->tm_mday;      //Date in month
            m = info->tm_mon + 1;   //Month
            y = info->tm_year + 1900;   //Year
            M = info->tm_min;       //Minutes
            H = info->tm_hour;      //Hour
            S = info->tm_sec;       //Second
        }

        DateTime(int D, int m, int y, int M, int H, int S) : D(D), m(m), y(y), M(M), H(H), S(S) {}
        //Create DateTime without time
        DateTime(int D, int m, int y) : D(D), m(m), y(y), M(0), H(0), S(0) {}

        DateTime Now() const        //Get current time
        {
            return DateTime();
        }

        int D, m, y, M, H, S;

        std::string GetDateString() const
        {
            return std::string(D < 10 ? "0" : "") + ToString(D) +
                    std::string(m < 10 ? ".0" : "") + ToString(m) + "." + ToString(y);
        }       //return DD.mm.yyyy

        std::string GetTimeString(const std::string &sep = ":") const
        {
            //Separator is ":" in Time format
            return std::string(H < 10 ? "0" : "") + ToString(H) + sep +
                    std::string(M < 10 ? "0" : "") + ToString(M) + sep +
                    std::string(S < 10 ? "0" : "") + ToString(S);
            //RETURN type HH:MM:SS
        }

        std::string GetDateTimeString(const std::string &sep = ":") const
        {
            return GetDateString() + " " + GetTimeString(sep);
        }
    };
    //GENERIC FUNCTION: CONVERT ANYTHING TO STRING
    template <class T>
    std::string ToString(const T &e)
    {
        std::ostringstream s;   //define output string stream as s
        s << e;
        return s.str();
    }

    void WriteAppLog( const std::string &s )
    {
        std::ofstream file("AppLog.txt", std::ios::app);     //std::ios::app la open file and append
        file << "[" << Helper::DateTime().GetDateTimeString() << "]" << "\n" <<
                s << std::endl << "\n";
        file.close();
    }
}

#endif // HELPER_H
