#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

//STRONGLY Recommend learning Base64
//This function is used to encode and encrypt string
//Base64: transform bit info to human-readable string in character and digit
//And fill in 62/64 character
//Additionally, algo may use + or / to index character 63, 64
//Also to manage padding character to equal-sized
//For decoder, cannot lost the file when transmit via mail
namespace Base64
{
    std::string base64_encode(const std::string &);

    //SALT = ENCRYPT KEY
    const std::string &SALT1 = "TT::MY::LOVE";
    const std::string &SALT2 = "LHP1417";
    const std::string &SALT3 = "kali_python";


    std::string EncryptB64(std::string s)
    {
        s = SALT1 + s + SALT2 + SALT3;
        s = base64_encode(s);
        //insert salt randomly for harder decrypt
        s.insert(7,SALT3);
        //insert or encode more if I want
        return s;
    }

    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string &s)
    {
        std::string ret;    //return value
        int val = 0;
        int bits = -6;
        const unsigned int b63 = 0x3F;   //3F = 63

        for(const auto &c : s)  //auto: if we dont know what type of input, then c++ will determine for you
                                //&c : s - will assigned as a every character of s string
        {
            val = (val << 8) + c;   //bit operator: shift left 8 places
            bits +=8;
            while(bits >= 0)
            {
                ret.push_back(BASE64_CODES[(val >> bits) & b63]);//GET in key in map
                /*
                & operator is the adding operator in binary. EX:
                    0011
                    0101
                =>  0001
                */
                bits -= 6;
                /*ex: MAN IN LOVE
                => M in then bits = 2 => get binary into &val
                => then push right val with to 2 characters, to get 6 char of binary
                Then get A into &val => bits = 4
                Keep repeat then 3 chars = 4 chars BASE64
                */
            }
        }
        if(bits > -6)
            ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);

        while (ret.size()%4)
            ret.push_back('=');

        return ret;
    }
}

#endif // BASE_64_H
