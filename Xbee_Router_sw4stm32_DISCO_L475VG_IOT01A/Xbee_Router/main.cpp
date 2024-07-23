/*
 * Embedded System Communication Demonstrator (ZigBee Router Node)
 * ----------------------------
 * By:   Dagogo Orifama
 * SID:  201177661
 * Date: 24/08/2018
 *
 * Description
 * This file contains program for a ZigBee Router Node
 */
#include "mbed.h"

Serial xbee_router(PC_1,PC_0);
DigitalOut xbee_reset(PA_7); // reset pin for the XBee router module
char switch_data ;// stores data to be transmitted using the rotary dip switch
char received_value; //value return from XBee coordinator

//Rotary Dip Switch pin configuration
DigitalIn Psw1(PA_0); //pin 1
DigitalIn Psw2(PA_1); // pin 2
DigitalIn Psw3(PD_14); // pin 4
DigitalIn Psw4(PB_0); // pin 8

DigitalOut driver_config[4]={PA_3, PB_4, PB_1, PA_4}; //PINS:D,C,B,A 

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
// Function to reset the xbee module 
void xbeeActivator(void){
    xbee_reset = 0;   //Set reset pin to 0
    wait_ms(1);
    xbee_reset = 1;   //Set reset pin to 1
    wait_ms(1); 
    }

int main() {
        // configure the pin mode of the rotary switch 
        Psw1.mode(PullUp);
        Psw2.mode(PullUp);
        Psw3.mode(PullUp);
        Psw4.mode(PullUp); 
        //char receive =0x00;
        xbeeActivator();
    
while(1) {
        switch_data = 0x00;
        rotarySwitchScanner();   // Retrive data specified by the rotary switch
        if(xbee_router.readable()) received_value = xbee_router.getc(); //XBee Read
        xbee_router.putc(switch_data); //XBee write
        
//check the received data and send a  corresponding value to the 7-segment driver
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
}