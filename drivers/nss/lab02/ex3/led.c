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
static ssize_t	led_write(struct file *, const char *, size_t, loff_t *);

static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.write		= led_write
};

static void led_remove(struct platform_device *pdev)
{
	struct led_data *data = platform_get_drvdata(pdev);

	unregister_chrdev_region(MKDEV(500, 0), 1);
	cdev_del(&data->cdev);
}

static int led_probe(struct platform_device *pdev)
{
	struct led_data *data;
	struct device *dev;

	dev = &pdev->dev;

	dev_info(dev, "LED driver probe start\n");

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_err(dev, "failed to allocate LED data\n");
		return -ENOMEM;
	}

	/* TODO: get GPIO handle */

	/* register chardev region */
	register_chrdev_region(MKDEV(500, 0), 1 , "led");

	/* initialize and add chardev */
	cdev_init(&data->cdev, &led_fops);
	data->cdev.owner = THIS_MODULE;
	cdev_add(&data->cdev, MKDEV(500, 0), 1);
	dev_info(dev, "Created char dev with MAJOR 500 and MINOR 0\n");

	platform_set_drvdata(pdev, data);
	my_led = data;

	dev_info(dev, "LED driver probe end\n");

	return 0;
}

static ssize_t led_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	pr_info("Got buffer = [%s], len = %zd\n", buffer, len);

	/* TODO: Use gpiod_set_value() based on the strings "on"/"off" got from userspace */

	return len;
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
