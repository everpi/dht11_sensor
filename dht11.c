/*  
 *  DHT11 Sensor for Raspberry Pi
 *
 *  Copyright (C) 2014 EverPi - everpi[at]tsar[dot]in
 *  blog.everpi.net 
 * 
 *  This file is part of dht11_sensor.
 *
 *  dht11_sensor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  dds_ad9850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with dht11_sensor.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <wiringPi.h>

#define HELP \
        "\n  \033[1mDHT11 by blog.everpi.net\n\n" \
        "\tUsage:  [wiringpi_pin]\033[0m\n\n"

unsigned int dht11_read(int pin){

    int bits = 42;
    unsigned char data = 0;
    unsigned int dht11_data = 0;
    unsigned int parity = 0;
    int waithigh = 0;
    struct timespec ntime = {.tv_sec = 0,
			     .tv_nsec = 1000L };   

    unsigned int waitvalues[42] = {0};
 
        pinMode(pin,OUTPUT);
        digitalWrite(pin,LOW);
        delay(11);
        digitalWrite(pin,HIGH);
        pinMode(pin,INPUT);    
	

    while(bits){
       
	while(1){
		data = digitalRead(pin);
		
		if(data == 1) break;
		
	}
		
	waithigh = 0;	
	
	while(digitalRead(pin) == 1){
		
		delayMicroseconds(1);
		waithigh++;
		if(waithigh > 5000) break;
	}
	
	waitvalues[bits] = waithigh;
	
	if(waithigh > 20){
		data = 1;
	}else data = 0;
	
	if(bits <= 8) parity = (parity<<1) | data;
	else
	dht11_data = ( (dht11_data<<1) | data );
	
	bits--;	
    }
	
	if(parity == ( ((dht11_data&0xff00)>>8) + 
		        (dht11_data>>24) )	){
	
	return dht11_data;
	}else{
		return 0;
	}
 
}

int main(int argc, char *argv[]){
	
	unsigned int data = 0;

	if(argc < 2){

        	printf("%s",HELP);
        	return 0;
        }

	if(wiringPiSetup()==-1) exit(1);

	while(!data){

		data = dht11_read(strtol(argv[1],NULL,10));	
	}

	fprintf(stderr,"H:%u - TEMP:%u\n",data>>24, (data&0xff00)>>8);
	
	return 0;
}
