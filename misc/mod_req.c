#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/printk.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("shashank parsi");
MODULE_DESCRIPTION("load module on demand");
MODULE_VERSION("0.1");

static int __init test_hello_init(void)
{
	if(request_module("cpuid") > 0)
		pr_info("requested module failed to load\n");
	else
		pr_info("requested module successfully loaded\n");
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("good bye\n");
}
module_init(test_hello_init);
module_exit(test_hello_exit);
