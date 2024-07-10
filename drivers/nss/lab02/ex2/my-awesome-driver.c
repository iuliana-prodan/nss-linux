/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright 2024 Facultatea de Vrajitorie si Farmece
 */

#include <linux/of.h>
#include <linux/platform_device.h>

/* TODO1: declare the struct of_device_id array */
static const struct of_device_id my_awesome_driver_ids[] = {
	{ .compatible = "nss,my-awesome-device", },
	{ },
};

static int my_probe(struct platform_device *pdev)
{
	printk("Hello World\n");
	return 0;
}

/* TODO2: create the platform driver structure */
static struct platform_driver my_awesome_driver = {
	.probe		= my_probe,
	.driver = {
		.name	= "my_awesome_driver",
		.of_match_table = my_awesome_driver_ids,
	},
};

/* TODO3: create the init/exit functions */
static int __init my_init(void)
{
	int ret;

	ret = platform_driver_register(&my_awesome_driver);
	if (ret)
		printk("error\n");
	else
		printk("ok\n");

	return ret;
}

static void __exit my_exit(void)
{
	platform_driver_unregister(&my_awesome_driver);
}

module_platform_driver(my_awesome_driver);

//module_init(my_init);
//module_exit(my_exit);

MODULE_DESCRIPTION("My very awesome platform driver");
MODULE_AUTHOR("Facultatea de Vrajitorie si Farmece");
MODULE_LICENSE("GPL");
