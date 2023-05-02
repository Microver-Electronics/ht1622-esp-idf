#ifndef RYP_DISPLAY_H_
#define RYP_DISPLAY_H_

#include <stdio.h>
#include <rom/ets_sys.h>
#include <driver/gpio.h>

#include "rypDef.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Helper functions and variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// HT162x commands (Start with 0b100, ends with one "don't care" byte)
#define  CMD_SYS_DIS  			0x00  // SYS DIS    (0000-0000-X) Turn off system oscillator, LCD bias gen [Default]
#define  CMD_SYS_EN   			0x01  // SYS EN     (0000-0001-X) Turn on  system oscillator
#define  CMD_LCD_OFF  			0x02  // LCD OFF    (0000-0010-X) Turn off LCD display [Default]
#define  CMD_LCD_ON   			0x03  // LCD ON     (0000-0011-X) Turn on  LCD display
#define  CMD_RC_INT   			0x18  // RC INT     (0001-10XX-X) System clock source, on-chip RC oscillator
#define  CMD_BIAS_COM 			0x29  // BIAS & COM (0010-10X1-X) 1/3 bias, 4 commons // HT1621 only
#define  CMD_IRQ_DISABLE 		0x80  // IRQ DIS    (100X-0XXX-X)
#define  CMD_WATCHDOG_DISABLE 	0x05  // WD DIS     (0000-0101-X)

#define  HT1622_ADDRS 0x40  // HT1622 has 64 possible 4-bit addresses

#define CS   38 // Active low
#define WR   36 // Active low
#define DATA  35
#define OE 40 // Screen power on pin ? Should be high when component is active.

#define BUZZER_PIN 4

#define LSB_FORMAT  true
#define MSB_FORMAT  false

#define SETUP_DELAY_USECS 1
#define HOLD_DELAY_USECS  1
#define WRITE_DELAY_USECS 2  // Tclk min. on data sheet - overhead is more than this at low clock speeds
#define RESET_DELAY_USECS 1000  // Not strictly necessary

void init_display(void);
void HT162x_SendBits(uint16_t data, uint8_t bits, bool LSB_FIRST);
void HT162x_Initialize(void);
void HT162x_ActivateTask(void);
void HT162x_Command(uint8_t cmd);
void HT162x_WriteData(uint8_t addr, uint16_t sdata, uint8_t bits);
void AllElements(uint8_t state);
void HT162x_Carry_Set(uint32_t carry_value);
void HT162x_Launch_Set(float launch_value);
void HT162x_Smash_Set(float smash_value);
void HT162x_BallSpeed_Set(uint32_t ball_velocity);
void HT162x_ClubSpeed_Set(uint32_t club_velocity);
void HT162x_CaptureMode_Set(uint32_t capture_mode);
void HT162x_Carry_Release();
void HT162x_Launch_Release();
void HT162x_Smash_Release();
void HT162x_BallSpeed_Release();
void HT162x_ClubSpeed_Release();
void HT162x_OpenUp_Test(void);
uint8_t convert_value_for_digits(uint32_t data);
void HT162x_Z_Set(uint8_t addr, uint8_t com, bool state);
void display_task(void *arg);
void HT162x_RestartUnitLabels(void);
void HT162x_RestartStaticLabels(void);

typedef enum HT1622_VelocityUnit{
	KPH,
	MPH
}HT1622_VelocityUnit_t;

typedef enum HT1622_DistanceUnit{
	METER,
	YARD
}HT1622_DistanceUnit_t;

typedef enum HT1622_StateMachine{
	HT1622_INIT,
	HT1622_IDLE,
	HT1622_REFRESH,
	HT1622_ERROR
}HT1622_StateMachine_t;

typedef enum HT1622_BatteryPercentage{
	PERCENT_100_85 = 5,
	PERCENT_85_70 = 4,
	PERCENT_70_55 = 3,
	PERCENT_55_40 = 2,
	PERCENT_40_25 = 1,
	PERCENT_25_00 = 0
}HT1622_BatteryPercentage_t;

typedef enum HT1622_DisplayMode{
	CLUB_AND_BALL_SPEED = (uint32_t)1,
	CLUB_SPEED = (uint32_t)2,
	BALL_SPEED = (uint32_t)3
}HT1622_DisplayMode_t;

typedef struct HT1622_DisplayConfig_s{
	HT1622_VelocityUnit_t VelocityUnit;
	HT1622_DistanceUnit_t DistanceUnit;
	HT1622_StateMachine_t State;
	HT1622_DisplayMode_t Mode;
	bool RefreshFlag;
}HT1622_DisplayConfig_t[1], *HT1622_DisplayConfig;

typedef struct HT1622_DisplayValues_s{
	uint16_t ClubSpeed;
	uint16_t BallSpeed;
	float Smash;
	float Launch;
	uint16_t Carry;
	HT1622_BatteryPercentage_t BatteryPercentage;
}HT1622_DisplayValues_t[1], *HT1622_DisplayValues;

extern HT1622_DisplayConfig HT1622_Config_s;
extern HT1622_DisplayValues HT1622_Values_s;

#endif /* RYP_DISPLAY_H_ */
