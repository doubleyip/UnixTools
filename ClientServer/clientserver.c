//Simon Yip CSCI 49366 HW 14 Client Server aka Final Project
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>


#define DB_SEARCH 1
#define DB_INSERT 2
#define DB_DELETE 3
#define DB_REPLACE 4
#define QUIT_REQUEST 5


/* a per-request struct, one per client thread */
typedef struct request_tag
{
	struct request_tag *next; /* pointer to next request */
	int operation; /* one of read/write/quit request */
	int synchronous; /* whether request is synchronous */
	int done_flag; /* predicate this request done */
	pthread_cond_t done;/* wait for this thread to finish */
	char prompt[32]; /* prompt server message to client */
	char text[28]; /* read/write text */
} request_t;

/* a server struct, organized as a client request queue */
typedef struct server_tag
{
	request_t *first; /* first request in the queue */
	request_t *last; /* last request in the queue */
	int running; /* predicate this server running */
	pthread_mutex_t mutex; /* lock around server data */
	pthread_cond_t request; /* wait for a request */
} server_t;

server_t server={NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};
int client_threads; /* client has 4 threads */
pthread_mutex_t client_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_done=PTHREAD_COND_INITIALIZER;
void *server_routine(void *arg);
void server_request(int operation, int sync, const char *prompt,char *string);
void *client_routine(void *arg);
void deleteline(char *filename, int linenum);
void replaceline(char *filename, int linenum, char *currentbook);

char currentbook[30];	//global to store current book
char rline[30];			//global to store target line

int main(int argc, char **argv)
{

char dbcommand[30];
int requestvar;
pthread_t thread;
client_threads=0;
int err;

while(1)	//while loop for user input
{
	printf("Please enter a command: search, insert, delete, replace, or quit\n");
	fgets(dbcommand,30,stdin); //user input for command to use
	
	if (strcmp(dbcommand,"search\n")==0){  //if search, initialize thread and call routine with req 1 
	printf("Enter the book to search for\n");
	fgets(currentbook,20,stdin);
	requestvar=1;		//call for client routine
	client_threads++;
	err=pthread_create(&thread, NULL, client_routine, (void*)(uintptr_t) requestvar);
	err=pthread_mutex_lock(&client_mutex);
	while (client_threads>0)	//wait for other threads to finish first, then unlock mutex
	{
		err=pthread_cond_wait(&clients_done, &client_mutex);
		err=pthread_mutex_unlock(&client_mutex);
	}
	}
	else if (strcmp(dbcommand,"insert\n")==0){	//if search, initialize thread and call routine with req 2
	printf("Enter the book to insert\n");
	fgets(currentbook,20,stdin);
	requestvar=2;		//call for client routine
	client_threads++;
	err=pthread_create(&thread, NULL, client_routine, (void*)(uintptr_t) requestvar);
	err=pthread_mutex_lock(&client_mutex);
	while (client_threads>0)	//wait for other threads to finish first, then unlock mutex
	{
		err=pthread_cond_wait(&clients_done, &client_mutex);
	
	}	err=pthread_mutex_unlock(&client_mutex);
	}
	else if (strcmp(dbcommand,"delete\n")==0){	//if delete, initialize thread and call routine with req 3
	printf("Enter the book to delete\n");
	fgets(currentbook,20,stdin);
	requestvar=3;		//call for client routine
	client_threads++;
	err=pthread_create(&thread, NULL, client_routine, (void*)(uintptr_t) requestvar);
	err=pthread_mutex_lock(&client_mutex);
	while (client_threads>0)	//wait for other threads to finish first, then unlock mutex
	{
		err=pthread_cond_wait(&clients_done, &client_mutex);
		err=pthread_mutex_unlock(&client_mutex);
	}
	}
	else if (strcmp(dbcommand,"replace\n")==0){	//if replace, initilize thread and call routine with req 4
	printf("Enter the book to replace with\n");
	fgets(currentbook,20,stdin);
	printf("Enter line to replace\n");		//prompt for line to replace
	fgets(rline,20,stdin);
	requestvar=4;		//call for client routine
	client_threads++;
	err=pthread_create(&thread, NULL, client_routine, (void*)(uintptr_t) requestvar);
	err=pthread_mutex_lock(&client_mutex);
	while (client_threads>0)	//wait for other threads to finish first, then unlock mutex
	{
		err=pthread_cond_wait(&clients_done, &client_mutex);
		err=pthread_mutex_unlock(&client_mutex);
	}
	}
	else if (strcmp(dbcommand,"quit\n")==0){	//if quit, exit the program
	printf("Exiting program");
	exit(0);
	}
	else
	{
		printf("Please enter a valid command\n");
	}
}	
return 0;	
}

