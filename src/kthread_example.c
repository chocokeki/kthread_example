#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

struct queue_info {
	struct task_struct	*task_id;
	void			*data;
};

struct queue_info *qinfo;
static char *sched_name = NULL;
module_param(sched_name, charp, 0);


int read_cpu_info(void *data)
{
	//do something with data
	if(unlikely(data == NULL))
		return -1;
	else
		return 0;
} 

int kth_schedd(void *arg)
{
	int ret;
	int count = 0;
	const int poll_interval = 1000;
	struct queue_info *qi = (struct queue_info*)arg;

	while(!kthread_should_stop())
	{
		msleep_interruptible(poll_interval);
	
		if(read_cpu_info(qi->data)) {
			ret = -1;
			//do something
		}

		printk("chocokeki@debug - gathering : %d\n", count++);
	}

	return 0;
}

static int kthread_example_init(void)
{
	int ret;

	qinfo = kzalloc(sizeof(struct queue_info), GFP_KERNEL);
	if(qinfo == NULL)
		return -ENOMEM;
	
	if(sched_name == NULL)
		sched_name = "kth_tmpp";

	qinfo->task_id = kthread_run(kth_schedd, qinfo, "%s-#%d", sched_name, 0);
	if(IS_ERR(qinfo->task_id)) {
		ret = PTR_ERR(qinfo->task_id);		
		goto kth_init_err1;
	}


	return 0;

kth_init_err1:
	kfree(qinfo);

	return ret;
}

static void kthread_example_exit(void)
{
	kthread_stop(qinfo->task_id);
	kfree(qinfo);
}

module_init(kthread_example_init);
module_exit(kthread_example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chocokeki@gmail.com");
