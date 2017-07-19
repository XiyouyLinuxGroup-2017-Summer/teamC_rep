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
#include<iostream>
using namespace std;

namespace libportal {

class Number {
private:
    static const int MAXSIZE = 1000;
    static const int DLEN = 4;
    static const int MAXN = 9999;
    int dat[MAXSIZE];
    int length;
public:
    Number();
    Number(const int);
    Number(const char *);
    Number(const Number &);

    Number &operator = (const Number &);
    Number operator + (const Number &) const;
    Number operator - (const Number &) const;
    Number operator * (const Number &) const;
    Number operator / (const int &) const;
    Number operator ^ (const int &) const;
    int operator % (const int &) const;
    bool operator > (const Number &) const;
    bool operator > (const int &) const;
    bool operator < (const Number &) const;
    bool operator < (const int &) const;
//COMMENT IT
//    void print();
};

}
