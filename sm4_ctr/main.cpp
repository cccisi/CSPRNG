#define _CRT_SECURE_NO_WARNINGS //�궨��
#include <stdio.h>
#include <iostream>
#include "sms4.h"
#include <windows.h> 
using namespace std;

#define IN
#define OUT
#define MAX_THREADS 16
#define MAX_BUF_SIZE 255

//BIN���
/**************************************************************************
*��  ��:    ���������
*��  ��:    data        �������ָ��
*           length      �������
*
*����:		�³��͡�����
**************************************************************************/
void print_bin(const void* data, int length)
{
	int i;
	const unsigned char* p = (const unsigned char*)data;
	for (i = 0; i < length; ++i) {
		int numdex = *p++;
		char *bin = (char *)malloc(sizeof(char) * 16);
		_itoa(numdex, bin, 2);
		//printf("ԭ��=%d ��������=%08s\n", numdex, bin);
		printf("%08s", bin);
		free(bin); bin = NULL;
	}
}

//HEX���
/**************************************************************************
*��  ��:    ʮ���������
*��  ��:    data        �������ָ��
*           length      �������
*
*����:		�³��͡�����
**************************************************************************/

void print_hex(const void* data, int length)
{
	int i;
	const unsigned char* p = (const unsigned char*)data;
	for (i = 0; i < length; ++i) {
		if (i && !(i & 3)) {
			//printf((i & 31) ? "" : "\n");
		}
		//printbin(*p++);
		printf("%02x", *p++);
	}
	printf("%s\n", "");
}

//����һ���ṹ��
typedef struct _MyData
{
	int id;
	int val2;
	//BYTE *seedctr = (BYTE *)malloc(sizeof(BYTE)*(SMS4_KEY_LENGTH));
	BYTE seedctr[SMS4_KEY_LENGTH];
}MyData, *pMyData;

//�߳�ִ�к���
/**************************************************************************
*��  ��:    �߳�ִ�к���
*����:		�³��͡�����
**************************************************************************/
DWORD WINAPI MyThreadPro(LPVOID lpParam)
{
	MyData* pa = (MyData*)lpParam;
	/*
	BYTE Plain[SMS4_KEY_LENGTH] = { 0xf2, 0x24, 0x22, 0x33, 0x54, 0x72, 0x21, 0x28, 0x93, 0xa5, 0x12, 0xc1, 0xb2, 0x83, 0x50, 0xd2 };
	BYTE *thp = (BYTE *)malloc(sizeof(BYTE)*(SMS4_KEY_LENGTH));
	*(thp + 15) += pa->id;
	int PlainLength = SMS4_KEY_LENGTH;
	BYTE Key[SMS4_KEY_LENGTH] = { 0x12, 0x21, 0x22, 0x33, 0x55, 0x12, 0x21, 0x22, 0x33, 0x55, 0x12, 0x21, 0x22, 0x33, 0x55, 0x12 };
	BYTE *Cipher = (BYTE *)malloc(sizeof(BYTE)*((PlainLength / 16 + 1) * 16));
	int CipherLength = NULL;
	SM4Encrypt(thp, PlainLength, Cipher, CipherLength, Key);
	printf("%s\n", "�����ƴ���");
	nist_dump_hex(Cipher, CipherLength);
	free(Cipher); Cipher = NULL;
	printf("%s\n", "");
	*/
	int SeedLength = SMS4_BLOCK_LENGTH;
	BYTE Key[SMS4_KEY_LENGTH] = { 0x71, 0x57, 0x95, 0x33, 0x55, 0x12, 0x21, 0x22, 0x33, 0x55, 0x12, 0x21, 0xa5, 0x12, 0xc1, 0x12 };
	BYTE *Cipher = (BYTE *)malloc(sizeof(BYTE)*((SeedLength / 16 + 1) * 16));
	int CipherLength = NULL;
	SM4Encrypt(pa->seedctr, SeedLength, Cipher, CipherLength, Key);
	print_bin(Cipher, CipherLength);
	free(Cipher); Cipher = NULL;
	//printf("%s\n", "");
	return 0;
	//���ص��ý��̵�Ĭ���ڴ�Ѿ����
}

//���������
/**************************************************************************
*��  ��:    ������
*����:		�³���
*�ص㣺     1.���̲߳�����2.ÿ����64kb��reseedһ��
**************************************************************************/

int main(int argc, char* argv[])
{
	int SEED_NU = 0;
	//�趨seed�أ��˴�ӦΪTRNG���
	BYTE Seed[4][SMS4_KEY_LENGTH] = {
	0xf2, 0x24, 0x22, 0x33, 0x54, 0x72, 0x21, 0x28, 0x93, 0xa5, 0x12, 0xc1, 0xb2, 0x83, 0x50, 0xd2,
	0xb3, 0x45, 0x25, 0x63, 0x78, 0x71, 0x57, 0x95, 0xfa, 0x4b, 0x3a, 0x7b, 0xa2, 0xc3, 0x59, 0xd7,
	0x13, 0x6B, 0x77, 0x89, 0x19, 0x5A, 0x34, 0x26, 0x83, 0x6D, 0x16, 0x1D, 0x42, 0x85, 0x67, 0x02,
	0xf1, 0xC4, 0xA2, 0xA3, 0xB4, 0x92, 0x42, 0xF9, 0x9F, 0x6A, 0x62, 0xc6, 0xB2, 0x83, 0x52, 0x44
	};
	
	printf("%s\n", "ʹ��Ĭ��seed�أ�SM4_CTR��������������������ƴ���");
	
	//���߳�����
	pMyData pData;
	DWORD dwThreadId[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	char flag = NULL;
	int rand = 0;
	do{
		rand = 3 & rand; 
		SEED_NU = rand++;
		flag = NULL;
		printf("%s������seed: ", "reseed"); print_hex(Seed[SEED_NU], SMS4_BLOCK_LENGTH);
	//forѭ����������߳�
	for (int i = 0; i < MAX_THREADS; i++)
	{
		//CTR,ÿ�����飬��ʼ������1
		Seed[SEED_NU][SMS4_BLOCK_LENGTH - 1] += 1;

		//��������
		pData = (pMyData)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof MyData);  //���ڽ���Ĭ�϶��ڴ���������һ�������ƶ��Ŀռ䣬��СΪsizeof(Data)���ڴ�ռ�
		if (pData == NULL)
			ExitProcess(2);
		pData->id = i;
		pData->val2 = 100 + i;
		int lenth;
		for (lenth = 0; lenth < SMS4_BLOCK_LENGTH; lenth++) {
			pData->seedctr[lenth] = Seed[SEED_NU][lenth];
		}

		//�����߳�
		hThread[i] = CreateThread(NULL, 0, MyThreadPro, pData, 0, &dwThreadId[i]);
		if (hThread[i] == NULL)
		{
			ExitProcess(i);
		}

		//Sleep(120);
	}

	//�ȴ������߳̽���
	WaitForMultipleObjects(MAX_THREADS, hThread, true, INFINITE);

	//ͨ��CloseHandle�������رո��̶߳���
	for (int i = 0; i < MAX_THREADS; i++)
	{
		CloseHandle(hThread[i]);
	}
	printf("\n�������%d bit��������������Ƿ������[Y/N]", 128 * MAX_THREADS);
	flag = getchar();
	getchar();
	} while (flag == 'y'|| flag == 'Y');

	system("pause");
	
	return 0;
}