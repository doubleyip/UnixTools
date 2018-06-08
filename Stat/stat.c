//Simon Yip 49366 HW 3 part 2
//I was unable to test this code because chown doesnt work on windows running cygwin.
#include <fcntl.h> /* for open() */
#include <sys/stat.h> /* for file access permission bits */
#include <stdio.h> /* for FILENAME_MAX */
#include <unistd.h> /* for pathconf() */
#include <string.h> /* for strcat() */
#include <stdlib.h> /* for malloc() */

int main (int argc, char *argv[])
{
char *path_name="../Stat";
int fd=open(path_name, O_CREAT|O_RDWR);

uid_t owner=8;
gid_t group=8;

if(chown(path_name,owner,group)<0){
	printf("Invalid permissions for file %s, Permissions changed\n",path_name);
}
if(chown(path_name,owner,group)==0){
	printf("Permissions for file %s changed\n",path_name);
}


}