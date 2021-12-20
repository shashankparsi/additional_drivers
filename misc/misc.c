#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/miscdevice.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/mm.h>
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("A simple misc driver");
MODULE_AUTHOR("shashank parsi");
#define SIZE 1024
static char *kernel_buffer;
static int misc_open(struct inode  *inode,struct file *file)
{
	char *buffer=kzalloc(SIZE,GFP_KERNEL);
	pr_info("%s: misc_open func called\n",__func__);
	printk(KERN_INFO "opening device file: %s\n",file_path(file,buffer,PATH_MAX));
	kfree(buffer);
	//return 0;
	return nonseekable_open(inode,file);
}

static int misc_release(struct inode *inode,struct file *file)
{
	char *buffer=kzalloc(SIZE,GFP_KERNEL);
	pr_info("%s: misc_release func called\n",__func__);
	printk(KERN_INFO "opening device file: %s\n",file_path(file,buffer,PATH_MAX));
        kfree(buffer);
	return 0;
}

static ssize_t misc_read(struct file *file,char __user *user_buf,size_t count,loff_t *off)
{
	/*int retval;
	pr_info("%s: misc_read func called\n",__func__);
	printk(KERN_INFO "requested to read %ld bytes from driver to trasmit for userspace \n",count);
	if((retval=copy_to_user(user_buf,kernel_buffer+*off,count)))
		printk(KERN_INFO "issue with copy data to user buffer\n");
	pr_info("copy to user returned: %d\n",retval);
	*off=*off+count;
	return count;*/
	int bytes_read,available_space,bytes_to_read;
	pr_info("%s: read offset: %lld\n",__func__,*off);
	available_space=SIZE-*off;
	if(count < available_space)
	{
		bytes_to_read=count;
	}
	else
	{
		bytes_to_read=available_space;
	}
	pr_info("bytes_to_read: %d\n",bytes_to_read);
	bytes_read = bytes_to_read - copy_to_user(user_buf, kernel_buffer+*off, bytes_to_read);
	pr_info("%s: Copy to user returned:%d\n", __func__, bytes_to_read);
	//update file offset
	*off += bytes_read;

        return bytes_read;

}

static ssize_t misc_write(struct file *file,const char __user *user_buf,size_t count,loff_t *off)
{
	int retval;
	pr_info("%s: misc_write func called\n",__func__);
	printk(KERN_INFO "In writer: %ld\n",count);
	retval=copy_from_user(kernel_buffer,user_buf,count);
	pr_info("copy from user returned: %d\n",retval);
	pr_info("kernel_buffer: %s\n",kernel_buffer);
	kernel_buffer[count]='\0';
	return count;
}


static loff_t misc_llseek(struct file *file,loff_t off,int origin)
{
	loff_t new_pos=0;
	switch(origin)
	{
		case 0: new_pos=off; //SEEK_SET
			break;
		case 1: new_pos=file->f_pos+off; //SEEk_CUR
			break;
		case 2: new_pos=SIZE-off;//SEEK_END
			break;
	}
	if(new_pos > SIZE)
		new_pos=SIZE;
	if(new_pos < 0)
		new_pos=0;
	file->f_pos=new_pos;
	return new_pos;
}
		

static const struct file_operations fops={
	.open=misc_open,
	.read=misc_read,
	.write=misc_write,
	.release=misc_release,
	.owner=THIS_MODULE,
	.llseek=misc_llseek,
};


static struct miscdevice misc_driver={
       .name="misc-test-driver",
	.mode=0666,
 	.minor=MISC_DYNAMIC_MINOR,
	.fops=&fops,
};	

static int __init test_hello_init(void)
{
	static int ret,m;
	ret=misc_register(&misc_driver);
	if(ret)
	{
		pr_info("errro in initializing the misc driver\n");
		return ret;
	}
	kernel_buffer=kvmalloc(SIZE,GFP_KERNEL);
	m=strlcpy(kernel_buffer,"initmessage\n",15);
	printk(KERN_INFO "misc driver is registered successfully with the kernel with minor: %d, m=%d and dev node=%s\n",misc_driver.minor,m,misc_driver.name);
	return 0;
}

static void __exit test_hello_exit(void)
{
	misc_deregister(&misc_driver);
	kfree(kernel_buffer);
	pr_info("successfully de-registered the misc driver from kernel\n");
}

module_init(test_hello_init);
module_exit(test_hello_exit);
