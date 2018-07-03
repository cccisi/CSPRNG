// SMS4�ԳƼ����㷨
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
Plain: ��������
PlainLength:���ĳ���(�ֽڣ�
Cipher: �������
CipherLength:���ĳ��ȣ�����ʱ��ʾ���Ļ������ĳ��ȣ�����Ϊ���ĳ���+16�������Ϊ���ĵ�ʵ�ʳ��ȣ�
Key: ������Կ��16�ֽ�
*/
void SM4Encrypt(BYTE *Plain, int PlainLength, BYTE *Cipher, int &CipherLength, BYTE *Key);

/**
Cipher: ��������
CipherLength:���ĳ���
Plain: �������
PlainLength:���ĳ���(����ʱ��ʾ���Ļ������ĳ��ȣ�����Ϊ���ĳ��ȣ������Ϊ���ĵ�ʵ�ʳ��ȣ�
Key: ������Կ��16�ֽ�
*/
void SM4Decrypt(BYTE *Cipher, int CipherLength, BYTE *Plain ,int &PlainLength, BYTE *Key);

//���²���

void Sms4ExtendKey(unsigned int *subkey, const unsigned char *key);
void Sms4Encrypt(unsigned char *cipher, const unsigned char *plain, const unsigned int *subkey);
void Sms4Decrypt(unsigned char *plain, const unsigned char *cipher, const unsigned int *subkey);


//private 
void SM4UnPad(BYTE *out, int outLength, BYTE *Plain, int &PlainLength);
void SM4Pad(BYTE *out, int outLength, BYTE *Plain, int &PlainLength);
unsigned int Sms4F(unsigned int w0, unsigned int w1, unsigned int w2, unsigned int w3, unsigned int rkey);
unsigned int Sms4FinExtendedKey(unsigned int w0, unsigned int w1, unsigned int w2, unsigned int w3, unsigned int ck);

#endif // SMS4_HEADER_FDA90FJA09H___FDA98SFHA____FD98ASFH__
