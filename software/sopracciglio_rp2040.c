//sopracciglio_rp2040.c
//author: mike kushnerik
//date: 9/25/2026

#include "sopracciglio_rp2040.h"

void sopracciglio_rp2040_init(void)
{
  //tooth leds
  pinMode(T_LED_1, OUTPUT);
  pinMode(T_LED_2, OUTPUT);
  pinMode(T_LED_3, OUTPUT);
  pinMode(T_LED_4, OUTPUT);
  pinMode(T_LED_5, OUTPUT);
  pinMode(T_LED_6, OUTPUT);
  digitalWrite(T_LED_1, LOW);
  digitalWrite(T_LED_2, LOW);
  digitalWrite(T_LED_3, LOW);
  digitalWrite(T_LED_4, LOW);
  digitalWrite(T_LED_5, LOW);
  digitalWrite(T_LED_6, LOW);

  //acc
  pinMode(ACC_INT1, INPUT);
  pinMode(ACC_INT2, INPUT);

  //rtc
  pinMode(RTC_INT, INPUT);
  pinMode(RTC_CLKOUT, INPUT);
  pinMode(RTC_EVI, OUTPUT);
  digitalWrite(RTC_EVI, LOW);

  //cap
  pinMode(CAP_ALERT, INPUT);

  //sao
  pinMode(SAO_GPIO1, OUTPUT);
  pinMode(SAO_GPIO2, OUTPUT);
  digitalWrite(SAO_GPIO1, LOW);
  digitalWrite(SAO_GPIO2, LOW);

  //stat led
  pinMode(STAT_LED, OUTPUT);

  //button switch
  pinMode(BUTTON_SW, INPUT_PULLUP);
  pinMode(BUTTON_SW_LED, OUTPUT);

  //motion
  pinMode(MOTION_LED, OUTPUT);

  //mic
  pinMode(MIC_LED, OUTPUT);
}