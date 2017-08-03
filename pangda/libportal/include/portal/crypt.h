/*************************************************************************
    libportal, A Network Library
    Copyright (C) 2017  hepangda

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/
#pragma once
#include<openssl/ssl.h>
#include<openssl/bn.h>
#include<openssl/err.h>
#include<openssl/rsa.h>
#include<openssl/md5.h>
#include<openssl/pem.h>
#include<string>
#include<assert.h>

namespace libportal {

class CryptRSA {
private:
    static const int KEYLENGTH = 1024;
    static const int STRINGLEN = 128;
public:
    static std::string decrypt(const std::string, const std::string&);
    static std::string encrypt(const std::string, const std::string&);
    static int generate_keyfile(std::string pubkey_name, std::string prikey_name);
};

class CryptMD5 {
private:
    static const int ENCRYPTLEN = 1024;
public:
    static std::string encrypt(const std::string&);
};


}