//Simon Yip Homework for CSCI 493.66 UNIX TOOLS HW 5
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	FILE *myfile=fopen("binary.txt", "r+b");

char data[10]={'a','b','c','d','e','f','g','h','i','j'};
printf("Write the following char array into text\n");
for (int i=0; i<10; i++)
{
//data[i]=i;
printf("%c ",data[i]);
}
printf("\n");


if (fwrite(&data[0], sizeof(char), 10, myfile)!=10)
printf("fwrite error");
char *ptr=malloc(10*sizeof(char));
fseek(myfile, 0, SEEK_SET);
int nitems=fread(ptr, 10*sizeof(char), 10, myfile);
printf("Read the following\n");
for (int j=0; j<10; j++)
printf("%c ", ptr[j]);
printf("\n");

fseek(myfile,0,SEEK_END);




int data1[10];
printf("Write the following number array into text\n");
for(int i=0;i<10;i++)
{
	data1[i]=i;
	printf("%d ", i, data1[i]);
}
printf("\n");
fwrite(&data1[0],sizeof(int),10,myfile);
fseek(myfile,10,SEEK_SET);
int *ptr1=malloc(10*sizeof(int));
int nitems1=fread(ptr1, 10*sizeof(int), 10, myfile);
printf("Read the following\n");
for (int j=0; j<10; j++)
printf("%d ", ptr1[j]);
return 0;

}

	
	
