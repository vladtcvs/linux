// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2026 Vladislav Tsendrovskii
 * Author: Vladislav Tsendrovskii <vtcendrovskii@gmail.com>
 */

#include <linux/module.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/of_graph.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_crtc.h>
#include <drm/drm_edid.h>
#include <drm/drm_print.h>
#include <drm/drm_probe_helper.h>

struct tfp401a_bridge {
	struct drm_bridge		bridge;

	struct regulator	*vdd;
	struct gpio_desc	*pd;
    struct gpio_desc	*pdo;
};

static inline struct tfp401a_bridge *bridge_to_tfp401a(struct drm_bridge *bridge)
{
	return container_of(bridge, struct tfp401a_bridge, bridge);
}

static int tfp401a_attach(struct drm_bridge *bridge,
						  struct drm_encoder *encoder,
                          enum drm_bridge_attach_flags flags)
{
	DRM_INFO("Start tfp401a_attach");
	struct tfp401a_bridge *tfp = bridge_to_tfp401a(bridge);

	if (flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR) {
		int ret = drm_bridge_attach(encoder, tfp->bridge.next_bridge, bridge, flags);
		return ret;
	}

	DRM_ERROR("No flag DRM_BRIDGE_ATTACH_NO_CONNECTOR - error");
	return -EINVAL;
}

static void tfp401a_atomic_enable(struct drm_bridge *bridge,
				  struct drm_atomic_commit *commit)
{
	DRM_INFO("tfp401a enable");
	struct tfp401a_bridge *tfp = bridge_to_tfp401a(bridge);
	int ret;

	if (tfp->vdd) {
		ret = regulator_enable(tfp->vdd);
		if (ret)
			DRM_ERROR("Failed to enable vdd regulator: %d\n", ret);
	}

	if (tfp->pd) {
		gpiod_set_value_cansleep(tfp->pd, 1);
	}

	if (tfp->pdo) {
		gpiod_set_value_cansleep(tfp->pdo, 1);
	}

	msleep(200); // wait for lock
}

static void tfp401a_atomic_disable(struct drm_bridge *bridge,
				   struct drm_atomic_commit *commit)
{
	DRM_INFO("tfp401a disable");
	struct tfp401a_bridge *tfp = bridge_to_tfp401a(bridge);
	int ret;

	if (tfp->pd) {
		gpiod_set_value_cansleep(tfp->pd, 0);
	}

	if (tfp->pdo) {
		gpiod_set_value_cansleep(tfp->pdo, 0);
	}

	if (tfp->vdd) {
		ret = regulator_disable(tfp->vdd);
		if (ret)
			DRM_ERROR("Failed to disable vdd regulator: %d\n", ret);
	}
}

static const struct drm_bridge_funcs tfp401a_bridge_funcs = {
        .atomic_create_state    = drm_atomic_helper_bridge_create_state,
        .atomic_destroy_state   = drm_atomic_helper_bridge_destroy_state,
        .atomic_duplicate_state = drm_atomic_helper_bridge_duplicate_state,
	.attach			= tfp401a_attach,
	.atomic_enable		= tfp401a_atomic_enable,
	.atomic_disable		= tfp401a_atomic_disable,
};

static int tfp401a_bridge_probe(struct platform_device *pdev)
{
	struct tfp401a_bridge *tfp;

	tfp = devm_drm_bridge_alloc(&pdev->dev, struct tfp401a_bridge, bridge,
				    &tfp401a_bridge_funcs);
	if (IS_ERR(tfp))
		return PTR_ERR(tfp);

	tfp->bridge.next_bridge = devm_drm_of_get_bridge(&pdev->dev, pdev->dev.of_node, 0, 0);
	if (IS_ERR(tfp->bridge.next_bridge))
		return PTR_ERR(tfp->bridge.next_bridge);

	DRM_INFO("TFP401A attaching next bridge: %s", tfp->bridge.next_bridge->of_node->full_name);

	tfp->bridge.type = DRM_MODE_CONNECTOR_DPI;
	tfp->bridge.ops = 0;
	tfp->bridge.ycbcr_420_allowed = false;
	tfp->bridge.interlace_allowed = false;
	tfp->bridge.support_hdcp = false;
	tfp->bridge.vendor = "TI";
	tfp->bridge.product = "TFP401A";
	tfp->bridge.of_node = pdev->dev.of_node;

	int ret = devm_drm_bridge_add(&pdev->dev, &tfp->bridge);
	if (ret < 0)
		dev_err(&pdev->dev, "failed to add tfp401a\n");
	return ret;
}

static const struct of_device_id tfp401a_bridge_match[] = {
	{
		.compatible = "ti,tfp401a",
		.data = (void*)NULL,
	},
	{},
};
MODULE_DEVICE_TABLE(of, tfp401a_bridge_match);

static struct platform_driver tfp401a_bridge_driver = {
	.probe	= tfp401a_bridge_probe,
	.driver		= {
		.name		= "tfp401a-bridge",
		.of_match_table	= tfp401a_bridge_match,
	},
};

module_platform_driver(tfp401a_bridge_driver);

MODULE_AUTHOR("Vladislav Tsendrovskii <vtcendrovskii@gmail.com>");
MODULE_DESCRIPTION("TFP401a bridge driver");
MODULE_LICENSE("GPL");
