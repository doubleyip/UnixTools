//Simon Yip CSCI 49366 HW 7 part 2
#include <sys/types.h>
#include <sys/param.h> /* for NGROUPS_MAX */
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>

int main()
{
char *user=getenv("USERNAME");
struct group *ptr;
char **userIterator;
int num_groups;
int list_groups;
const gid_t basegid;
gid_t grouplist[NGROUPS_MAX];
gid_t *gi;
num_groups=getgroups(NGROUPS_MAX, grouplist);
initgroups(user,basegid);

printf("There are %d supplementary groups\n",num_groups);

for (int i=0; i<num_groups; i++)
{
printf("group %d\n", grouplist[i]);
while ((ptr=getgrent())!=NULL)
{
	if(grouplist[i]==ptr->gr_gid)
	{
		printf("group name %s\n", ptr->gr_name);
		//printf("group ID %d\n", ptr->gr_gid);

	}

}
endgrent();
}

return 0;
}