void *server_routine(void *arg)
{
printf("In server_routine()...\n");
static pthread_mutex_t prompt_mutex=
PTHREAD_MUTEX_INITIALIZER;
request_t *request;
int operation;
int err;
while (1)
{
	/* lock the server mutex, to protect server-modifying data */
	err=pthread_mutex_lock(&server.mutex); 
	while (server.first==NULL)
	{
		/* wait for request */
		err=pthread_cond_wait(&server.request, &server.mutex);
	}
/* dequeue 1st request in the queue */
request=server.first;
/* advance the head of the queue */
server.first=request->next;
if (server.first==NULL)
/* if queue is empty, reset tail to NULL */
server.last=NULL;
err=pthread_mutex_unlock(&server.mutex);
/* read what the requested op was: read/write/quit */
operation=request->operation;
switch (operation)	//switch case for db operations
{
case DB_SEARCH:		//case for db_search
	printf("Searching for: %s",currentbook);	//scan file looking for word
	char word[1024];
	int count;
	int found4=0;
	FILE* fp = fopen("bookrecord.txt", "r+w");
	while(fscanf(fp,"%s", word) != EOF)
	{
		//printf("%s\n",word);
		//printf("%s",currentbook);
		if(strncmp(word, currentbook,strlen(word))==0)
		{
			printf("Found %s",currentbook);		//print word and count if found
			fscanf(fp,"%d",&count);
			printf("Count is %d\n",count);
			found4=1;
		}
	}
	if(found4==0)
	{
		printf("Not found: %s",currentbook);
	}
	fclose(fp);
break;
case DB_INSERT:		//case for db_insert
	printf("Inserting: %s",currentbook);
	int linecount=0;
	int found=0;
	int count1;
	char word1[1024];
	FILE* fp1 = fopen("bookrecord.txt", "r+b");
	while(fscanf(fp1,"%s", word1)!= EOF)	//scan file looking for word
	{	
		linecount=linecount+1;
		//printf("linecount is: %d\n",linecount);
		//printf("%s\n",word);
		//printf("%s",currentbook);
		if(strncmp(word1, currentbook,strlen(word1))==0)
		{									//if word is found
			printf("Found %s",currentbook);
			fscanf(fp1,"%d",&count1);
			printf("Count is %d\n",count1);		//print word and count if found
			fseek(fp1,-1,SEEK_CUR);			//move pointer backwards to increment
			count1++;
			fprintf(fp1,"%d",count1);		//increment count in file
			printf("New count is %d\n",count1);
			found=1;
			linecount=linecount+2;			
			int linefound=(linecount-1)/2;		//print line book was found at
			printf("book was found at line %d\n",linefound);
			//printf("linecount is: %d\n",linecount);
		}
	}
	fclose(fp1);
	if(found==0)	//if not found
	{
		printf("Not Found: %s",currentbook);
		printf("Inserting book\n");
		FILE* fp2=fopen("bookrecord.txt","a+b");
		fprintf(fp2,"\n");	//write book and count of 1 to file
		char *newbook=strcat(strtok(currentbook,"\n")," 1"); 
		fprintf(fp2,"%s",newbook);
		fclose(fp2);
	}
break;
case DB_DELETE:		//case for db_delete
printf("Deleting: %s",currentbook);
//printf("Inserting: %s",currentbook);
	int linecount1=0;
	int found1=0;
	int count2;
	char word2[1024];
	FILE* fp3 = fopen("bookrecord.txt", "r+w");
	while(fscanf(fp3,"%s", word2) != EOF)	//scan file looking for book to delete
	{
		linecount1=linecount1+1;
		//printf("linecount is: %d\n",linecount1);
		//printf("%s\n",word);
		//printf("%s",currentbook);
		if(strncmp(word2, currentbook,strlen(word2))==0) //if book is found
		{
			printf("Found %s",currentbook);	//print book and count
			//fscanf(fp3,"%d",&count2);
			//printf("Count is %d\n",count2);
			found1=1;
			linecount1=linecount1+2;
			int linefound1=(linecount1-1)/2;
			//printf("linecount is: %d\n",linecount1);
			printf("Deleting book at line %d\n",linefound1);
			deleteline("bookrecord.txt",linefound1); //function to "delete" line
		}
	}

	fclose(fp3);
	if(found1==0)	//if not found print error message
	{
		printf("Not Found: %s",currentbook);
		printf("Deletion Failed");
	}
break;
case DB_REPLACE:		//case for db_replace
printf("Replacing: %s",currentbook);
	int linecount2=0;
	int found2=0;
	int count3;
	char word3[1024];
	FILE* fp5 = fopen("bookrecord.txt", "r+b");
	while(fscanf(fp5,"%s", word3)!= EOF)	//scan file for book to replace
	{
		linecount2=linecount2+1;
		//printf("linecount is: %d\n",linecount2);
		if(strncmp(word3, currentbook,strlen(word3))==0)	//if book is found
		{
			printf("Found %s",currentbook);
			fscanf(fp5,"%d",&count3);
			printf("Count is %d\n",count3);	//print book and count
			fseek(fp5,-1,SEEK_CUR);	//move pointer back
			count3++;
			fprintf(fp5,"%d",count3);	//increment count in file
			printf("New count is %d\n",count3);
			found2=1;
			linecount2=linecount2+2;
			int linefound2=(linecount2-1)/2;
			//printf("linecount is: %d\n",linecount2);
			printf("book was found at line %d\n",linefound2);
		}
	}
	fclose(fp5);
	if(found2==0)	//if not found
	{
		printf("Not Found: %s",currentbook);
		printf("Inserting book at line %s",rline);
		int rlinenum=atoi(rline);	//convert string to int
		//printf("num = %d\n", rlinenum);
		replaceline("bookrecord.txt",rlinenum,currentbook);	//function to "delete" line and print new entry
	}
break;
default:
break;
}
/* 0=non-synchronous, 1=synchronous */
if (request->synchronous==1)
{
err=pthread_mutex_lock(&server.mutex);
/* set the done flag to 1, will be the predicate to
client to synchronize with other */
request->done_flag=1;
/* signal that the request is done processing */
pthread_cond_signal(&request->done);
err=pthread_mutex_unlock(&server.mutex);
}
else
free(request);
if (operation==QUIT_REQUEST)
break; /* break from the while(1) loop */
}
return NULL;
}

