
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "zlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * pFile;
long lSize;
char * buffer;
size_t result;

int uncompressData(char *loadFileName);
void loadfile(char *loadFileName);
int compressData(char *loadFileName);

void out_help()
{
	printf("	\nThis is a call to zilb compression/decompression of data procedures\n\n");
	printf("	/h		--See how to use it\n\n");
	printf("	/c [filename]	--Compress a file name is \"compress_\" + [fimename]\n\n");
	printf("	/u [filename]	--decompression a file name is \"uncompress_\" + [fimename]\n\n");
	printf("	--Files are only allowed to be zlib compressed data--\n");
	printf("	");

}


int main(int argc, char * argv[])
{
	switch (argc)
	{
	case 1:
		out_help();
		break;
	case 2:
		if(!strcmp(argv[1],"/h"))
		{
			out_help();
		}
		else
		{
			printf("	\n**Error parameter**\n");
			out_help();
		}
		break;
		
	case 3:
		if (!strcmp(argv[1], "/c"))
		{
			printf("compress data ");
			loadfile(argv[2]);
			compressData(argv[2]);

		}
		else if (!strcmp(argv[1], "/u"))
		{
			printf("uncompress data ");
			loadfile(argv[2]);
			uncompressData(argv[2]);
		}
		break;
	default:
		printf("	\n**Error parameter**\n");
		out_help();
		break;
	}

	return 0;
}


int uncompressData(char *loadFileName)
{

	char* de_text = NULL;
	uLong tlen = lSize * 10;
	if ((de_text = (char*)malloc(sizeof(char)*lSize * 10)) == NULL)
	{
		printf("no enough memory!\n");
		return -1;
	}

	int number = uncompress((Bytef *)de_text, &tlen, (Bytef *)buffer, lSize);
	/* 解压缩 */
	if (number)
	{
	//Z_OK
	//Z_STREAM_END    1
	//Z_NEED_DICT     2
	//Z_ERRNO        (-1)
	//Z_STREAM_ERROR (-2)
	//Z_DATA_ERROR   (-3)
	//Z_MEM_ERROR    (-4)
	//Z_BUF_ERROR    (-5)
	//Z_VERSION_ERROR (-6)
		printf("uncompress failed!\n");
		return -1;
	}

	//保存到文件
	FILE *p2file;
	char outFile[MAX_PATH];
	memset(outFile, 0, sizeof(outFile));
	//拼接字符串输出
	char str1[]= "uncompress_";
	strcat_s(outFile, strlen(str1) + 1, str1);
	int len2 = strlen(outFile) + strlen(loadFileName) + 1;
	strcat_s(outFile, len2, loadFileName);
	
	p2file = fopen(outFile, "wb");
	int a = fwrite(de_text, tlen, 1, p2file);
	fclose(p2file);
	/* 打印结果，并释放内存 */
	printf("succeed");
	if (de_text != NULL)
	{
		free(de_text);
		de_text = NULL;
	}
	return 0;
}

int compressData(char *loadFileName)
{

	char* de_text = NULL;
	uLong tlen = compressBound(lSize);
	if ((de_text = (char*)malloc(sizeof(char)*tlen)) == NULL)
	{
		printf("no enough memory!\n");
		return -1;
	}
	
	/* 压缩 */
	if (compress((Bytef *)de_text, &tlen, (Bytef *)buffer, tlen) != Z_OK)
	{
		printf("compress failed!\n");
		return -1;
	}


	//保存到文件
	FILE *p2file;
	char outFile[MAX_PATH];
	memset(outFile, 0, sizeof(outFile));
	//拼接字符串输出
	char str1[] = "compress_";
	strcat_s(outFile, strlen(str1) + 1, str1);
	int len2 = strlen(outFile) + strlen(loadFileName) + 1;
	strcat_s(outFile, len2, loadFileName);

	p2file = fopen(outFile, "wb");
	int a = fwrite(de_text, tlen, 1, p2file);
	fclose(p2file);
	/* 打印结果，并释放内存 */
	printf("succeed");
	if (de_text != NULL)
	{
		free(de_text);
		de_text = NULL;
	}
	return 0;
}



void loadfile(char *loadFileName)
{
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */
	//pFile = fopen("E://CcFile//everyday//TS//bug18151//agent_nck//virus//file.bin", "rb");
	pFile = fopen(loadFileName, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		printf("open file fail");
	}

	/* 获取文件大小 */
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	/* 分配内存存储整个文件 */
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		printf("Memory alloc falil");
	}

	/* 将文件拷贝到buffer中 */
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		printf("Load file to memory falil");
	}
}
