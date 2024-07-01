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

static void led_remove(struct platform_device *pdev)
{
	struct led_data *data = platform_get_drvdata(pdev);

	/* TODO: delete chardev */
	/* TODO: unregister chardev region */
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
	/* TODO: register chardev region */
	/* TODO: initialize and add chardev */

	platform_set_drvdata(pdev, data);

	dev_info(dev, "LED driver probe end\n");

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
