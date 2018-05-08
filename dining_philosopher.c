#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/time.h>
#include<linux/timer.h>
#include<linux/semaphore.h>
#include<linux/delay.h>

#define N 5
#define LEFT (num+4) % 5
#define RIGHT (num+1) % 5
#define HUNGRY 1
#define EATING 0
#define THINKING 2

static struct semaphore bowl;
//static struct semaphore del;
static struct semaphore chopstick[5];
int state[5];
static struct task_struct *philThread[5];

//static int alldone = 0;
static int count=0;

int philosopher(void* x)
{
	
	int num = (int)x, i;
		//pickChopstick(num);
		printk(KERN_INFO "enter in pick\n");
		down(&bowl);
		count++;
		state[num] = HUNGRY;	
		printk(KERN_INFO "Philosopher %d is hungry\n",num+1);
		
		//chopstickTest(num);
		printk(KERN_INFO "enter test\n");	
		if(state[num]==HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
		{	
			printk(KERN_INFO "in if test\n");
			state[num] = EATING;		
			printk(KERN_INFO "Philosopher %d taking fork %d and %d\n",num+1,LEFT+1,num+1);
			printk(KERN_INFO "Philosopher %d is eating\n",num+1);
			up(&chopstick[num]);
			//count++;
		}
		printk(KERN_INFO "exit test\n");
			
		up(&bowl);
		down(&chopstick[num]);
		printk(KERN_INFO "exit in pick\n");
		
		//putChopstick(num);
		down(&bowl);
		printk(KERN_INFO "enter put\n");
		state[num] = THINKING;
		printk(KERN_INFO "Philosopher %d putting fork %d and %d\n",num+1,LEFT+1,num+1);
		printk(KERN_INFO "Philosopher %d is thinking\n",num+1);
		
		//chopstickTest(LEFT);
		printk(KERN_INFO "enter test\n");	
		if(state[LEFT]==HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
		{	
			printk(KERN_INFO "in if test\n");
			state[LEFT] = EATING;		
			printk(KERN_INFO "Philosopher %d taking fork %d and %d\n",LEFT+1,LEFT+1,LEFT+1);
			printk(KERN_INFO "Philosopher %d is eating\n",LEFT+1);
			up(&chopstick[LEFT]);
			//count++;
		}
		printk(KERN_INFO "exit test\n");
		
		//chopstickTest(RIGHT);
		printk(KERN_INFO "enter test\n");	
		if(state[RIGHT]==HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
		{	
			printk(KERN_INFO "in if test\n");
			state[RIGHT] = EATING;		
			printk(KERN_INFO "Philosopher %d taking fork %d and %d\n",RIGHT+1,LEFT+1,RIGHT+1);
			printk(KERN_INFO "Philosopher %d is eating\n",RIGHT+1);
			up(&chopstick[RIGHT]);
			//count++;
		}
		printk(KERN_INFO "exit test\n");

		up(&bowl);
		//alldone++;
		printk(KERN_INFO "exit put\n");
	
	do_exit(0);
	return 0;
}

asmlinkage long sys_dining(void) {	

	int i = 1;
	
	printk(KERN_INFO "in init");

	sema_init(&bowl,1);	
	
	for(i=0; i<5; i++)
	{
		sema_init(&chopstick[i],0);
	}	
	
	char our_thread[8]="thread1";
	for(i=0; i<5; i++)		
	{
		philThread[i] = kthread_create(philosopher,(void*)i,our_thread);
		if((philThread[i]))
		{
			wake_up_process(philThread[i]);
			printk(KERN_INFO "in if");
			
		}
		printk(KERN_INFO "Philosopher %d is thinking\n",i+1);
	}
	printk(KERN_INFO "\nEXIT MODULE\n");

	printk(KERN_INFO "Cleaning up kthread");

	if(count >=4)
	{
		for(i=0; i<5; i++)
		{
			kthread_stop(philThread[i]);
			printk(KERN_INFO "philossopher %d killed\n", i+1);		
		}
		
	}
	
	return 0;
}
