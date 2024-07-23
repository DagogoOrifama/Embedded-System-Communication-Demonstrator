/*
 * Embedded System Communication Demonstrator (SPI Slave Node)
 * ----------------------------
 * By:   Dagogo Orifama
 * SID: 201177661
 * Date: 24/08/2018
 *
 * Description
 * This file contains program for a SPI Slave Node
 */
#include "mbed.h"

SPISlave spi_slave(PA_7, PA_6, PA_5,PA_15); // Initialise the SPI pins (mosi, miso, sclk, ssel)
char switch_data ; // stores data to be transmitted using the rotary dip switch
char received_value; //value return from SPI Master

//Rotary Dip Switch pin configuration
DigitalIn Psw1(PB_8); //pin 1
DigitalIn Psw2(PB_9); // pin 2
DigitalIn Psw3(PC_5); // pin 4
DigitalIn Psw4(PC_4);  // pin 8

DigitalOut driver_config[4]={PC_3, PC_2, PC_1, PC_0}; //PINS:D,C,B,A (PIN NAMES: Dp, g, f, e, d, c, b, a)

/* An array to turn ON/OFF the pins of the CMOS BCD-to-7-Segment Latch 
  Decoder Driver, its in order of the DigitalOut Pins for the 7-segment driver (D,C,B,A) 
*/
int number[11][8]={
                    {0,0,0,0},          //zero
                    {0,0,0,1},          //one
                    {0,0,1,0},          //two
                    {0,0,1,1},          //three
                    {0,1,0,0},          //four
                    {0,1,0,1},          //five
                    {0,1,1,0},          //six
                    {0,1,1,1},          //seven
                    {1,0,0,0},          //eight
                    {1,0,0,1},          //nine
                    {1,0,1,0}           // Wrong output to blank segment
                    };
 
 
 
// Function to scan the pins of the Rotary switch
void rotarySwitchScanner(){
    
     if(Psw1==0){
        switch_data= 1;  //switch_data|0x01;
    } 
 
        if(Psw2==0){
        switch_data= 2;  //switch_data|0x01;
    } 
        if(Psw1==0 & Psw2==0){
        switch_data= 3;  //switch_data|0x01;
    } 
        if(Psw3==0){
        switch_data= 4;  //switch_data|0x01;
    } 
        if(Psw1==0 & Psw3==0){
        switch_data= 5;
    }
        if(Psw2==0 & Psw3==0){
        switch_data= 6;
    }
        if(Psw1==0 & Psw2==0 & Psw3==0){
        switch_data= 7;
    }
        if(Psw4==0){
        switch_data= 8;
    } 
        if(Psw1==0 & Psw4==0){
        switch_data= 9;
    } 
        if(Psw1==1 & Psw2==1 & Psw3==1 & Psw4==1){
        switch_data= 0;
    }
        if(Psw2==0 & Psw4==0){
        switch_data= 0;
    } 
        if(Psw1==0 & Psw2==0 & Psw4==0){
        switch_data= 0;
    }
        if(Psw3==0 & Psw4==0){
        switch_data= 0;
    } 
        if(Psw1==0 & Psw3==0 & Psw4==0){
        switch_data= 0;
    }
        if(Psw2==0 & Psw3==0 & Psw4==0){
        switch_data= 0;
    }
        if(Psw1==0 & Psw2==0 & Psw3==0 & Psw4==0){
        switch_data= 0;
    }
}
 
int main() {
        // configure the pin mode of the rotary switch 
        Psw1.mode(PullUp);
        Psw2.mode(PullUp);
        Psw3.mode(PullUp);
        Psw4.mode(PullUp); 
        spi_slave.frequency(1000000);
while(1) {
    switch_data=0;
    rotarySwitchScanner();  // Retrive data specified by the rotary switch
    if(spi_slave.receive()) { //check for the occurance of data transfer
    received_value = spi_slave.read(); // Read data sent from the master node
    spi_slave.reply(switch_data); // Reply with a switch data
}
    //check the received data and send a  corresponding value to the 7-segment driver
     if (received_value==1){
     for (int i=0; i<4; i++){driver_config[i] = number[1][i];}//0x06;
     }
     if (received_value==2){
     for (int i=0; i<4; i++){driver_config[i] = number[2][i];}//0x06;
     }
     if (received_value==3){
     for (int i=0; i<4; i++){driver_config[i] = number[3][i];}//0x06;
     }
     if (received_value==4){
     for (int i=0; i<4; i++){driver_config[i] = number[4][i];}//0x06;
     }
     if (received_value==5){
     for (int i=0; i<4; i++){driver_config[i] = number[5][i];}//0x06;
     }
     if (received_value==6){
     for (int i=0; i<4; i++){driver_config[i] = number[6][i];}//0x06;
     }
     if (received_value==7){
     for (int i=0; i<4; i++){driver_config[i] = number[7][i];}//0x06;
     }
     if (received_value==8){
     for (int i=0; i<4; i++){driver_config[i] = number[8][i];}//0x06;
     }
     if (received_value==9){
     for (int i=0; i<4; i++){driver_config[i] = number[9][i];}//0x06;
     }
     if (received_value==0){
     for (int i=0; i<4; i++){driver_config[i] = number[10][i];}//0x06;
     }
}
} 
