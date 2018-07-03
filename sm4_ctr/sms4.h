// SMS4对称加密算法
#ifndef SMS4_HEADER_FDA90FJA09H___FDA98SFHA____FD98ASFH__
#define SMS4_HEADER_FDA90FJA09H___FDA98SFHA____FD98ASFH__

#define SMS4_KEY_LENGTH				(128/8)
#define SMS4_BLOCK_LENGTH			(128/8)
#define SMS4_ROUND					32

#include <stdint.h>
#include "string.h"

typedef uint8_t BYTE;

//public api
/**
Plain: 输入明文
PlainLength:明文长度(字节）
Cipher: 输出密文
CipherLength:密文长度（输入时表示密文缓冲区的长度（至少为明文长度+16），输出为密文的实际长度）
Key: 输入密钥，16字节
*/
void SM4Encrypt(BYTE *Plain, int PlainLength, BYTE *Cipher, int &CipherLength, BYTE *Key);

/**
Cipher: 输入密文
CipherLength:密文长度
Plain: 输出明文
PlainLength:明文长度(输入时表示明文缓冲区的长度（至少为密文长度），输出为明文的实际长度）
Key: 输入密钥，16字节
*/
void SM4Decrypt(BYTE *Cipher, int CipherLength, BYTE *Plain ,int &PlainLength, BYTE *Key);

//以下不用

void Sms4ExtendKey(unsigned int *subkey, const unsigned char *key);
void Sms4Encrypt(unsigned char *cipher, const unsigned char *plain, const unsigned int *subkey);
void Sms4Decrypt(unsigned char *plain, const unsigned char *cipher, const unsigned int *subkey);


//private 
void SM4UnPad(BYTE *out, int outLength, BYTE *Plain, int &PlainLength);
void SM4Pad(BYTE *out, int outLength, BYTE *Plain, int &PlainLength);
unsigned int Sms4F(unsigned int w0, unsigned int w1, unsigned int w2, unsigned int w3, unsigned int rkey);
unsigned int Sms4FinExtendedKey(unsigned int w0, unsigned int w1, unsigned int w2, unsigned int w3, unsigned int ck);

#endif // SMS4_HEADER_FDA90FJA09H___FDA98SFHA____FD98ASFH__