void *client_routine(void *arg)
{
printf("In client_routine()...\n");
int client_number=(uintptr_t)arg;
char prompt[32];
char string[128], formatted[128];
int err;
sprintf(prompt, "Client %d>", client_number);
//puts(prompt);
while (1)		//1 for search, 2 for insert, 3 for delete, 4 for replace
{
	if (client_number==1)
	{
		server_request(DB_SEARCH,1,prompt,string);	//server req for search
		break;
	}
	if (client_number==2)
	{
		server_request(DB_INSERT,1,prompt,string);  //server req for insert
		break;
	}
	if (client_number==3)
	{
		server_request(DB_DELETE,1,prompt,string);	//server req for delete 
		break;
	}
	if (client_number==4)
	{
		server_request(DB_REPLACE,1,prompt,string);	//server req for replace
		break;
	}
}
err=pthread_mutex_lock(&client_mutex);
client_threads--;
if (client_threads<=0)
err=pthread_cond_signal(&clients_done);
err=pthread_mutex_unlock(&client_mutex);
printf("Exiting client_routine()...\n");
return NULL;
}

void server_request(int operation, int sync, const char *prompt,char *string)
{
request_t *request;
int err;
/* lock the server data */
err=pthread_mutex_lock(&server.mutex);

/* create the server thread */
if (!server.running)
{
pthread_t thread;
/* server thread is created as a detached thread */
pthread_attr_t detached_attr;
err=pthread_attr_init(&detached_attr);
err=pthread_attr_setdetachstate(&detached_attr,PTHREAD_CREATE_DETACHED);
server.running=1;
err=pthread_create(&thread, &detached_attr,server_routine, NULL);
pthread_attr_destroy(&detached_attr);
}
/* create the request */
request=(request_t *)malloc(sizeof(request_t));
request->next=NULL; /* this is tail */
/* these are going to be passed-in arguments */
request->operation=operation;
request->synchronous=sync;
if (sync)
{
/* set the done predicate to 0 */
request->done_flag=0;
/* initialize the pthread_cond_t variable "done" */
err=pthread_cond_init(&request->done, NULL);
}
/* all requests get a prompt */
if (prompt!=NULL)
strncpy(request->prompt, prompt, 32);
else
request->prompt[0]='\0';
if (server.first==NULL) /* if queue is empty */
{
server.first=request; /* head=tail=request */
server.last=request;
}
else /* if queue is non-empty */
{
/* make next of current tail point to request */
(server.last)->next=request;
/* make tail equal to request */
server.last=request;
}
/* tell server that there is a request made */
err=pthread_cond_signal(&server.request);
/* if request was synchronous, wait for a reply */
if (sync)
{
while (!request->done_flag)
err=pthread_cond_wait(&request->done,&server.mutex);
err=pthread_cond_destroy(&request->done);
free(request);
}
err=pthread_mutex_unlock(&server.mutex);
}

