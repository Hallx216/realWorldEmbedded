/*
 * DS3231.cpp 
 * Copyright (c) 2025 Derek Molloy (www.derekmolloy.ie)
 * Modified by: Student Name
 */

 #include "DS3231.h"
 #include <iostream>
 #include <unistd.h>
 #include <math.h>
 #include <stdio.h>
 #include <ctime>
 
 using namespace std;
 
 
 namespace een1071 {
 
    //DS3231::DS3231Values_RegisterAddr volatile *const ds3231BaseRegAddr=(DS3231::DS3231Values_RegisterAddr*) 0x00;  //declared for direct referencing
    DS3231::DS3231_RegisterAddr ds3231RegAddrMap;
    
 
 
    int DS3231:: bcdToDecimal(unsigned char bcdValue) {
       int tens = (bcdValue >> 4) & 0x0F;  
       int ones = bcdValue & 0x0F;       
       return (tens * 10) + ones;
    }
 
    unsigned char DS3231:: decimalToBCD(int decimal) {
       unsigned char bcd = 0;
       int shift = 0;

       while (decimal > 0) {
           unsigned char digit = decimal % 10; 
           bcd |= (digit << shift);  
           decimal /= 10;             
           shift += 4;  
       }
   
       return bcd;
   }
    
    //function to set current time and date
    void DS3231::setTimeDate()
      {
 
       int tsec=0,tmin=0,thour=0,daySet=0,monthSet=0,yearSet=0,dateSet=0;
         cout<<"Ready to take values"<<endl;
         cout<<"Enter seconds to be set"<<endl;
         cin>>tsec;
         cout<<"Enter minutes to be set"<<endl;
         cin>>tmin;
         cout<<"Enter hours to be set"<<endl;
         cin>>thour;
         cout<<"Enter day to be set in between 0-6(0=Sunday .....)"<<endl;
         cin>>daySet;
         cout<<"Enter month to be set"<<endl;
         cin>>monthSet;
         cout<<"Enter year to be set"<<endl;
         cin>>yearSet;
         cout<<"Enter date to be set"<<endl;
         cin>>dateSet;
         writeRegister(ds3231RegAddrMap.seconds,decimalToBCD(tsec));
         writeRegister(ds3231RegAddrMap.minutes,decimalToBCD(tmin));
         writeRegister(ds3231RegAddrMap.hours,decimalToBCD(thour));
         writeRegister(ds3231RegAddrMap.day,decimalToBCD(daySet));
         writeRegister(ds3231RegAddrMap.date,decimalToBCD(dateSet));
         writeRegister(ds3231RegAddrMap.month,decimalToBCD(monthSet));
         writeRegister(ds3231RegAddrMap.year,decimalToBCD(yearSet));
 
    
      }
    
    
    
      //function to read and display current time,date and temperature
     void DS3231::readAndDisplayTimeDateTemp()
      {
         unsigned char timeSeconds=0,timeMinutes=0,timehours=0,readDay,readDate=0,readMonth=0,readYear=0,readTempInteger=0,readTempDecimal=0;
         int dayNumber=0;string dayName;
         
         timeSeconds=readRegister(ds3231RegAddrMap.seconds);
         timeMinutes=readRegister(ds3231RegAddrMap.minutes);
         timehours=readRegister(ds3231RegAddrMap.hours);
         readDay=readRegister(ds3231RegAddrMap.day);
         dayNumber=bcdToDecimal(readDay)+1;    //Added one because in NTP Sunday=1 and so on ....
         switch(dayNumber){
          case 1:       dayName="Sunday";break;
          case 2:       dayName="Monday";break;
          case 3:       dayName="Tuesday";break;
          case 4:       dayName="Wednesday";break;
          case 5:       dayName="Thursday";break;
          case 6:       dayName="Friday";break;
          case 7:       dayName="Saturday";break;
          default:      dayName="Invalid Day";break;
         }
 
         readDate=readRegister(ds3231RegAddrMap.date);
         readMonth=readRegister(ds3231RegAddrMap.month);
         readYear=readRegister(ds3231RegAddrMap.year);
         readTempInteger=readRegister(ds3231RegAddrMap.tempInteger);
         readTempDecimal=readRegister(ds3231RegAddrMap.tempDecimal);
         cout<<"Temperature:"<<bcdToDecimal(readTempInteger)<<endl;
         cout<<"yy:mm:dd"<<" "<<bcdToDecimal(readYear)<<":"<<bcdToDecimal(readMonth)<<":"<<bcdToDecimal(readDate)<<"  "<<"hh:mm:ss"<<" "<<bcdToDecimal(timehours)<<":"<<bcdToDecimal(timeMinutes)<<":"<<bcdToDecimal(timeSeconds)<<"  "<<dayName<<endl;
      }
 
 
      //function for alarm once per second
 
      void DS3231 ::alarmOncePerSecond(){
 
 
       unsigned char statusRegValue=0;
       
       writeRegister(ds3231RegAddrMap.control_reg,0x04); //here i dont know without enabling A1IE its working how?
       writeRegister(ds3231RegAddrMap.alarm1SecondInterval_reg,0x80);
       writeRegister(ds3231RegAddrMap.alarm1MinuteInterval_reg,0x80);
       writeRegister(ds3231RegAddrMap.alarm1HourInterval_reg,0x80);
       writeRegister(ds3231RegAddrMap.alarm1DayDateInterval_reg,0x80);
 
       statusRegValue=readRegister(ds3231RegAddrMap.status_reg);  
       
       if(statusRegValue & 0x01)
       {
          cout<<"Alarm has been configured"<<endl;
       }
       else cout<<"Alarm has not been configured"<<endl;
 
 
      }
 
 
 
      //function to set alarm when minutes match
 
      void DS3231::alarmMinutesMatch(){
 
       unsigned currentMinutes=0,updatedMinutesBCDvalue=0,statusRegA2F=0;
       int updatedMinutesDecimalValue=0;
 
       writeRegister(ds3231RegAddrMap.control_reg,0x06);   //enable bit in control register and also A2IE
       currentMinutes=readRegister(ds3231RegAddrMap.minutes);  //read the current minutes
       updatedMinutesDecimalValue=bcdToDecimal(currentMinutes)+1;          //update the minutes by adding 1
       updatedMinutesBCDvalue=decimalToBCD(updatedMinutesDecimalValue);   //setting bit 7 of A2M2 register to zero by updating minute value only
       writeRegister(ds3231RegAddrMap.alarm2HourInterval_reg,0x80);
       writeRegister(ds3231RegAddrMap.alarm2DayDateInterval_reg,0x80);
       writeRegister(ds3231RegAddrMap.alarm2MinuteInterval_reg,updatedMinutesBCDvalue);      //write to A2M2 regiter
 
 
       statusRegA2F=readRegister(ds3231RegAddrMap.status_reg);
       if(statusRegA2F & 0x02){
          cout<<"Alarm for minutes match has been configured"<<endl;
       }
 
       else cout<<"Alarm for minutes match has not been configured"<<endl;
       
 }
 



    //function for square wave frequency select

    void DS3231::squareWaveFreqSelect(){

      int freqinput=0;
      cout<<"Enter 1 for 1Hz square wave output frequency"<<endl;
      cout<<"Enter 2 for 1.024kHz square wave output frequency"<<endl;
      cout<<"Enter 3 for 4.096kHz square wave output frequency"<<endl;
      cout<<"Enter 4 for 8.192kHz square wave output frequency"<<endl;
      cin>>freqinput;
      writeRegister(ds3231RegAddrMap.control_reg,readRegister(ds3231RegAddrMap.control_reg) | (0x18));//default value of frequency that is 8.192kHz
      switch(freqinput)
      {
         case 1:   writeRegister(ds3231RegAddrMap.control_reg,readRegister(ds3231RegAddrMap.control_reg) & (0xE7)); break;
         case 2:   writeRegister(ds3231RegAddrMap.control_reg,readRegister(ds3231RegAddrMap.control_reg) & (0xEF)); break;
         case 3:   writeRegister(ds3231RegAddrMap.control_reg,readRegister(ds3231RegAddrMap.control_reg) & (0xF7)); break;
         case 4:   writeRegister(ds3231RegAddrMap.control_reg,readRegister(ds3231RegAddrMap.control_reg) | (0x18)); break;
         default : cout<<"Select the valid option";break;
      }


    }
 
 
 
    //function to sync ds3231 with NTP
 
    void DS3231::syncNtpDs3231(){
       cout << "Synchronizing system time with NTP..." <<endl;
       system("sudo ntpdate pool.ntp.org");
       cout << "Setting DS3231 time..." <<endl;
       time_t t=time(0);
       struct tm *toSync=localtime(&t);
 
       writeRegister(ds3231RegAddrMap.seconds,decimalToBCD(toSync->tm_sec));
       writeRegister(ds3231RegAddrMap.minutes,decimalToBCD(toSync->tm_min));
       writeRegister(ds3231RegAddrMap.hours,decimalToBCD(toSync->tm_hour));
       writeRegister(ds3231RegAddrMap.day,decimalToBCD(toSync->tm_wday));
       writeRegister(ds3231RegAddrMap.date,decimalToBCD(toSync->tm_mday));
       writeRegister(ds3231RegAddrMap.month,decimalToBCD(toSync->tm_mon+1));
       writeRegister(ds3231RegAddrMap.year,decimalToBCD(toSync->tm_year+1900-2000));
 
 
    }
 
 }
 