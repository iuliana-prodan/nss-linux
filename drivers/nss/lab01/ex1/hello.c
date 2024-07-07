#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/** The LKM initialization function */
/* TODO: implement the initialization function */

/** The LKM cleanup function */
/* TODO: implement the cleanup function */

/** A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function
 */
module_init(hello_init);
module_exit(hello_exit);

/** Module information */
/* TODO: add description, author, license, version */
