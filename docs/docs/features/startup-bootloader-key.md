---
title: Startup Bootloader Key
sidebar_label: Startup Bootloader Key
---

The startup bootloader key feature reboots the keyboard into the bootloader if a specific key is held while powering on the keyboard. This is useful for keyboards which don't have a physical reset button or where the reset button is difficult to access.

This is similar to the [`&bootloader` behavior](../behaviors/reset.md#bootloader-reset), except it doesn't take up a spot in your keymap since it overrides the behavior of the key just for a moment at startup. It also works on the peripheral side of a split keyboard even when it isn't connected to the central side.

The startup bootloader key can be used as the primary way to enter the bootloader, or it can be a fallback in case you flash a broken keymap and can't access your `&bootloader` key.

## Adding a Bootloader Key

To enable this feature on a new board or shield, add a `zmk,startup-bootloader-key` node to your board's `.dts` file or shield's `.overlay` file and select which key will trigger it with the `key-position` property.

You can also enable the feature for any keyboard by adding it to your `.keymap` file.

```
/ {
    ...
    bootloader_key: bootloader_key {
        compatible = "zmk,startup-bootloader-key";
        key-position = <0>;
    };
    ...
};
```

:::info

Key positions are numbered like the keys in your keymap, starting at 0. So, if the first key in your keymap is `Q`, this key is in position `0`. The next key (possibly `W`) will have position 1, etcetera.

:::

### Split Keyboards

For split keyboards, you can define multiple bootloader keys and then only enable the correct key for each side. For example, if key 0 is the top-left key on the left side and key 11 is the top-right key on the right side, you would use:

**shield.dtsi**

```
/ {
    ...
    bootloader_key_left: bootloader_key_left {
        compatible = "zmk,startup-bootloader-key";
        key-position = <0>;
        status = "disabled";
    };

    bootloader_key_right: bootloader_key_right {
        compatible = "zmk,startup-bootloader-key";
        key-position = <11>;
        status = "disabled";
    };
    ...
};
```

**shield_left.overlay**

```
#include "shield.dtsi"

&bootloader_key_left {
    status = "okay";
};
```

**shield_right.overlay**

```
#include "shield.dtsi"

&bootloader_key_right {
    status = "okay";
};
```

## Startup Timeout

By default, the startup bootloader key feature watches for the trigger key for 500 ms. You can change this timeout with `CONFIG_ZMK_STARTUP_BOOTLOADER_KEY_TIMEOUT_MS` if it isn't reliably triggering, for example if you have some board-specific initialization code which takes a while.

To change the value for a new board or shield, set this option in your `Kconfig.defconfig` file:

```
config ZMK_STARTUP_BOOTLOADER_KEY_TIMEOUT_MS
	default 1000
```

You can also set it from your keyboard's `.conf` file in a user config repo:

```ini
CONFIG_ZMK_STARTUP_BOOTLOADER_KEY_TIMEOUT_MS=1000
```
