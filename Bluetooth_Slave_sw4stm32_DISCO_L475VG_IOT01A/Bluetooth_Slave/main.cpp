/*
 * Embedded System Communication Demonstrator (Bluetooth Slave Node)
 * ----------------------------
 * By:   Dagogo Orifama
 * SID:  201177661
 * Date: 24/08/2018
 *
 * Description:
 *  This file contains program for a Bluetooth paired slave Node
 */
#include "mbed.h"

Serial rn42Bluetooth(PC_1,PC_0); //name the serial port rn41ZZ
DigitalOut RN42_reset_pin(PA_1); // reset pin for the Bluetooth slave module
char switch_data ; // stores data to be transmitted using the rotary dip switch
char received_value; //value received from Master


//Rotary Dip Switch pin configuration
DigitalIn Psw1(PA_7); //pin 1
DigitalIn Psw2(PB_2); // pin 2
DigitalIn Psw3(PA_2); // pin 4
DigitalIn Psw4(PA_15); // pin 8

DigitalOut driver_config[4]={PB_1, PB_4, PA_3, PA_4}; //PINS:D,C,B,A 

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
 
/* Function to scan the pins of the Rotary DIP switch
 The common pin of the rotary switch is connected to ground
 Hence test to see if a pin is LOW, Hence the BCD Decimal 
 Complementary code specified in its datasheet was implemented */
void rotarySwitchScanner(){
    
     if(Psw1==0){
        switch_data= 1; 
    } 
 
        if(Psw2==0){
        switch_data= 2;  
    } 
        if(Psw1==0 & Psw2==0){
        switch_data= 3;  
    } 
        if(Psw3==0){
        switch_data= 4;  
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

// Function to check the received data and send a  corresponding value to the 7-segment driver
void SevenSegmentDriver(char received_value){
        if (received_value==1){
            for (int i=0; i<4; i++){driver_config[i] = number[1][i];}
        }
        if (received_value==2){
            for (int i=0; i<4; i++){driver_config[i] = number[2][i];}
        }
        if (received_value==3){
            for (int i=0; i<4; i++){driver_config[i] = number[3][i];}
        }
        if (received_value==4){
            for (int i=0; i<4; i++){driver_config[i] = number[4][i];}
        }
        if (received_value==5){
            for (int i=0; i<4; i++){driver_config[i] = number[5][i];}
        }
        if (received_value==6){
            for (int i=0; i<4; i++){driver_config[i] = number[6][i];}
        }
        if (received_value==7){
            for (int i=0; i<4; i++){driver_config[i] = number[7][i];}
        }
        if (received_value==8){
            for (int i=0; i<4; i++){driver_config[i] = number[8][i];}
        }
        if (received_value==9){
            for (int i=0; i<4; i++){driver_config[i] = number[9][i];}
        }
        if (received_value==0){
            for (int i=0; i<4; i++){driver_config[i] = number[10][i];}
        }
}

// Function to reset the Bluetooth module 
void bluetoothActivator(void){
    RN42_reset_pin = 0;
    wait_ms(500);
    RN42_reset_pin = 1;
    }
    
void BTConnectionInit(void);
int main() {
    // configure the pin mode of the rotary switch 
    Psw1.mode(PullUp);
    Psw2.mode(PullUp);
    Psw3.mode(PullUp);
    Psw4.mode(PullUp);    
    bluetoothActivator(); // Bluetooth module reset
    rn42Bluetooth.baud(115200); // set the baud rate of the module
    wait(1.0); 
    BTConnectionInit();

while (1) {
      switch_data = 0;
      rotarySwitchScanner();
      if(rn42Bluetooth.readable()) { // if data has been sent by the master
         received_value=rn42Bluetooth.getc(); // get and store the data
      }
      rn42Bluetooth.putc(switch_data); // send char data on serial  
    //check the received data and send a  corresponding value to the 7-segment driver
     SevenSegmentDriver(received_value);
}
}

/* This function Initialising the Bluetooth module, it configures the bluetooth module to operate
   in master mode and also provides a MAC address of a slave that will be connected with the master
   All values and commands used in this function are the hexadecimal value equivalent of thecorresponding
   ASCII characters
*/
void BTConnectionInit() {
    // Enter command mode ($$$)
    rn42Bluetooth.putc(0x24); 
    rn42Bluetooth.putc(0x24);
    rn42Bluetooth.putc(0x24);
    wait(2);
    // configure module authentication to allow remote connection attempt
    rn42Bluetooth.putc(0x53);
    rn42Bluetooth.putc(0x41);
    rn42Bluetooth.putc(0x2C);
    rn42Bluetooth.putc(0x30);
    rn42Bluetooth.putc(0x0D);
    //Set the timeout to 255 to prevent break in Data transmission
    rn42Bluetooth.putc(0x53);
    rn42Bluetooth.putc(0x54);
    rn42Bluetooth.putc(0x2C);
    rn42Bluetooth.putc(0x32);
    rn42Bluetooth.putc(0x35);
    rn42Bluetooth.putc(0x35);
    rn42Bluetooth.putc(0x0D);
    // Exit the module command mode
    rn42Bluetooth.putc(0x2D);
    rn42Bluetooth.putc(0x2D);
    rn42Bluetooth.putc(0x2D);
    rn42Bluetooth.putc(0x0D);
    wait(0.5);    
}