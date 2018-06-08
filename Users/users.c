//Simon Yip CSCI 49366 HW 7 part 1
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
int main(void)
{
struct passwd *pwd;
/*
uid_t UID=getuid();
printf("UID=%d\n", UID);
uid_t eUID=geteuid();
printf("UID=%d\n", eUID);*/
for (int i=0;i<100000;i++){
if(pwd=getpwuid(i)){


printf("user name: %s\n", pwd->pw_name);
//printf("|\tencrypted password\t| %20s \t|\n",pwd->pw_passwd);
printf("user ID: %u\n", pwd->pw_uid);
//printf("|\tnumerical group ID\t| %20u \t|\n", pwd->pw_gid);
//printf("|\tcomment field\t\t| %20s \t|\n", pwd->pw_gecos);
//printf("|\thome directory\t\t| %20s \t|\n", pwd->pw_dir);
//printf("|\tdefault shell\t\t| %20s \t|\n", pwd->pw_shell);
//printf("|\tuser class\t\t| %20s \t|\n", pwd->pw_class);
}
}
return 0;
}