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
#include<portal/crypt.h>
#include<cstdio>

using libportal::CryptMD5;

std::string CryptMD5::encrypt(const std::string &dat) {
    unsigned char buf[ENCRYPTLEN];
    std::string ret;
    char tmp[3] = { 0 };
    MD5((const unsigned char *)dat.c_str(), dat.length(), buf);
    for (int i = 0; i < ENCRYPTLEN; i++) {
        sprintf(tmp, "%02x", (int)buf[i]);
        ret += tmp;
    }
    return ret;
}