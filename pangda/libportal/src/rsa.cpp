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
#include<portal/global.h>
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
    //PEM_read_RSA_PUBKEY
    if (PEM_read_RSAPublicKey(pubkey_file, &encrypt_key, 0, 0) == NULL) {
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

int CryptRSA::generate_keyfile(std::string pubkey_name, std::string prikey_name) {
    //TODO:check if pubname priname is NULL
    //生成key,这里设置了1024，意味着最多可以编解码1024/8-11=117个字节，
    //RSA_F4为公钥指数，一般情况下使用RSA_F4即可，
    //其它两个参数可以设置为NULL
    RSA *rsa = RSA_generate_key(KEYLENGTH, RSA_F4, NULL, NULL);
    if (rsa == NULL) {
        libportal::lib_error("Error on CryptRSA::generate_keyfile, function: RSA_generate_key");
        return -1;
    }

    BIO *bio = BIO_new_file(pubkey_name.c_str(), "wb");
    if (bio == NULL) {
        libportal::lib_error("Error on CryptRSA::generate_keyfile, function: BIO_new_file");
        return -2;
    }

    if (PEM_write_bio_RSAPublicKey(bio, rsa) == 0) {
        libportal::lib_error("Error on CryptRSA::generate_keyfile, function: PEM_write_bio_RSAPublicKey");
        return -3;
    }
    BIO_free_all(bio);

    bio = BIO_new_file(prikey_name.c_str(), "w");
    if (bio == NULL) {
        libportal::lib_error("Error on CryptRSA::generate_keyfile, function: BIO_new_file");
        return -4;
    }
    if (PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL) == 0) {
        libportal::lib_error("Error on CryptRSA::generate_keyfile, function: PEM_write_bio_RSAPrivateKey");
        return -5;
    }
    BIO_free_all(bio);
    RSA_free(rsa);

    return 0;
}