//sopracciglio_rp2040_test_v1.ino
//author: mike kushnerik
//date: 9/25/2026

#include "sopracciglio_rp2040.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>

#define BUTTON_SW_POLL_DELAY    20
#define ACC_POLL_DELAY          100
#define MOTION_LED_THRESHOLD    15
#define MIC_POLL_DELAY          1
#define MIC_SAMPLE_COUNT        50
#define MIC_INPUT_THRESHOLD     15000

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

unsigned long currentMillis=0;
unsigned long previousMillis=0;

uint16_t buttonSwPollCounter=0;
uint8_t currentButtonSwState=1;
uint8_t previousButtonSwState=0;

uint16_t accPollCounter=0;

uint16_t micPollCounter=0;
uint16_t sample;
uint8_t sampleCount=0;
uint16_t peakToPeak=0;
uint16_t signalMax=0;
uint16_t signalMin=4096;

void setup()
{
  //debug
  Serial.begin();

  //init pins
  sopracciglio_rp2040_init();

  //init onboard i2c
  Wire.setSDA(I2C0_SDA);
  Wire.setSCL(I2C0_SCL);
  Wire.begin();

  lis.begin(0x18);

  analogWriteResolution(16);
}

void loop()
{
  currentMillis = millis();
  if((currentMillis-previousMillis) >= 1)
  {
    buttonSwPollCounter++;
    if(buttonSwPollCounter>BUTTON_SW_POLL_DELAY)
    {
      pollButtonSwitch();
      updateToothLeds(readToothSwitches());
      buttonSwPollCounter=0;
    }

    accPollCounter++;
    if(accPollCounter>ACC_POLL_DELAY)
    {
      updateMotionLed();
      accPollCounter=0;
    }

    micPollCounter++;
    if(micPollCounter>MIC_POLL_DELAY)
    {
      sample=analogRead(MIC_AUDIO_IN);
      if(sample > signalMax)
      {
        signalMax = sample;
      }
      else if(sample < signalMin)
      {
        signalMin = sample;
      }

      sampleCount++;
      if(sampleCount > MIC_SAMPLE_COUNT)
      {
        peakToPeak = signalMax - signalMin;

        uint16_t micLed = map(peakToPeak, 0, 700, 0, 65536);
        
        if(micLed > MIC_INPUT_THRESHOLD)
        {
          analogWrite(MIC_LED, micLed);
        }
        else 
        {
          analogWrite(MIC_LED, 0);
        }
        //Serial.println(micLed);

        sampleCount=0;
        sample=0;
        signalMax=0;
        signalMin = 4096;
      }

      micPollCounter=0;
    }

    previousMillis=currentMillis;
  }
}

void loop1()
{
  //use this core for sao stuff??
  digitalWrite(STAT_LED, HIGH);
  delay(1000);
  digitalWrite(STAT_LED, LOW);
  delay(1000);
}

void pollButtonSwitch(void)
{
  currentButtonSwState=digitalRead(BUTTON_SW);

  //falling edge
  if(currentButtonSwState == LOW && previousButtonSwState == HIGH)
  {
    //toggle led
    digitalWrite(BUTTON_SW_LED, !digitalRead(BUTTON_SW_LED));
  }

  previousButtonSwState=currentButtonSwState;
}

//SW1: 0bxxxxxxx1
//SW2: 0bxxxxxx1x
//SW3: 0bxxxxx1xx
//SW4: 0bxxxx1xxx
//SW5: 0bxxx1xxxx
//SW6: 0bxx1xxxxx
uint8_t readToothSwitches(void)
{
  uint8_t sensor_status=0;

  Wire.beginTransmission(CAP1206_ADDR);
  Wire.write(CAP1206_SENSOR_INPUT_STATUS);
  Wire.endTransmission(false);

  Wire.requestFrom(CAP1206_ADDR,1);

  if(Wire.available())
  {
    sensor_status = Wire.read();
    //Serial.println(sensor_status, BIN);
  }

  //clear interrupts after reading status
  Wire.beginTransmission(CAP1206_ADDR);
  Wire.write(CAP1206_MAIN_CONTROL);
  Wire.endTransmission(false);

  Wire.requestFrom(CAP1206_ADDR,1);

  if(Wire.available())
  {
    uint8_t main_control = Wire.read();
    //clear int bit
    main_control = main_control & ~(1);

    //write register back to cap1206
    Wire.beginTransmission(CAP1206_ADDR);
    Wire.write(CAP1206_MAIN_CONTROL);
    Wire.write(main_control);
    Wire.endTransmission(false);
  }

  return sensor_status;
}

void updateToothLeds(uint8_t val)
{
  digitalWrite(T_LED_1, val & (1));
  digitalWrite(T_LED_2, (val & (1<<1)) >> 1);
  digitalWrite(T_LED_3, (val & (1<<2)) >> 2);
  digitalWrite(T_LED_4, (val & (1<<3)) >> 3);
  digitalWrite(T_LED_5, (val & (1<<4)) >> 4);
  digitalWrite(T_LED_6, (val & (1<<5)) >> 5);
}

void updateMotionLed(void)
{
  if(lis.haveNewData())
  {
    sensors_event_t event;
    lis.getEvent(&event);

    uint16_t motionSum = (abs(event.acceleration.x)+abs(event.acceleration.y)+abs(event.acceleration.z));

    //Serial.println(motionSum);

    if(motionSum > MOTION_LED_THRESHOLD)
    {
      digitalWrite(MOTION_LED, HIGH);
    }
    else
    {
      digitalWrite(MOTION_LED, LOW);
    }
  }
}