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
#include<cstring>

using libportal::CryptRSA;

std::string CryptRSA::encrypt(const std::string pubkey_filename, const std::string& dat) {
    if (pubkey_filename.empty() || dat.empty()) {
        assert(false);
        return "";
    }
    FILE *pubkey_file = fopen(pubkey_filename.c_str(), "rb");
    if(pubkey_file == NULL) {
        assert(false);
        return "";
    }

    std::string ret;
    RSA *encrypt_key = RSA_new();
    if (PEM_read_RSA_PUBKEY(pubkey_file, &encrypt_key, 0, 0) == NULL) {
        assert(false);
        return "";
    }

    int pklen = RSA_size(encrypt_key);
    char *pret_temp = new char[pklen + 1];
    int test = RSA_public_encrypt(dat.length(), (const unsigned char*)dat.c_str(), 
                                 (unsigned char*)pret_temp, encrypt_key, RSA_PKCS1_PADDING);

    if (test >= 0) {
        ret = std::string(pret_temp, test);
    }
    delete[] pret_temp;

    RSA_free(encrypt_key);
    fclose(pubkey_file);
    CRYPTO_cleanup_all_ex_data();
    return ret;
}

std::string CryptRSA::decrypt(const std::string prikey_filename, const std::string& dat) {
    if (prikey_filename.empty() || dat.empty()) {
        assert(false);
        return "";
    }
    FILE *prikey_file = fopen(prikey_filename.c_str(),"rb");
    if (prikey_file == NULL ) {
        assert(false);
        return "";
    }
    std::string ret;
    RSA *decrypt_key = RSA_new();
    if(PEM_read_RSAPrivateKey(prikey_file, &decrypt_key, 0, 0) == NULL) {
        assert(false);
        return "";
    }
    int pklen = RSA_size(decrypt_key);
    char *pret_temp = new char[pklen + 1];

    int test = RSA_private_decrypt(dat.length(), (const unsigned char*)dat.c_str(),
                                  (unsigned char*)pret_temp, decrypt_key, RSA_PKCS1_PADDING);
    if(test >= 0) {
        ret = std::string(pret_temp, test);
    }
    delete[] pret_temp;
    RSA_free(decrypt_key);
    fclose(prikey_file);
    CRYPTO_cleanup_all_ex_data();
    return ret;
}