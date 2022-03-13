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
	signal(SIGUSR1,myfunc); 
	p = (struct mybuf *)shmat(shmid,NULL,0);
	
	//read share mem
	pid = p->pid;

	//write client pid to share mem
	p->pid = getpid();

	kill(pid,SIGUSR2);
	
	//client start read data from share mem
	while(1)
	{
		pause();//wait server write data to share mem
		printf("client process recive data from share memory:%s",p->buf);//read data 
		kill(pid,SIGUSR2);//server may write share mem
	}
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
}
