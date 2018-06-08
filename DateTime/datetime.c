//Simon Yip CSCI 49366 HW 6, change line 9 to 360 print time every hour
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
	printf("Times is also printed to file, Current time set to print every 10 seconds\n");
	while(1){
	sleep(10); //chamge to 360 for hour 
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
    char s[128];
    strftime(s, sizeof(s), "%c", tm);
    printf("%s\n", s);
	FILE* fp=fopen("datetime.txt","a+b");
	fprintf(fp,"%s",s);
	fprintf(fp,"\n");
	fclose(fp);
	}
}