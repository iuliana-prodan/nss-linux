/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright 2024 Facultatea de Vrajitorie si Farmece
 */

#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/gpio/consumer.h>

struct led_math_data {
	struct gpio_desc *bit0;
	struct gpio_desc *bit1;
	struct gpio_desc *bit2;
	struct cdev cdev;
};

static void led_math_remove(struct platform_device *pdev)
{
	struct led_math_data *data = platform_get_drvdata(pdev);

	/* TODO: delete chardev */
	/* TODO: unregister chardev region */
}

static int led_math_probe(struct platform_device *pdev)
{
	struct led_math_data *data;
	struct device *dev;

	dev = &pdev->dev;

	dev_info(dev, "LED math driver probe start\n");

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_err(dev, "failed to allocate LED data\n");
		return -ENOMEM;
	}

	/* TODO: get bit0 GPIO handle */
	/* TODO: get bit1 GPIO handle */
	/* TODO: get bit2 GPIO handle */

	/* TODO: register chardev region */
	/* TODO: initialize and add chardev */

	platform_set_drvdata(pdev, data);

	dev_info(dev, "LED math driver probe end\n");

	return 0;
}

static const struct of_device_id led_math_of_match[] = {
	{ .compatible = "nss,led-math", },
	{ /* sentinel */ },
};

static struct platform_driver led_math_driver = {
	.probe = led_math_probe,
	.remove_new = led_math_remove,
	.driver = {
		.name = "nss_led_math",
		.of_match_table = led_math_of_match,
	},
};

module_platform_driver(led_math_driver);

MODULE_DESCRIPTION("LED math driver");
MODULE_AUTHOR("Facultatea de Vrajitorie si Farmece");
MODULE_LICENSE("GPL");
