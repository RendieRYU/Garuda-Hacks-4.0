#ifndef ENCRYPT_H
#define ENCRYPT_H

#ifdef __cplusplus
extern "C" {
#endif

void encryptAES(const unsigned char *plainText, const unsigned char *key, unsigned char *cipherText);

#ifdef __cplusplus
}
#endif

#endif // ENCRYPT_H
