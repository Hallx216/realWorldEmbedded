/*
 * Application.cpp
 * Copyright (c) 2025 Derek Molloy (www.derekmolloy.ie)
 * Modified by: Student Name
 */

#include <iostream>
#include "DS3231.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace een1071;

int main() {
	DS3231 ds3231;
	int optionNumber;
while(1){
	cout<<"Enter 1 for read and display day,date and temperature"<<endl;
	cout<<"Enter 2 for set time and date"<<endl;
	cout<<"Enter 3 to set alarm once per second"<<endl;
	cout<<"Enter 4 to set alarm for minutes matching"<<endl;
	cout<<"Enter 5 to set square wave output frequency"<<endl;
	cout<<"Enter 6 to sync ds3231 time with NTP"<<endl;

    cin>>optionNumber;
	switch(optionNumber)
	{
		case 1: ds3231.readAndDisplayTimeDateTemp(); break;
		case 2: ds3231.setTimeDate(); break;
		case 3: ds3231.alarmOncePerSecond();break;
		case 4: ds3231.alarmMinutesMatch();break;
		case 5: ds3231.squareWaveFreqSelect();break;
		case 6: ds3231.syncNtpDs3231();break;
	}
}	
	return 0;
}
