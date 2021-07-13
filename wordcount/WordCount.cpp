#include<stdio.h>
#include<string.h>
#include<io.h>
#include<stdlib.h>
#define MAXSIZE 1024

void findFiles(char* Path, char* choose[], int argc);
void countchar(char* filename);
void countline(char* filename);
void countword(char* filename);
void countSline(char* filename);
void countnoteline(char* filename);

int main(int argc, char* argv[])
{
	int i = 1;
	FILE* fp;
	fp = fopen(argv[argc - 1], "r");
	if (fp == NULL)
	{
		findFiles(argv[argc - 1], argv, argc);
	}
	else
	{
		for (i = 1; i < argc - 1; i++)
		{
			if (strcmp(argv[i], "-c") == 0)
				countchar(argv[argc - 1]);
			if (strcmp(argv[i], "-l") == 0)
				countline(argv[argc - 1]);
			if (strcmp(argv[i], "-w") == 0)
				countword(argv[argc - 1]);
			if (strcmp(argv[i], "-a") == 0)
			{
				countSline(argv[argc - 1]);
				countnoteline(argv[argc - 1]);
			}
		}
		fclose(fp);
	}

	return 0;
}
//输入目录时调用此函数处理目录下文件
void findFiles(char* Path, char* choose[], int argc)
{
	intptr_t judgePath;	//文件句柄，相当与文件指针fp,用于判断是否路径是否存在
	_finddata_t File;

	int i = 1;
	FILE* fp;						//用于判断是否是统计对象
	char strfind[MAXSIZE];			//用于加通配符，寻找文件
	char str[MAXSIZE];				//用于记录文件路径
	strcpy(strfind, Path);
	strcat(strfind, "\\");
	strcat(strfind, "*.*");        // 在要遍历的目录后加上通配符
	judgePath = _findfirst(strfind, &File);    // 查找目录中的第一个文件
	if (judgePath == -1)
	{
		printf("文件路径错误或不存在\n");
		return;
	}
	do
	{
		if (File.attrib & _A_SUBDIR
			&& strcmp(File.name, ".") == 0
			&& strcmp(File.name, "..") == 0
			);
		else
		{
			strcpy(str, Path);
			strcat(str, "\\");
			strcat(str, File.name);
			if ((fp = fopen(str, "r")) != NULL)
			{
				printf("%s\n", str);
				for (i = 1; i < argc - 1; i++)
				{
					if (strcmp(choose[i], "-c") == 0)
						countchar(str);
					if (strcmp(choose[i], "-l") == 0)
						countline(str);
					if (strcmp(choose[i], "-w") == 0)
						countword(str);
				}
			}
		}


	} while (_findnext(judgePath, &File) == 0);    // 查找下一个文件

	_findclose(judgePath);    // 关闭句柄
}



//统计字符数
void countchar(char* filename)
{
	char c;
	int charnum = 0;
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("打开文件失败或文件不存在\n");
	}
	else
	{
		c = fgetc(fp);
		while (c != EOF)
		{
			charnum++;
			c = getc(fp);
		}
		printf("字符数：%d\n", charnum);
		fclose(fp);
	}
}
//统计文件行数
void countline(char* filename)
{
	FILE* fp;
	int linenum = 0;
	char c;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("打开文件失败或文件不存在");
		return;
	}
	c = fgetc(fp);
	if (c != EOF) linenum++;
	while (c != EOF)
	{
		if (c == '\n' || c == '\r')
			linenum++;
		c = fgetc(fp);
	}
	printf("行数：%d\n", linenum);
	fclose(fp);
}
//统计词数，一个字母算作一个单词
void countword(char* filename)
{
	int wordnum = 0;
	char c;
	int judge = 0;
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("打开文件失败或文件不存在");
		return;
	}
	c = fgetc(fp);
	while (c != EOF)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			judge = 1;	//记录前一个字符是不是字母
		}
		else
			judge = 0;
		c = fgetc(fp);
		if (c == EOF) break;
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
			if (judge == 1)
				wordnum++;
	}
	if (judge == 1)
		wordnum++;
	printf("词数：%d\n", wordnum);

	fclose(fp);
}

//统计空行和代码行
void countSline(char* filename)
{
	int spaceline = 0;		//空行
	int codeline = 0;		//代码行
	char c;
	int jnull = 1;		//标志此行是不是空行
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("文件打开失败或文件不存在");
		return;
	}
	do
	{
		c = fgetc(fp);
		if (c == '\n')
		{
			if (jnull == 1) spaceline++;
			else codeline++;
			jnull = 1;				//每一行开始时默认为空行
		}
		if (c != ' ' && c != '\n' && c != '\r' && c != -1)  //不是空格或换行或文件末尾，则不是空行
			jnull = 0;
	} while (!feof(fp));
	if (jnull == 1) spaceline++;	//最后一行判断（没有换行符要独立判断）
	else codeline++;
	printf("代码行：%d\n", codeline);
	printf("空行：%d\n", spaceline);
	fclose(fp);
}

void countnoteline(char* filename)
{
	FILE* fp;
	fp = fopen(filename, "r");
	int noteline = 0;
	char c;
	c = fgetc(fp);
	fp = fopen(filename, "r");
	while (c != EOF)
	{
		if (c == '\\')
		{
			c = fgetc(fp);
			if (c == '\\')
				noteline++;
		}
		c = fgetc(fp);
	}
	printf("注释行：%d\n", noteline);
	fclose(fp);
}