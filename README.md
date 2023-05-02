# ht1622-esp-idf
ESP-IDF library for HT1622 segment LCD driver. Written for a special segment LCD.


### Credits to MartyMacGyver...
This library is ported to ESP-IDF framework by using https://github.com/MartyMacGyver/LCD_HT1622_16SegLcd repository, which has an Arduino framemwork library for HT1622. 

This HT1622 LCD driver library should work with your ESP-IDF projects as a component.



## HOW TO USE:
1. Create your ESP-IDF project.
2. Create a component for your project, named display. (I guess the source and library file names should match with the component name, be careful about that)
3. Copy and paste the contents of display folder. 
4. Check the CMakeLists.txt file. It needs to be linked to other components or to the main file location in order to work with them.
5. Change the library according to your needs and you are ready to go. The parts you need to change are shown below.

### Definitions in display.h need to be changed according to the custom lcd that you use:
```C
#define SEGMENT_SIZE 27
#define HT1622_ADDRESS_SIZE 38

#define Z1_LOCATION 1, 1
#define Z2_LOCATION 6, 1
#define Z3_LOCATION 7, 0
#define Z4_LOCATION 6, 3
#define Z5_LOCATION 6, 2
#define Z6_LOCATION 7, 1
#define Z7_LOCATION 7, 2
#define Z8_LOCATION 11, 3
#define Z9_LOCATION 13, 3
#define Z10_LOCATION 1, 0
#define Z11_LOCATION 0, 3
#define Z12_LOCATION 17, 3
#define Z13_LOCATION 19, 3
#define Z14_LOCATION 0, 2
#define Z15_LOCATION 25, 3
#define Z16_LOCATION 0, 1
#define Z17_LOCATION 29, 3
#define Z18_LOCATION 0, 0
#define Z19_LOCATION 33, 3
#define Z20_LOCATION 35, 3
#define Z21_LOCATION 27, 3

#define DIGIT_0 0b01110111
#define DIGIT_1 0b00010010
#define DIGIT_2 0b01101011
#define DIGIT_3 0b01011011
#define DIGIT_4 0b00011110
#define DIGIT_5 0b01011101
#define DIGIT_6 0b01111101
#define DIGIT_7 0b00010011
#define DIGIT_8 0b01111111
#define DIGIT_9 0b01011111


#define DIGIT_1_ADDRESS 2
#define DIGIT_2_ADDRESS 4
#define DIGIT_3_ADDRESS 12
#define DIGIT_4_ADDRESS 10
#define DIGIT_5_ADDRESS 8
#define DIGIT_6_ADDRESS 18
#define DIGIT_7_ADDRESS 16
#define DIGIT_8_ADDRESS 14
#define DIGIT_9_ADDRESS 24
#define DIGIT_10_ADDRESS 22
#define DIGIT_11_ADDRESS 20
#define DIGIT_12_ADDRESS 30
#define DIGIT_13_ADDRESS 28
#define DIGIT_14_ADDRESS 26
#define DIGIT_15_ADDRESS 36
#define DIGIT_16_ADDRESS 34
#define DIGIT_17_ADDRESS 32

#define  HT1622_ADDRS 0x40  // HT1622 has 64 possible 4-bit addresses

#define CS   38 // Active low
#define WR   36 // Active low
#define DATA  35
#define OE 40 // Screen power on pin ? Should be high when component is active.

#define LSB_FORMAT  true
#define MSB_FORMAT  false
```

### These functions should stay same. They are the core functions of this library.:
```C
void HT162x_SendBits(uint16_t data, uint8_t bits, bool LSB_FIRST);
void HT162x_Command(uint8_t cmd);
void HT162x_WriteData(uint8_t addr, uint16_t sdata, uint8_t bits);
void AllElements(uint8_t state);
void HT162x_Z_Set(uint8_t addr, uint8_t com, bool state);
```

Feel free to change any other function in the library according to your needs.

## IMPORTANT:
If you want to understand the code much better, you can check the driver datasheet and our custom lcd layout.
PS: We are using a buzzer inside the library. Just delete the buzzer codes if you don't want to use it like this.



If you have any issues, let me know https://github.com/dogualpay.


Tested on ESP-IDF v4.4, however it should work with all versions, if the framework functions are same with the IDF v4.4.
