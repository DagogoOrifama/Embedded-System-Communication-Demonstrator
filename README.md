# Design and Implementation of an Embedded System Communication Demonstrator

## Overview
This project presents the design and implementation of a communication demonstrator for embedded systems. The objective is to address software processing bandwidth issues by creating subsystems that transfer results to a central processor via various embedded communication protocols. The demonstrator features both wired (I2C, SPI) and wireless (Bluetooth, ZigBee) communication protocols.

## Aim and Objectives
The aim is to establish bidirectional communication between a master microcontroller and four slave microcontrollers using different communication protocols. The specific objectives include:
- Implementing I2C, SPI, ZigBee, and Bluetooth protocols for communication.
- Designing a system where one master microcontroller communicates with four slaves.
- Using selector switches to determine the active communication protocol.
- Transferring control information via a rotary switch and displaying it on a 7-segment display.

## Components
- **Master Node:** Controls the selection and communication with slave nodes.
- **Slave Nodes:** Receive and send data to the master node via the specified communication protocol.
- **DISCO-L475VG-IOT01A Microcontroller:** The main hardware used for both master and slave nodes.
- **XBee-PRO ZigBee and RN-42 Bluetooth Modules:** Used for wireless communication.
- **Selector Switches and Rotary DIP Switch:** Used for selecting communication protocols and sending control data.
- **BCD-to-7-Segment Driver and 7-Segment Display:** For displaying the received data.

## Implementation
1. **Hardware Design:**
   - Establish connections between microcontrollers and modules for SPI, I2C, Bluetooth, and ZigBee communications.
   - Interface rotary switches and 7-segment displays with each node.
   - Implement selector switches to control communication protocol selection.

2. **Software Design:**
   - Develop firmware for master and slave nodes in Embedded C.
   - Utilize the Mbed compiler for compiling and generating binary files for the microcontrollers.
   - Implement communication protocols with appropriate initializations and data transfer methods.

## Results
The project successfully demonstrated bidirectional communication between a master and slave microcontroller using SPI, I2C, ZigBee, and Bluetooth protocols. Each communication setup was validated by transferring control data via a rotary switch and displaying the data on a 7-segment display.

## Conclusion
The project achieved its objectives by designing and implementing a versatile embedded system communication demonstrator. This demonstrator can transfer data between nodes using multiple communication protocols, showcasing the effectiveness of embedded system communications in managing subsystems.

## Recommendations
For future improvements:
- Allow simultaneous communication with multiple slave nodes.
- Enable Bluetooth communication initialization without resetting the master node.

## Acknowledgements
Thanks to the supervisor, assessors, friends, and family for their support and contributions to the success of this project. Special thanks to Shell Petroleum Development Company (SPDC) Nigeria for financial support.
