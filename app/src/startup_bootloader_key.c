/*
 * Copyright (c) 2023 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_startup_bootloader_key

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/reset.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define BOOTLOADER_KEY_POSITION(n) DT_INST_PROP_OR(n, key_position, 0),

static const int bootloader_key_positions[] = {
    DT_INST_FOREACH_STATUS_OKAY(BOOTLOADER_KEY_POSITION)};

static int64_t timeout_uptime;

static int timeout_init(const struct device *device) {
    timeout_uptime = k_uptime_get() + CONFIG_ZMK_STARTUP_BOOTLOADER_KEY_TIMEOUT_MS;
    return 0;
}

SYS_INIT(timeout_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

static int event_listener(const zmk_event_t *eh) {
    const int64_t uptime = k_uptime_get();
    if (uptime > timeout_uptime) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev && ev->state) {
        for (int i = 0; i < ARRAY_SIZE(bootloader_key_positions); i++) {
            if (ev->position == bootloader_key_positions[i]) {
                zmk_reset(ZMK_RESET_BOOTLOADER);
            }
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(startup_bootloader_key, event_listener);
ZMK_SUBSCRIPTION(startup_bootloader_key, zmk_position_state_changed);
