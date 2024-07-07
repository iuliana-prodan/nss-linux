#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* TODO 1: declare the variable that will take the value of the command
 * line argument as global */

/* TODO 2: module_param() macro is used to set the mechanism up -
 * takes 3 arguments: the name of the variable, its type and permissions
 * for the corresponding file in sysfs */

/* TODO 3: MODULE_PARM_DESC() is used to document the argument that
 * the module can take - takes 2 parameters: a variable name and a free
 * form string describing that variable */


/** Module parameter */
/* TODO: declare variable and set it as module parameter */


/** The LKM initialization function */
static int __init hello_init(void)
{
	printk(KERN_INFO "Hello %s from the i.MX8MQ LKM!\n", name);
	return 0;
}

/** The LKM cleanup function */
static void __exit hello_exit(void)
{
	printk(KERN_INFO "Goodbye %s from the i.MX8MQ LKM!\n", name);
}

/** A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function
 */
module_init(hello_init);
module_exit(hello_exit);

/** Module information */
MODULE_DESCRIPTION("A Linux kernel module for i.MX8MQ, with parameter.");	// The description -- see modinfo
MODULE_LICENSE("GPL");		// The license type -- this affects runtime behavior
MODULE_AUTHOR("Jane Doe");	// The author -- visible when you use modinfo
MODULE_VERSION("0.1");		// The version of the module
