/*************************************************************************
    libportal, a library for network
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
#include<string>

namespace libportal {

struct pt_version_t {
    int main_version, sub_version, update_version;
    static const int MAGIC_NUMBER = 200000;
};

const pt_version_t LIBRARY_VERSION { 1, 0, 0 };

int lib_error(std::string statement);

}


