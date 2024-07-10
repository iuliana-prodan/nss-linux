/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright 2024 Facultatea de Vrajitorie si Farmece
 */

#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/gpio/consumer.h>

struct led_data {
	struct gpio_desc *led;
	struct cdev cdev;
};

struct led_data *my_led;

static ssize_t	led_read(struct file *, char *, size_t, loff_t *);
static ssize_t	led_write(struct file *, const char *, size_t, loff_t *);

static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
//	.open		= led_open,
	.read		= led_read,
	.write		= led_write
};

static void led_remove(struct platform_device *pdev)
{
	struct led_data *data = platform_get_drvdata(pdev);
	dev_t devno;

	/* TODO: delete chardev */
	/* TODO: unregister chardev region */
	devno = MKDEV(500, 0);
	unregister_chrdev_region(devno, 1);
	cdev_del(&data->cdev);
}

static int led_probe(struct platform_device *pdev)
{
	struct led_data *data;
	struct device *dev;

	dev_t devno;
	dev = &pdev->dev;

	dev_info(dev, "LED driver probe start\n");

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_err(dev, "failed to allocate LED data\n");
		return -ENOMEM;
	}

	devno = MKDEV(500, 0);

	/* TODO: get GPIO handle */
	data->led = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
	if (IS_ERR(data->led)) {
	    dev_err(dev, "LED error\n");
	}

	/* TODO: register chardev region */
	register_chrdev_region(devno, 1 , "led");

	/* TODO: initialize and add chardev */
	cdev_init(&data->cdev, &led_fops);
	data->cdev.owner = THIS_MODULE;
	cdev_add(&data->cdev, devno, 1);

	platform_set_drvdata(pdev, data);

	my_led = data;

	dev_info(dev, "LED driver probe end\n");

	return 0;
}

static ssize_t led_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    int val_ret = 0;
    int led = 0;
    char *msg;

    len = min(len, strlen(msg) + 1);

    led = gpiod_get_value(my_led->led);
    if (led == 1)
	msg = "on";
    else if (led == 0)
	msg = "off";
    else
	msg = "err";

    val_ret = copy_to_user(buffer, msg, len);
    if (val_ret == 0)
	return len;
    else
	return len - val_ret;
}

static ssize_t led_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	pr_info("buffer = %s\n", buffer);

	if(!strcmp(buffer, "on"))
	    gpiod_set_value(my_led->led, 1);
	else if (!strcmp(buffer, "off"))
	    gpiod_set_value(my_led->led, 0);
	else
	    printk("Invalid value from user\n");

	return 0;
}

static const struct of_device_id led_of_match[] = {
	{ .compatible = "nss,led", },
	{ /* sentinel */ },
};

static struct platform_driver led_driver = {
	.probe = led_probe,
	.remove_new = led_remove,
	.driver = {
		.name = "nss_led",
		.of_match_table = led_of_match,
	},
};

module_platform_driver(led_driver);

MODULE_DESCRIPTION("LED warm-up driver");
MODULE_AUTHOR("Facultatea de Vrajitorie si Farmece");
MODULE_LICENSE("GPL");
