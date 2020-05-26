
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

//遍历文件夹
void find(char * lpPath,bool uncode)
{
	char szFind[MAX_PATH], szFile[MAX_PATH],szFindDirName[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind, lpPath);
	strcat(szFind, "\\*.*"); 
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strcpy(szFile, lpPath);
				strcat(szFile, "\\");
				strcat(szFile, FindFileData.cFileName);
				find(szFile, uncode);
			}
		}
		else
		{
			strcpy(szFindDirName, lpPath);
			strcat(szFindDirName, "\\");
			strcat(szFindDirName, FindFileData.cFileName);
			if (uncode)
			{
				loadfile(szFindDirName);
				uncompressData(szFindDirName);
				
			}
			else
			{
				loadfile(szFindDirName);
				compressData(szFindDirName);
			}
			
			
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}
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
			find(argv[2],false);
		}
		else if (!strcmp(argv[1], "/u"))
		{
			printf("uncompress data ");
			find(argv[2], true);
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
		printf("no enough memory: ");
		printf("%s\n", loadFileName);
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
		printf("uncompress failed: ");
		printf("%s\n", loadFileName);
		return -1;
	}

	//保存到文件
	FILE *p2file;
	char outFile[MAX_PATH];
	memset(outFile, 0, sizeof(outFile));
	//拼接字符串输出
	char str1[]= "_uncompress";

	strcat_s(outFile, strlen(loadFileName) + 1, loadFileName);
	int len2 = strlen(outFile) + strlen(str1) + 1;
	strcat_s(outFile, len2, str1);
	
	p2file = fopen(outFile, "wb");
	if (p2file == NULL)
	{
		printf("create file failed: ");
		printf("%s\n", loadFileName);
		if (de_text != NULL)
		{
			free(de_text);
			de_text = NULL;
		}
		return -1;
	}
	int a = fwrite(de_text, tlen, 1, p2file);
	fclose(p2file);
	/* 打印结果，并释放内存 */
	printf("succeed: ");
	printf("%s", loadFileName);
	printf(">>>>");
	printf("%s\n", outFile);
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
		printf("no enough memory: ");
		printf("%s\n", loadFileName);
		return -1;
	}
	
	/* 压缩 */
	if (compress((Bytef *)de_text, &tlen, (Bytef *)buffer, tlen) != Z_OK)
	{
		
		printf("compress failed: ");
		printf("%s\n", loadFileName);
		return -1;
	}


	//保存到文件
	FILE *p2file;
	char outFile[MAX_PATH];
	memset(outFile, 0, sizeof(outFile));
	//拼接字符串输出
	char str1[] = "_compress";
	strcat_s(outFile, strlen(loadFileName) + 1, loadFileName);
	int len2 = strlen(outFile) + strlen(str1) + 1;
	strcat_s(outFile, len2, str1);

	p2file = fopen(outFile, "wb");
	if (p2file == NULL)
	{
		printf("create file failed: ");
		printf("%s\n", loadFileName);
		if (de_text != NULL)
		{
			free(de_text);
			de_text = NULL;
		}
		return -1;
	}
	int a = fwrite(de_text, tlen, 1, p2file);
	fclose(p2file);
	/* 打印结果，并释放内存 */
	printf("succeed: ");
	printf("%s", loadFileName);
	printf(" >>>> ");
	printf("%s\n", outFile);
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
