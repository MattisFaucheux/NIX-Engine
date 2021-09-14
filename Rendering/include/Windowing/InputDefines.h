#pragma once

/*Joystick hat states*/
#define JOYSTICK_HAT_CENTERED           0
#define JOYSTICK_HAT_UP                 1
#define JOYSTICK_HAT_RIGHT              2
#define JOYSTICK_HAT_DOWN               4
#define JOYSTICK_HAT_LEFT               8
#define JOYSTICK_HAT_RIGHT_UP           (JOYSTICK_HAT_RIGHT | JOYSTICK_HAT_UP)
#define JOYSTICK_HAT_RIGHT_DOWN         (JOYSTICK_HAT_RIGHT | JOYSTICK_HAT_DOWN)
#define JOYSTICK_HAT_LEFT_UP            (JOYSTICK_HAT_LEFT  | JOYSTICK_HAT_UP)
#define JOYSTICK_HAT_LEFT_DOWN          (JOYSTICK_HAT_LEFT  | JOYSTICK_HAT_DOWN)

/* Modifier key flags */
#define KEY_MOD_SHIFT           0x0001
#define KEY_MOD_CONTROL         0x0002
#define KEY_MOD_ALT             0x0004
#define KEY_MOD_SUPER           0x0008
#define KEY_MOD_CAPS_LOCK       0x0010
#define KEY_MOD_NUM_LOCK        0x0020

/* Joysticks */
#define JOYSTICK_1             0
#define JOYSTICK_2             1
#define JOYSTICK_3             2
#define JOYSTICK_4             3
#define JOYSTICK_5             4
#define JOYSTICK_6             5
#define JOYSTICK_7             6
#define JOYSTICK_8             7
#define JOYSTICK_9             8
#define JOYSTICK_10            9
#define JOYSTICK_11            10
#define JOYSTICK_12            11
#define JOYSTICK_13            12
#define JOYSTICK_14            13
#define JOYSTICK_15            14
#define JOYSTICK_16            15
#define JOYSTICK_LAST          JOYSTICK_16
