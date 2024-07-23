/*
 * Embedded System Communication Demonstrator (Master Node)
 * ----------------------------
 * By:   Dagogo Orifama
 * SID: 201177661
 * Date: 24/08/2018
 *
 * Description
 * This file contains the Master Node program, for SPI, I2C, ZigBee and Bluetooth Communication
 */
#include "mbed.h"

//Initialising pins for the various protocols
SPI spi_master(PA_7, PA_6, PA_5); // Initialise the SPI pins (mosi, miso, sclk)
I2C i2c_master(PB_9, PB_8); // Configure the I2C ports (SDA,SCL)
Serial xbee_coordinator(PC_4,PC_5);; // configuring a serial port for the XBee ZigBee module
Serial rn42Bluetooth(PC_1,PC_0);  // configuring a serial port for the RN-42 Bluetooth module
DigitalOut slave_select(D9); //Slave select pin, this is used to select the SPI slave
DigitalOut xbee_reset(D10); // reset pin for the XBee module
DigitalOut RN42_reset_pin(D10); // reset pin for the Bluetooth module

// Configuring the selector switches
DigitalIn selector_switch1(D4); //Selector Switch 1 
DigitalIn selector_switch2(D5); //Selector Switch 2
DigitalIn selector_switch3(D6); //Selector Switch 3
DigitalIn selector_switch4(D7); //Selector Switch 4
 
//Rotary Dip Switch pin configuration
DigitalIn Psw1(PD_5); // pin 1
DigitalIn Psw2(PD_4); // pin 2
DigitalIn Psw3(PC_2); // pin 4
DigitalIn Psw4(PC_3); // pin 8

//pin configuration for the CD4511B CMOS BCD-to-7-Segment Latch Decoder Driver
DigitalOut driver_config[4]={D2, D1, D0, D3}; //PINS (D,C,B,A) 

char switch_data;  // stores data to be transmitted using the rotary dip switch
char switch_data_send;
const int slave_address = 0x64; // specifies the I2C slave address of the slave to be communicated 

char received_value; //value return from SPI slave
char received_value2[2]; //value return from I2C slave
char received_value3; //value return from XBee router
char received_value4; //value return from Bluetooth Slave

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
        if (received_value==1){ // if the received_value = 1
        // Configure the 7-segment driver to display 1 on the 7-segment display 
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
// Function to reset the xbee module 
void xbeeActivator(void){
    xbee_reset = 0;   //Set reset pin to 0
    wait_ms(1);
    xbee_reset = 1;   //Set reset pin to 1
    wait_ms(1); 
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
 
    //zigbee module reset
    xbeeActivator(); 
 if(selector_switch4){     
    // Bluetooth module reset
    bluetoothActivator();
    rn42Bluetooth.baud(115200); // set the baud rate of the module
    wait(1.0);
    BTConnectionInit();
}
    spi_master.frequency(1000000); // set the frequency of the SPI bus clock (1MHz)
    
while(1){

       
       //SPI communication
       if (selector_switch1==1){
       //Set up the word to be sent, by testing switch inputs
       switch_data=0; 
       rotarySwitchScanner();   // Retrive data specified by the rotary switch
     
       slave_select = 0; //select Slave
       received_value=spi_master.write(switch_data); //send switch_data and store the received value
       slave_select = 1;   // deselect slave
       SevenSegmentDriver(received_value);  // send the received data to the 7-segment display via its driver
    }
    //I2C communication
    else if ((selector_switch2)&&(!selector_switch1)){ // check if I2C selector switch has been enabled
       switch_data = 0;
       rotarySwitchScanner();  // Retrive data specified by the rotary switch
       if(switch_data_send!=switch_data) {
         switch_data_send = switch_data;
         i2c_master.write(slave_address,(char*)&switch_data_send,1,true); // write data to the I2C slave
       } else {
        i2c_master.read(slave_address,(char*)&received_value2,1,true); // Read data from I2C slave
       }
        SevenSegmentDriver(received_value2[0]); // send the received data to the 7-segment display via its driver
    }
    //ZigBee Communication
    if((selector_switch3)&&(!selector_switch1)){ // check if XBee selector switch has been enabled
       switch_data = 0x00; // Initialise the switch data to be transmitted to zero
       rotarySwitchScanner();   // Retrive data specified by the rotary switch
       xbee_coordinator.putc(switch_data); //XBee write
       if(xbee_coordinator.readable()) received_value3 = xbee_coordinator.getc();
       //set 7-segment driver according to incoming data from the router
       SevenSegmentDriver(received_value3); // send the received data to the 7-segment display via its driver
    }
    // Bluetooth Communication
    if((selector_switch4)&&(!selector_switch1)){    // check if Bluetooth selector switch has been enabled
       switch_data = 0;
       rotarySwitchScanner(); // Retrive data specified by the rotary switch
       rn42Bluetooth.putc(switch_data); // send char data on serial
       if(rn42Bluetooth.readable()) { // if data available
       received_value4=rn42Bluetooth.getc(); // get data
       SevenSegmentDriver(received_value4); // send the received data to the 7-segment display via its driver
       }
     }

} //end of the while loop
} // end of the main function

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
    //Enter the master mode and set the master to auto-connect
    rn42Bluetooth.putc(0x53); 
    rn42Bluetooth.putc(0x4D);
    rn42Bluetooth.putc(0x2C);
    rn42Bluetooth.putc(0x31);
    rn42Bluetooth.putc(0x0D);
    wait(0.1);
    // configure module to attempt connection and provide the MAC address of the 
    // slave to be connected
    rn42Bluetooth.putc(0x43); 
    rn42Bluetooth.putc(0x2C); 
    rn42Bluetooth.putc(0x30); 
    rn42Bluetooth.putc(0x30); 
    rn42Bluetooth.putc(0x30); 
    rn42Bluetooth.putc(0x36); 
    rn42Bluetooth.putc(0x36); 
    rn42Bluetooth.putc(0x36); 
    rn42Bluetooth.putc(0x37); 
    rn42Bluetooth.putc(0x35); 
    rn42Bluetooth.putc(0x39); 
    rn42Bluetooth.putc(0x37); 
    rn42Bluetooth.putc(0x46); 
    rn42Bluetooth.putc(0x46); 
    rn42Bluetooth.putc(0x0D);
    //Set the timeout to 255 to prevent break in Data transmission
    rn42Bluetooth.putc(0x53);
    rn42Bluetooth.putc(0x54);
    rn42Bluetooth.putc(0x2C);
    rn42Bluetooth.putc(0x32);
    rn42Bluetooth.putc(0x35);
    rn42Bluetooth.putc(0x35);
    rn42Bluetooth.putc(0x0D);
    wait(1.0);
    // Exit the module command mode
    rn42Bluetooth.putc(0x2D); 
    rn42Bluetooth.putc(0x2D);
    rn42Bluetooth.putc(0x2D);
    rn42Bluetooth.putc(0x0D);
    wait(0.5);
}