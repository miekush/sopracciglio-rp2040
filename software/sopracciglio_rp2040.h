//sopracciglio_rp2040.h
//author: mike kushnerik
//date: 9/25/2026

#ifndef _SOPRACCIGLIO_RP2040_H_
#define _SOPRACCIGLIO_RP2040_H_

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

//onboard i2c bus for rtc, cap touch controller, and accelerometer
#define I2C0_SDA        4
#define I2C0_SCL        5

//i2c addresses
#define CAP1206_ADDR    0x28
#define LIS3DH_ADDR     0x30
#define RV3028_ADDR     0x52

//cap1206 registers
#define CAP1206_MAIN_CONTROL          0x00
#define CAP1206_GENERAL_STATUS        0x02
#define CAP1206_SENSOR_INPUT_STATUS   0x03

//tooth leds
#define T_LED_1         0
#define T_LED_2         1
#define T_LED_3         2
#define T_LED_4         11
#define T_LED_5         12
#define T_LED_6         13

//accelerometer pins
#define ACC_INT1        10
#define ACC_INT2        3

//rtc pins
#define RTC_INT         6
#define RTC_CLKOUT      7
#define RTC_EVI         8

//cap touch pins
#define CAP_ALERT       9

//sao pins
#define SAO_GPIO1       22
#define SAO_GPIO2       24
#define SAO_I2C_SDA     18
#define SAO_I2C_SCL     19

//onboard leds
#define STAT_LED        25

//microphone
#define MIC_AUDIO_IN    26

/// badge i/o ///

//button switch
#define BUTTON_SW       15
#define BUTTON_SW_LED   16

//motion
#define MOTION_LED      17

//microphone
#define MIC_LED         28

//function prototypes
void sopracciglio_rp2040_init(void);

#ifdef __cplusplus
}
#endif

#endif