void deleteline(char *filename, int linenum)	//function to delete target line
{
		int count = 0;
		int linenumber=linenum;
        char ch;
        FILE *fp1;
		FILE *fp2;
		char fname[128];
        char string[128]; 
		char temp[]="temp.txt";	
        fp1=fopen(filename,"r");
        if(!fp1) 
		{
                printf(" File not found or unable to open the input file!!\n");
        }
        fp2=fopen(temp,"w"); // open the temporary file in write mode 
        if (!fp2) 
		{
                printf("Unable to open a temporary file to write!!\n");
                fclose(fp1);
        }
        // copy all contents to the temporary file except the specific line
        while (!feof(fp1)) 
        {
			strcpy(string,"\0");
            fgets(string,128,fp1);
            if (!feof(fp1)) 
            {
                count++;
                if(count!=linenumber) //if not target line, copy contents
                {
					fprintf(fp2,"%s",string);
                }
            }
        }
        fclose(fp1);
        fclose(fp2);
        remove(fname);  			 
        rename(temp, filename); 
}

void replaceline(char *filename, int linenum, char *currentbook)	//function to replace target line with book
{
		int count = 0;
		int linenumber=linenum;
        char ch;
        FILE *fp1;
		FILE *fp2;
		char fname[128];
        char string[128]; 
		char temp[]="temp.txt";	
        fp1=fopen(filename,"r");
        if(!fp1) 
		{
                printf(" File not found or unable to open the input file!!\n");
        }
        fp2=fopen(temp,"w");  
        if (!fp2) 
		{
                printf("Unable to open a temporary file to write!!\n");
                fclose(fp1);
        }
        // copy all contents to the temporary file except the specific line
        while (!feof(fp1)) 
        {
			strcpy(string,"\0");
            fgets(string,128,fp1);
            if (!feof(fp1)) 
            {
                count++;
				//replace the line with current book
                if(count!=linenumber) //if not target line, copy contents
                {
					fprintf(fp2,"%s",string);
                }
				else
				{
					char *newbook2=strcat(strtok(currentbook,"\n")," 1");
					fprintf(fp2,"%s\n",newbook2);	//at target line, print current book
				}
            }
        }
        fclose(fp1);
        fclose(fp2);
        remove(fname);  			 
        rename(temp, filename); 
}