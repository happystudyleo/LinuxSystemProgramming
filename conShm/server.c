//无亲缘关系进程通信
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/shm.h>
#include<string.h>
struct mybuf
{
	int pid;
	char buf[124];
};

void myfunc(int signum)
{
	return;
}
int main()
{
	int shmid;
	int key;
	pid_t pid;
	struct mybuf *p = NULL;
	key = ftok("./a.c",'a');
	if(key<0)
	{
		fprintf(stderr,"ftok error\n");
		return -1;
	}
	shmid = shmget(key,128,IPC_CREAT|0777);
	if(shmid <0)
	{
		 fprintf(stderr,"ftok error\n");
                 return -1;
	}
	printf("create share memory success shmid = %d\n",shmid);
	signal(SIGUSR2,myfunc); 
	p = (struct mybuf *)shmat(shmid,NULL,0);

	//get client pid 
	p->pid = getpid();

	pause();

	pid = p->pid;

	while(1)
	{
		printf("parent process start write share memory:\n");
		fgets(p->buf,123,stdin);
		kill(pid,SIGUSR1);
		pause();
	}

	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
}
