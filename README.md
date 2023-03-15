# ht1622-esp-idf
ESP-IDF library for HT1622 segment LCD driver. Written for a special segment LCD.

This library is ported to ESP-IDF framework by using https://github.com/MartyMacGyver/LCD_HT1622_16SegLcd repository, which has an Arduino framemwork library for HT1622. Credits to MartyMacGyver...

This HT1622 LCD driver library should work with your ESP-IDF projects as a component.

#HOW TO USE:
1- Create your ESP-IDF project.
2- Create a component for your project, named display. (I guess the source and library file names should match with the component name, be careful about that)
3- Remake the CMakeLists.txt file. It need to be linked to other components or to the main file location in order to work with them.
4- Change the library according to your needs and ready to go.

If you have any issues, let me know.

Tested on ESP-IDF v4.4, however it should work with all versions, if the framework functions are same with the IDF v4.4.
