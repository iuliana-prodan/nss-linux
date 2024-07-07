#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/* only one minor */
#define MAX_DEV 	1
/* message size supported */
#define MAX_SIZE	10

/* TODO: Follow comments marked with TODO and implement them. */

/* The prototype functions for the character driver -- must come before the struct definition */
static int	imx8mq_chardev_open(struct inode *, struct file *);
static int	imx8mq_chardev_release(struct inode *, struct file *);
static ssize_t	imx8mq_chardev_read(struct file *, char *, size_t, loff_t *);
static ssize_t	imx8mq_chardev_write(struct file *, const char *, size_t, loff_t *);

/* TODO 1: complete the callbacks for each operation */
/** Devices are represented as file structure in the kernel.
 *  The file_operations structure from /linux/fs.h lists the callback functions
 *  that you wish to associated with your file operations.
 *  char devices usually implement open, read, write and release callbacks.
 */
static const struct file_operations imx8mq_chardev_fops = {
	.owner		= THIS_MODULE,
	.open		= ,
	.release	= ,
	.read		= ,
	.write		=
};

struct imx8mq_char_device_data {
	struct cdev cdev;
};

static int 	dev_major = 0; // Stores the device number
static char	message[MAX_SIZE + 1] = {0}; // Memory for the string that is passed from userspace
static uint32_t	numberOpens = 0; // Counts the number of times the device is opened
static uint32_t	numberReads = 0; // Counts the number of reads from user space to kernel space

static struct class *imx8mq_chardev_class = NULL; // The device-driver class struct pointer
static struct device* imx8mq_chardev = NULL; // The device-driver device struct pointer

static struct imx8mq_char_device_data imx8mq_chardev_data;

/** The LKM initialization function */
static int __init imx8mq_chardev_init(void)
{
	int err;
	dev_t dev;

	printk(KERN_INFO "imx8mq_chardev: Initializing the imx8mq_chardev LKM\n");

	/* TODO 2: dynamically allocate a major number for the device. Check for invalid number */
	/* get major number */
	/* print major and minor numbers */

	/* Register the device class */
	imx8mq_chardev_class = class_create(THIS_MODULE, "imx8mq_chardev");
	if (IS_ERR(imx8mq_chardev_class)){ // Check for error and clean up if there is
		err = PTR_ERR(imx8mq_chardev_class); // Correct way to return an error on a pointer
		printk(KERN_ALERT "Failed to register device class %d\n", err);
		goto err_class;
	}
	printk(KERN_INFO "imx8mq_chardev: device class registered correctly\n");

	/* Initialize the cdev structure */
	cdev_init(&imx8mq_chardev_data.cdev, &imx8mq_chardev_fops);
	imx8mq_chardev_data.cdev.owner = THIS_MODULE;

	/* Register the character device with the kernel */
	err = cdev_add(&imx8mq_chardev_data.cdev, dev, 1);
	if (err) {
		printk(KERN_ALERT "Failed to add char device %d\n", err);
		goto err_cdev;
	}
	/* Register the device driver */
	imx8mq_chardev = device_create(imx8mq_chardev_class, NULL, dev, NULL, "imx8mq_chardev-%d", MINOR(dev));
	if (IS_ERR(imx8mq_chardev)) {
		err = PTR_ERR(imx8mq_chardev);
		printk(KERN_ALERT "Failed to create the device-%d %d\n", MINOR(dev), err);
		goto err;
	}

	printk(KERN_INFO "imx8mq_chardev: Added dev [%d,%d]\n", MAJOR(dev), MINOR(dev));

	return 0;

err:
	cdev_del(&imx8mq_chardev_data.cdev);
err_cdev:
	class_destroy(imx8mq_chardev_class);
err_class:
	unregister_chrdev_region(MKDEV(dev_major, 0), 1);

	return err;
}

/** The LKM cleanup function */
static void __exit imx8mq_chardev_exit(void)
{
	device_destroy(imx8mq_chardev_class, MKDEV(dev_major, 0)); // Remove the device
	class_unregister(imx8mq_chardev_class); // Unregister the device class
	class_destroy(imx8mq_chardev_class); // Remove the device class

	/* TODO 3: unregister the major number */

	printk(KERN_INFO "imx8mq_chardev: Goodbye from imx8mq_chardev LKM!\n");
}

/** The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int imx8mq_chardev_open(struct inode *inode, struct file *file)
{
	/* TODO 4: increment the numberOpens counter and
	 * display a message with the number of opens. */

	return 0;
}

/** The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int imx8mq_chardev_release(struct inode *inode, struct file *file)
{
	/* TODO 5: display a message */

	return 0;
}

/** This function is called whenever device is being read from user space
 *  i.e. data is being sent from the device to the user.
 *  In this case it uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the buffer
 *  @param offset The offset if required
 *  @return ssize_t Number of bytes read
 */
static ssize_t imx8mq_chardev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{

	int error_count = 0;
	char *msg = "NXP Summer School";
	numberReads++;

	if ((numberReads & 1) == 0)
		return 0;

	// get min length of the message send to user
	len = min(len, strlen(msg) + 1);

	/* TODO 6: Use the copy_to_user() function to copy information from kernel space to user space.
	 * Check for errors.
	 * copy_to_user has the format ( * to, *from, size) and returns 0 on success */

	// copy_to_user has the format ( * to, *from, size) and returns 0 on success
	// succeeded -> return number of bytes read
	// failed -> still return number of bytes read, but see copy_to_user return
}

/** This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 *  @return ssize_t Number of bytes written
 */
static ssize_t imx8mq_chardev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	size_t ncopied;

	/* TODO 7: Get the message from user space: copy_from_user has the format ( * to, *from, size) and returns 0 on success
	 * Display a message with the message and the number of characters received from user. */

	return len;
}

/** A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(imx8mq_chardev_init);
module_exit(imx8mq_chardev_exit);

MODULE_DESCRIPTION("A simple Linux char driver for the i.MX8MQ");	// The description -- see modinfo
MODULE_LICENSE("GPL");			// The license type -- this affects available functionality
MODULE_AUTHOR("Jane Doe");		// The author -- visible when you use modinfo
MODULE_VERSION("0.1");			// A version number to inform users
