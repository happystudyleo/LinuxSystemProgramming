#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
void myfunc(int signum)
{
	return;
}
int main()
{
	int shmid;
	pid_t pid;
	char *p;
	shmid =shmget(IPC_PRIVATE,128,IPC_CREAT|0777);
	if(shmid<0)
	{
		fprintf(stderr,"create shm failure\n");
		return -1;
	}

	pid =fork();

	if(pid>0)
	{	
		signal(SIGUSR2,myfunc);
		p = (char*)shmat(shmid,NULL,0);
		if(p==NULL)
		{
			fprintf(stderr,"shmat failure\n");
			return -1;
		}

		while(1)
		{
			printf("parent process start write share memory:\n");
			fgets(p,128,stdin);
			kill(pid,SIGUSR1);
			pause();
		}
	}
	if(pid==0)
	{
		signal(SIGUSR1,myfunc);
		p = (char*)shmat(shmid,NULL,0);
		if(p==NULL)
		{
			fprintf(stderr,"shmat failure\n");
			return -1;
		}
		while(1)
		{
		pause();
		printf("share mem data:%s",p);
		kill(getppid(),SIGUSR2);
		}
	}
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	system("ipcs -m");
return 0;
}
