#define _CRT_SECURE_NO_WARNINGS //宏定义
#include <stdio.h>
#include <iostream>
#include "sms4.h"
#include <windows.h> 
using namespace std;

#define IN
#define OUT
#define MAX_THREADS 16
#define MAX_BUF_SIZE 255

//BIN输出
/**************************************************************************
*功  能:    二进制输出
*参  数:    data        输出数据指针
*           length      输出长度
*
*作者:		陈朝晖、刘攀
**************************************************************************/
void print_bin(const void* data, int length)
{
	int i;
	const unsigned char* p = (const unsigned char*)data;
	for (i = 0; i < length; ++i) {
		int numdex = *p++;
		char *bin = (char *)malloc(sizeof(char) * 16);
		_itoa(numdex, bin, 2);
		//printf("原数=%d 二进制数=%08s\n", numdex, bin);
		printf("%08s", bin);
		free(bin); bin = NULL;
	}
}

//HEX输出
/**************************************************************************
*功  能:    十六进制输出
*参  数:    data        输出数据指针
*           length      输出长度
*
*作者:		陈朝晖、刘攀
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

//定义一个结构体
typedef struct _MyData
{
	int id;
	int val2;
	//BYTE *seedctr = (BYTE *)malloc(sizeof(BYTE)*(SMS4_KEY_LENGTH));
	BYTE seedctr[SMS4_KEY_LENGTH];
}MyData, *pMyData;

//线程执行函数
/**************************************************************************
*功  能:    线程执行函数
*作者:		陈朝晖、刘攀
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
	printf("%s\n", "二进制串：");
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
	//返回调用进程的默认内存堆句柄。
}

//主函数入口
/**************************************************************************
*功  能:    主函数
*作者:		陈朝晖
*特点：     1.多线程并发；2.每发生64kb，reseed一次
**************************************************************************/

int main(int argc, char* argv[])
{
	int SEED_NU = 0;
	//设定seed池，此处应为TRNG输出
	BYTE Seed[4][SMS4_KEY_LENGTH] = {
	0xf2, 0x24, 0x22, 0x33, 0x54, 0x72, 0x21, 0x28, 0x93, 0xa5, 0x12, 0xc1, 0xb2, 0x83, 0x50, 0xd2,
	0xb3, 0x45, 0x25, 0x63, 0x78, 0x71, 0x57, 0x95, 0xfa, 0x4b, 0x3a, 0x7b, 0xa2, 0xc3, 0x59, 0xd7,
	0x13, 0x6B, 0x77, 0x89, 0x19, 0x5A, 0x34, 0x26, 0x83, 0x6D, 0x16, 0x1D, 0x42, 0x85, 0x67, 0x02,
	0xf1, 0xC4, 0xA2, 0xA3, 0xB4, 0x92, 0x42, 0xF9, 0x9F, 0x6A, 0x62, 0xc6, 0xB2, 0x83, 0x52, 0x44
	};
	
	printf("%s\n", "使用默认seed池，SM4_CTR方法发生拟随机数二进制串：");
	
	//多线程配置
	pMyData pData;
	DWORD dwThreadId[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	char flag = NULL;
	int rand = 0;
	do{
		rand = 3 & rand; 
		SEED_NU = rand++;
		flag = NULL;
		printf("%s，本轮seed: ", "reseed"); print_hex(Seed[SEED_NU], SMS4_BLOCK_LENGTH);
	//for循环创建多个线程
	for (int i = 0; i < MAX_THREADS; i++)
	{
		//CTR,每个分组，初始向量加1
		Seed[SEED_NU][SMS4_BLOCK_LENGTH - 1] += 1;

		//参数数据
		pData = (pMyData)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof MyData);  //即在进程默认堆内存上申请了一个不可移动的空间，大小为sizeof(Data)的内存空间
		if (pData == NULL)
			ExitProcess(2);
		pData->id = i;
		pData->val2 = 100 + i;
		int lenth;
		for (lenth = 0; lenth < SMS4_BLOCK_LENGTH; lenth++) {
			pData->seedctr[lenth] = Seed[SEED_NU][lenth];
		}

		//创建线程
		hThread[i] = CreateThread(NULL, 0, MyThreadPro, pData, 0, &dwThreadId[i]);
		if (hThread[i] == NULL)
		{
			ExitProcess(i);
		}

		//Sleep(120);
	}

	//等待所有线程结束
	WaitForMultipleObjects(MAX_THREADS, hThread, true, INFINITE);

	//通过CloseHandle函数来关闭该线程对象
	for (int i = 0; i < MAX_THREADS; i++)
	{
		CloseHandle(hThread[i]);
	}
	printf("\n本轮输出%d bit二进制随机数，是否继续？[Y/N]", 128 * MAX_THREADS);
	flag = getchar();
	getchar();
	} while (flag == 'y'|| flag == 'Y');

	system("pause");
	
	return 0;
}