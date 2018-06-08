//Simon Yip CSCI 49366 Hw 3 part 1
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

void *binary(int octal);  //function to convert octal number to binary

int main (int argc, char *argv[])
{
	struct stat buf;
	
	if(stat("stat.c",&buf)==0) 
	{
		printf("stat structure values of %s\n","stat.c");
		printf("st_mode \t\t| %o\n", buf.st_mode);	
		printf("st_ino \t\t\t| %o\n", buf.st_ino);
		printf("st_dev \t\t\t| %o\n", buf.st_dev);
		printf("st_rdev \t\t| %o\n", buf.st_rdev);
		printf("st_nlink value\t\t| %o\n", buf.st_nlink);
		printf("st_uid \t\t\t| %o\n", buf.st_uid);
		printf("st_gid \t\t\t| %o\n", buf.st_gid);
		printf("st_size \t\t| %o\n", buf.st_size);
		printf("st_atime \t\t| %o\n", buf.st_atime);
		printf("st_mtime \t\t| %o\n", buf.st_mtime);
		printf("st_ctime \t\t| %o\n", buf.st_ctime);
		printf("st_blksize \t\t| %o\n", buf.st_blksize);
		printf("st_blocks \t\t| %o\n", buf.st_blocks);
				
		printf("st_mode values of %s\n",argv[1]);
		printf("set-user-ID\t\t|");binary(buf.st_mode &0x8000);printf("\n");
		printf("set-group-ID\t\t|");binary(buf.st_mode &0x4000);printf("\n");
		printf("sticky bit\t\t|");binary(buf.st_mode &0x2000);printf("\n");
		printf("file type\t\t|");binary(buf.st_mode &0x1E00);printf("\n");
		printf("access permission\t|");binary(buf.st_mode &0x01FF);printf("\n");
	}
	else
		if(stat("stat.c",&buf)==-1)
		{
		printf("File %s not found", "stat.c");
		}



}


void *binary(int octal)  //function to convert octal number to binary
{
	int n=octal;
	int i=0;
	while(n>0)
	{
		n=n/2;
		i++;
	}
	
	char *binarynum;
	binarynum= malloc(sizeof(char)* i);
	n=octal;
	int j=i-1;
	
	if(octal==0)
	{
		printf("0");
		binarynum="0";
	}
	
	while (n>0)
	{
		binarynum[j]=n%2;
		n=n/2;
		j--;
	}
	for (int k=0;k<i;k++)
	{
		printf("%d",binarynum[k]);
	}
	
} 
