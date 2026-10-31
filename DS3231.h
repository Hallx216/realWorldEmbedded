/*
 * DS3231.h
 * Copyright (c) 2025 Derek Molloy (www.derekmolloy.ie)
 * Modified by: Student Name
 */

#ifndef DS3231_H_
#define DS3231_H_
#include"I2CDevice.h"
#include <stdint.h>
#include <iostream>


namespace een1071 {

class DS3231:public I2CDevice{

public:
#define busNumber      1
#define deviceAddress  0x68

 //structure of registers for direct referencing
   /* typedef struct
    {
       uint32_t seconds;
       uint8_t minutes;
       uint8_t hours;                           
       uint8_t day;                           
       uint8_t date;                           
       uint8_t month;                          
       uint8_t year;                           
       uint8_t alarm1SecondInterval_reg;       
       uint8_t alarm1MinuteInterval_reg;       
       uint8_t alarm1HourInterval_reg;         
       uint8_t alarm1DayDateInterval_reg;      
       uint8_t alarm2MinuteInterval_reg;       
       uint8_t alarm2HourInterval_reg;         
       uint8_t alarm2DayDateInterval_reg;      
       uint8_t control_reg;                    
       uint8_t status_reg;                     
       uint8_t tempUpperByte_reg;              
       uint8_t tempLowerByte_reg;              
      
   }DS3231Values_RegisterAddr;
*/
   typedef struct{

      unsigned int seconds=0x00;
      unsigned int minutes=0x01;
      unsigned int hours=0x02;
      unsigned int day=0x03;
      unsigned int date=0x04;
      unsigned int month=0x05;
      unsigned int year=0x06;
      unsigned int alarm1SecondInterval_reg=0x07;
      unsigned int alarm1MinuteInterval_reg=0x08;       
      unsigned int alarm1HourInterval_reg=0x09;         
      unsigned int alarm1DayDateInterval_reg=0x0A;      
      unsigned int alarm2MinuteInterval_reg=0x0B;       
      unsigned int alarm2HourInterval_reg=0x0C;         
      unsigned int alarm2DayDateInterval_reg=0x0D;      
      unsigned int control_reg=0x0E;                    
      unsigned int status_reg=0x0F;                     
      unsigned int tempInteger=0x11;
      unsigned int tempDecimal=0x12;

   }DS3231_RegisterAddr;
   
    
   DS3231():I2CDevice(busNumber, deviceAddress){
      std::cout<<"I2C Initialized"<<std::endl;
   }
   void readAndDisplayTimeDateTemp();
   int bcdToDecimal(unsigned char bcd);
   void setTimeDate();
   unsigned char decimalToBCD(int decimal);
   void alarmOncePerSecond();
   void alarmLEDEnableINT();
   void toggleLED();
   void alarmMinutesMatch();
   void squareWaveFreqSelect();
   void syncNtpDs3231();
   
};

} /* namespace een1071 */

#endif
