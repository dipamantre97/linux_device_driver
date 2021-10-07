#include <linux/module.h>/*module init and exit*/
#include <linux/kernel.h>/*Flags KERN_...*/
#include <linux/kthread.h> /*all the kthread functions*/
#include <linux/delay.h>/*sleep*/ 
#include <linux/sched.h>/*task_struct*/
#include <linux/spinlock.h> /*for spin*/



MODULE_LICENSE("GPL");/*authorization*/
MODULE_AUTHOR("DIPA");
MODULE_DESCRIPTION(" spin");
MODULE_VERSION("1.0.0");



//static atomic_t rds_ib_allocation = ATOMIC_INIT(0);
static struct task_struct *char_thread1;/*passing the structs to create the threads 1 & 2*/
static struct task_struct *char_thread2;
//static task_t *char_thread1;
//static task_t *char_thread2;


int mydata =0;


int my_thread_fun1(void *p);/*operating on the global variable inside the thread functions*/
int my_thread_fun2(void *p);


spinlock_t my_spin;


int my_thread_fun1(void *p)
{	
	while(!kthread_should_stop())
	{		
		spin_lock(&my_spin);/*locking mechanisms*/
		mydata++;
		printk(KERN_SOH "my_thread_fun1 spinlocked 		%d\n",mydata);
		printk(KERN_SOH "  ");
		spin_unlock(&my_spin);
		printk(KERN_SOH "my_thread_fun1 spin unlocked 	%d\n",mydata);
		msleep(1000);
	}
	
	return 0;	
}

int my_thread_fun2(void *p)
{	
	while(!kthread_should_stop())
	{
		spin_lock(&my_spin);
		mydata++;
		printk(KERN_SOH "my_thread_fun2 spinlocked 	        %d\n", mydata);
		printk(KERN_SOH "  ");
		spin_unlock(&my_spin);
		printk(KERN_SOH "my_thread_fun2 spin unlocked       	%d\n", mydata);
		msleep(1000);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_SOH"My sample driver start");


	char_thread1= kthread_run(my_thread_fun1,NULL,"my char thread");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread1");
		
	}
	char_thread2 = kthread_run(my_thread_fun2,NULL,"my char thread1");
	if(char_thread2)
	{
		printk(KERN_INFO" create the thread2");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread2");
		
	}
	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My sample driver exit\n");

	kthread_stop(char_thread1);
	kthread_stop(char_thread2);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);