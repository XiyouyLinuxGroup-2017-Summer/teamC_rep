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
#include<portal/number.h>
#include<algorithm>
#include<cstring>

using libportal::Number;

Number::Number() {
    length = 1;
    memset(dat, 0, sizeof(dat));
}

Number::Number(const int b) {
    int c, d = b;
    length = 0;
    memset(dat, 0, sizeof(dat));

    while (d > MAXN) {
        c = d - (d / (MAXN + 1)) * (MAXN + 1);
        d = d / (MAXN + 1);
        dat[length++] = c;
    }
    dat[length++] = d;
}

Number::Number(const char *s) {
    int index, l = strlen(s);
    memset(dat, 0, sizeof(dat));
    l = strlen(s);
    length = l / DLEN;

    if (l % DLEN)
        length++;
    index = 0;
    for (int i = l - 1; i >= 0; i -= DLEN) {
        int t = 0, k = i - DLEN + 1;
        if (k < 0)
            k = 0;
        for (int j = k; j <= i; j++) {
            t = t * 10 + s[j] - '0';
        }
        dat[index++] = t;
    }
}

Number::Number(const Number &T) : length(T.length) {
    memset(dat, 0, sizeof(dat));
    for (int i = 0; i < length; i++) {
        dat[i] = T.dat[i];
    }
}

Number &Number::operator =(const Number &n) {
    memset(dat, 0, sizeof(dat));
    length = n.length;
    for (int i = 0; i < length; i++) {
        dat[i] = n.dat[i];
    }
    return *this;
}


Number Number::operator +(const Number &T) const {
    Number t(*this);
    int big = (T.length > length) ? T.length : length;

    for (int i = 0; i < big; i++) {
        t.dat[i] += T.dat[i];
        if (t.dat[i] > MAXN) {
            t.dat[i + 1]++;
            t.dat[i] -= MAXN + 1;
        }
    }

    t.length = (t.dat[big]) ? big + 1: big;
    return t;
}

Number Number::operator -(const Number &T) const {
    Number t1 = std::max(*this, T),
        t2 = std::min(*this, T);
    bool flag = (*this > T) ? false : true;

    int big = t1.length;
    for (int i = 0; i < big; i++) {
        if (t1.dat[i] < t2.dat[i]) {
            int j = i + 1;
            while (t1.dat[j] == 0) {
                j++;
            }
            t1.dat[j--]--;
            while (j > i) {
                t1.dat[j--] += MAXN;
            }
            t1.dat[i] += MAXN + 1 - t2.dat[i];
        } else {
            t1.dat[i] -= t2.dat[i];
        }
    }
    t1.length = big;
    while (t1.dat[length - 1] == 0 && t1.length > 1) {
        t1.length--;
        big--;
    }

    if (flag)
        t1.dat[big - 1] = 0 - t1.dat[big - 1];
    return t1;
}

Number Number::operator *(const Number &T) const {
    Number ret;

    int i, j = 0;
    for (i = 0; i < length; i++) {
        int up = 0;
        for (j = 0; j < T.length; j++) {
            int tmp = dat[i] * T.dat[j] + ret.dat[i + j] + up;
            if (tmp > MAXN) {
                int tmp2 = tmp - tmp / (MAXN + 1) * (MAXN + 1);
                up = tmp / (MAXN + 1);
                ret.dat[i + j] = tmp2;
            } else {
                up = 0;
                ret.dat[i + j] = tmp;
            }
        }
        if (up != 0)
            ret.dat[i + j] = up;
    }
    ret.length = i + j;
    while ((ret.dat[ret.length - 1] == 0) && (ret.length > 1)) {
        ret.length--;
    }
    return ret;
} 

Number Number::operator /(const int &b) const {
    Number ret;
    int down = 0;
    for (int i = length - 1; i >= 0; i--) {
        ret.dat[i] = (dat[i] + down * (MAXN + 1)) / b;
        down = dat[i] + down * (MAXN + 1) - ret.dat[i] * b;
    }
    ret.length = length;
    while (ret.dat[ret.length - 1] == 0 && ret.length > 1) {
        ret.length--;
    }
    return ret;
}

int Number::operator %(const int &b) const {
    int d = 0;
    for (int i = length - 1; i >= 0; i--) {
        d = ((d * (MAXN + 1)) % b + dat[i]) % b;
    }
    return d;
}

Number Number::operator ^(const int &n) const {
    Number t, ret(1);
    int i;

    if (n <= 0)
        return 1;
    if (n == 1)
        return *this;
    
    int m = n;
    while (m > 1) {
        t = *this;
        for (i = 1; i << 1 <= m; i <<= 1) {
            t = t * t;
        }
        m -= i;
        ret = ret * t;
        if (m == 1)
            ret = ret * (*this);
    }
    return ret;
}

bool Number::operator >(const Number &T) const {
    if (length > T.length)
        return true;
    if (length == T.length) {
        int ln = length - 1;
        while (dat[ln] == T.dat[ln] && ln >= 0) {
            ln--;
        }
        if (ln >= 0 && dat[ln] > T.dat[ln]) {
            return true;
        }
    }
    return false;
}

bool Number::operator >(const int &t) const {
    Number b(t);
    return *this > b;
}

bool Number::operator <(const Number &T) const {
    return !(*this > T);
}

bool Number::operator <(const int &t) const {
    return !(*this > t);
}

/*
istream& operator>>(istream & in, Number & b)
{
    char ch[Number::MAXSIZE*4];
    int i = -1;
    cin>>ch;
    int l=strlen(ch);
    int count=0,sum=0;
    for(i=l-1;i>=0;)
    {
        sum = 0;
        int t=1;
        for(int j=0;j<4&&i>=0;j++,i--,t*=10)
        {
            sum+=(ch[i]-'0')*t;
        }
        b.dat[count]=sum;
        count++;
    }
    b.length =count++;
    return in;
 
}
ostream& operator<<(ostream& out,  Number& b)
{
    int i;
    cout << b.dat[b.length - 1];
    for(i = b.length - 2 ; i >= 0 ; i--)
    {
        cout.width(Number::DLEN);
        cout.fill('0');
        cout << b.dat[i];
    }
    return out;
}

#include<stdio.h>
void Number::print()    //输出大数
{
    int i;
    printf("%d",dat[length-1]);
    for(i = length - 2 ; i >= 0 ; i--)
    {
        printf("%04d",dat[i]);
    }
    printf("\n");
}*/