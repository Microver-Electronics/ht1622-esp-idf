#include "display.h"

uint8_t HT1622_Memory[HT1622_ADDRESS_SIZE] = {0x0};
TaskHandle_t displayTaskHandle = NULL;
HT1622_DisplayConfig HT1622_Config_s = NULL;
HT1622_DisplayValues HT1622_Values_s = NULL;

// To set or clear Z coded segments on custom LCD
void HT162x_Z_Set(uint8_t addr, uint8_t com, bool state){ 
	uint8_t data;

	// Bit-wise edit of locally allocated screen memory
	if(state){
		HT1622_Memory[addr] |= 1 << com;
	}
	else{
		HT1622_Memory[addr] &= ~(1 << com);
	}

	data = HT1622_Memory[addr] & 0xf;

	HT162x_WriteData(addr, (uint16_t)data, 4);
}

// To convert the input data to 7 segment value for each digit.
uint8_t convert_value_for_digits(uint32_t data){
	switch(data){
		case 0:
			return DIGIT_0;
			break;

		case 1:
			return DIGIT_1;
			break;

		case 2:
			return DIGIT_2;
			break;

		case 3:
			return DIGIT_3;
			break;

		case 4:
			return DIGIT_4;
			break;

		case 5:
			return DIGIT_5;
			break;

		case 6:
			return DIGIT_6;
			break;

		case 7:
			return DIGIT_7;
			break;

		case 8:
			return DIGIT_8;
			break;

		case 9:
			return DIGIT_9;
			break;

		default:
			return 0b00000000;
	}
}

// To clear club speed on screen.
void HT162x_ClubSpeed_Release(){
	// Digit IDs = 3, 4, 5
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_5_ADDRESS, DIGIT_4_ADDRESS, DIGIT_3_ADDRESS};

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

// To clear ball speed on screen.
void HT162x_BallSpeed_Release(){
	// Digit IDs = 6, 7, 8
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_8_ADDRESS, DIGIT_7_ADDRESS, DIGIT_6_ADDRESS};

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

// To clear smash value on screen.
void HT162x_Smash_Release(){
	// Digit IDs = 9, 10, 11
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_11_ADDRESS, DIGIT_10_ADDRESS, DIGIT_9_ADDRESS};

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

// To clear launch angle on screen.
void HT162x_Launch_Release(){
	// Digit IDs = 12, 13, 14
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_14_ADDRESS, DIGIT_13_ADDRESS, DIGIT_12_ADDRESS};

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

// To clear carry value on screen.
void HT162x_Carry_Release(){
	// Digit IDs = 15, 16, 17
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_17_ADDRESS, DIGIT_16_ADDRESS, DIGIT_15_ADDRESS};

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

void HT162x_CaptureMode_Set(uint32_t capture_mode){
	// Digit IDs = 1, 2
	uint8_t addresses[2] = {DIGIT_2_ADDRESS, DIGIT_1_ADDRESS};
	uint8_t digits[2] = {0x0};

	digits[0] = convert_value_for_digits(capture_mode%10);
	digits[1] = convert_value_for_digits((capture_mode/10)%10);

	for(int i=0; i<2; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

void HT162x_ClubSpeed_Set(uint32_t club_velocity){
	// Digit IDs = 3, 4, 5
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_5_ADDRESS, DIGIT_4_ADDRESS, DIGIT_3_ADDRESS};

	digits[0] = convert_value_for_digits(club_velocity%10);
	digits[1] = convert_value_for_digits((club_velocity/10)%10);
	digits[2] = convert_value_for_digits((club_velocity/100)%10);

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

void HT162x_BallSpeed_Set(uint32_t ball_velocity){
	// Digit IDs = 6, 7, 8
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_8_ADDRESS, DIGIT_7_ADDRESS, DIGIT_6_ADDRESS};

	digits[0] = convert_value_for_digits(ball_velocity%10);
	digits[1] = convert_value_for_digits((ball_velocity/10)%10);
	digits[2] = convert_value_for_digits((ball_velocity/100)%10);

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

void HT162x_Smash_Set(float smash_value){
	// Digit IDs = 9, 10, 11
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_11_ADDRESS, DIGIT_10_ADDRESS, DIGIT_9_ADDRESS};
	uint32_t decimal_value = (uint32_t)((float)10 * smash_value);

	digits[0] = convert_value_for_digits(decimal_value%10);
	digits[1] = convert_value_for_digits((decimal_value/10)%10);
	digits[2] = convert_value_for_digits((decimal_value/100)%10);

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

void HT162x_Launch_Set(float launch_value){
	// Digit IDs = 12, 13, 14
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_14_ADDRESS, DIGIT_13_ADDRESS, DIGIT_12_ADDRESS};
	uint32_t decimal_value = (uint32_t)((float)10 * launch_value);

	digits[0] = convert_value_for_digits(decimal_value%10);
	digits[1] = convert_value_for_digits((decimal_value/10)%10);
	digits[2] = convert_value_for_digits((decimal_value/100)%10);

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}

void HT162x_Carry_Set(uint32_t carry_value){
	// Digit IDs = 15, 16, 17
	uint8_t digits[3] = {0x0};
	uint8_t addresses[3] = {DIGIT_17_ADDRESS, DIGIT_16_ADDRESS, DIGIT_15_ADDRESS};

	digits[0] = convert_value_for_digits(carry_value%10);
	digits[1] = convert_value_for_digits((carry_value/10)%10);
	digits[2] = convert_value_for_digits((carry_value/100)%10);

	for(int i=0; i<3; i++){
		HT1622_Memory[addresses[i]] = digits[i] & 0b00001111;
		HT162x_WriteData(addresses[i], (uint16_t)HT1622_Memory[addresses[i]], 4);
		HT1622_Memory[addresses[i]+1] = (HT1622_Memory[addresses[i]+1] & (0b00001000)) | ((digits[i] & 0b01110000) >> 4);
		HT162x_WriteData(addresses[i]+1, (uint16_t)HT1622_Memory[addresses[i]+1], 4);
	}
}



// To send bits to the driver.
void HT162x_SendBits(uint16_t data, uint8_t bits, bool LSB_FIRST)
{
	// Data is shifted out bitwise, either LSB-first or MSB-first.
	// The mask is used to isolate the bit being transmitted.
	uint16_t mask = LSB_FIRST ? (1) : (1 << (bits-1));

	for (uint8_t i = bits; i > 0; i--)
	{
		ets_delay_us(WRITE_DELAY_USECS);
		gpio_set_level(WR, 0);
		data & mask ? gpio_set_level(DATA, 1) : gpio_set_level(DATA, 0);
		ets_delay_us(WRITE_DELAY_USECS);
		gpio_set_level(WR, 1);
		ets_delay_us(HOLD_DELAY_USECS);
		LSB_FIRST ? (data >>= 1) : (data <<= 1);
	}
}


// To send the command for the driver. See datasheet...
void HT162x_Command(uint8_t cmd)
{
	ets_delay_us(SETUP_DELAY_USECS);
	gpio_set_level(CS, 0);
	ets_delay_us(SETUP_DELAY_USECS);
	HT162x_SendBits(0b100, 3, MSB_FORMAT);
	HT162x_SendBits(cmd, 8, MSB_FORMAT);
	HT162x_SendBits(1, 1, MSB_FORMAT);
	ets_delay_us(SETUP_DELAY_USECS);
	gpio_set_level(CS, 1);
}

// To write data to memory space of LCD driver.
void HT162x_WriteData(uint8_t addr, uint16_t sdata, uint8_t bits)
{
	// Note: bits needs to be a multiple of 4 as data is in nibbles AND its default value is 4
	ets_delay_us(SETUP_DELAY_USECS);
	gpio_set_level(CS, 0);
	ets_delay_us(SETUP_DELAY_USECS);
	HT162x_SendBits(0b101, 3, MSB_FORMAT);
	HT162x_SendBits(addr, 6, MSB_FORMAT);
	for (int n = (bits/4)-1; n >= 0; n--) {
		uint16_t nib = (sdata & (0xf) << 4*n) >> (4*n);
		HT162x_SendBits(nib, 4, LSB_FORMAT);
	}
	ets_delay_us(SETUP_DELAY_USECS);
	gpio_set_level(CS, 1);
}

// To initialize our custom LCD and HT1622. You can change it according to your needs.
void HT162x_Initialize(void){
	// Set up I/O and display
	gpio_pad_select_gpio(OE);
	gpio_pad_select_gpio(CS);
	gpio_pad_select_gpio(WR);
	gpio_pad_select_gpio(DATA);
	gpio_pad_select_gpio(BUZZER_PIN);
	gpio_set_direction(OE, GPIO_MODE_OUTPUT); // set the pin
	gpio_set_direction(CS, GPIO_MODE_OUTPUT);
	gpio_set_direction(WR, GPIO_MODE_OUTPUT);
	gpio_set_direction(DATA, GPIO_MODE_OUTPUT);
	gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
	ets_delay_us(10);
	gpio_set_level(BUZZER_PIN, 0);
	ets_delay_us(10);
	gpio_set_level(OE, 1);
	ets_delay_us(10);
	HT162x_Command(CMD_WATCHDOG_DISABLE);
	ets_delay_us(10);
	HT162x_Command(CMD_IRQ_DISABLE);
	ets_delay_us(10);
	HT162x_Command(CMD_RC_INT);
	ets_delay_us(10);
	HT162x_Command(CMD_SYS_EN);
	ets_delay_us(10);
	HT162x_Command(CMD_LCD_ON);
	ets_delay_us(10);

	AllElements(0);

	HT1622_Config_s = (HT1622_DisplayConfig)malloc(sizeof(HT1622_DisplayConfig_t[1]));
	HT1622_Values_s = (HT1622_DisplayValues)malloc(sizeof(HT1622_DisplayValues_t[1]));

	if(HT1622_Config_s != NULL && HT1622_Values_s != NULL){
		HT1622_Config_s->VelocityUnit = KPH;
		HT1622_Config_s->DistanceUnit = METER;
		HT1622_Config_s->State = HT1622_INIT;
		HT1622_Config_s->Mode = CLUB_AND_BALL_SPEED;
		HT1622_Config_s->RefreshFlag = false;

		HT1622_Values_s->BatteryPercentage = 0;
		HT1622_Values_s->ClubSpeed = 0;
		HT1622_Values_s->BallSpeed = 0;
		HT1622_Values_s->Launch = 0.0;
		HT1622_Values_s->Smash = 0.0;
		HT1622_Values_s->Carry = 0;
	}
}

// To set or clear all elements at the same time.
void AllElements(uint8_t state)
{
	for (uint8_t addr = 0; addr < HT1622_ADDRESS_SIZE; addr++)
	{
		for(int i=0; i< HT1622_ADDRESS_SIZE; i++){
			HT1622_Memory[i] = (state ? 0xF : 0x0);
		}
		HT162x_WriteData(addr, (state ? 0xF : 0x0), 4);
	}
}

// To restart and reconsider all segments that are for unit labels, according to their states.
void HT162x_RestartUnitLabels(void){
	switch(HT1622_Config_s->Mode){
		case CLUB_AND_BALL_SPEED:
			if(HT1622_Config_s->VelocityUnit == KPH){
				HT162x_Z_Set(Z8_LOCATION, false);
				HT162x_Z_Set(Z9_LOCATION, true);
				HT162x_Z_Set(Z12_LOCATION, false);
				HT162x_Z_Set(Z13_LOCATION, true);
			}
			else if(HT1622_Config_s->VelocityUnit == MPH){
				HT162x_Z_Set(Z8_LOCATION, true);
				HT162x_Z_Set(Z9_LOCATION, false);
				HT162x_Z_Set(Z12_LOCATION, true);
				HT162x_Z_Set(Z13_LOCATION, false);
			}

			if(HT1622_Config_s->DistanceUnit == METER){
				HT162x_Z_Set(Z19_LOCATION, true);
				HT162x_Z_Set(Z20_LOCATION, false);
			}
			else if(HT1622_Config_s->DistanceUnit == YARD){
				HT162x_Z_Set(Z19_LOCATION, false);
				HT162x_Z_Set(Z20_LOCATION, true);
			}
			break;
		case CLUB_SPEED:
			HT162x_Z_Set(Z12_LOCATION, false);
			HT162x_Z_Set(Z13_LOCATION, false);
			HT162x_Z_Set(Z19_LOCATION, false);
			HT162x_Z_Set(Z20_LOCATION, false);
			if(HT1622_Config_s->VelocityUnit == KPH){
				HT162x_Z_Set(Z8_LOCATION, false);
				HT162x_Z_Set(Z9_LOCATION, true);

			}
			else if(HT1622_Config_s->VelocityUnit == MPH){
				HT162x_Z_Set(Z8_LOCATION, true);
				HT162x_Z_Set(Z9_LOCATION, false);
			}
			break;
		case BALL_SPEED:
			HT162x_Z_Set(Z8_LOCATION, false);
			HT162x_Z_Set(Z9_LOCATION, false);
			HT162x_Z_Set(Z19_LOCATION, false);
			HT162x_Z_Set(Z20_LOCATION, false);
			if(HT1622_Config_s->VelocityUnit == KPH){
				HT162x_Z_Set(Z12_LOCATION, false);
				HT162x_Z_Set(Z13_LOCATION, true);
			}
			else if(HT1622_Config_s->VelocityUnit == MPH){
				HT162x_Z_Set(Z12_LOCATION, true);
				HT162x_Z_Set(Z13_LOCATION, false);
			}
			break;
	}
}

// To restart and reconsider all segments that are for labels, according to their states.
void HT162x_RestartStaticLabels(void){
	switch(HT1622_Config_s->Mode){
		case CLUB_AND_BALL_SPEED:
			HT162x_Z_Set(Z1_LOCATION, true);
			HT162x_Z_Set(Z2_LOCATION, true);
			HT162x_Z_Set(Z10_LOCATION, true);
			HT162x_Z_Set(Z11_LOCATION, true);
			HT162x_Z_Set(Z14_LOCATION, true);
			HT162x_Z_Set(Z16_LOCATION, true);
			HT162x_Z_Set(Z18_LOCATION, true);
			HT162x_Z_Set(Z15_LOCATION, true);
			HT162x_Z_Set(Z17_LOCATION, true);
			HT162x_Z_Set(Z21_LOCATION, true);
			break;
		case CLUB_SPEED:
			HT162x_Z_Set(Z1_LOCATION, true);
			HT162x_Z_Set(Z2_LOCATION, true);
			HT162x_Z_Set(Z10_LOCATION, true);
			HT162x_Z_Set(Z11_LOCATION, false);
			HT162x_Z_Set(Z14_LOCATION, false);
			HT162x_Z_Set(Z16_LOCATION, false);
			HT162x_Z_Set(Z18_LOCATION, false);
			HT162x_Z_Set(Z15_LOCATION, false);
			HT162x_Z_Set(Z17_LOCATION, false);
			HT162x_Z_Set(Z21_LOCATION, false);
			break;
		case BALL_SPEED:
			HT162x_Z_Set(Z1_LOCATION, true);
			HT162x_Z_Set(Z2_LOCATION, true);
			HT162x_Z_Set(Z10_LOCATION, false);
			HT162x_Z_Set(Z11_LOCATION, true);
			HT162x_Z_Set(Z14_LOCATION, true);
			HT162x_Z_Set(Z16_LOCATION, true);
			HT162x_Z_Set(Z18_LOCATION, false);
			HT162x_Z_Set(Z15_LOCATION, false);
			HT162x_Z_Set(Z17_LOCATION, true);
			HT162x_Z_Set(Z21_LOCATION, true);
			break;
	}
}

// To restart and reconsider all segments that are for battery icon, according to their states.
void HT162x_RestartBatteryIcon(){
	switch(HT1622_Values_s->BatteryPercentage){
		case PERCENT_100_85:
			HT162x_Z_Set(Z3_LOCATION, true);
			HT162x_Z_Set(Z4_LOCATION, true);
			HT162x_Z_Set(Z5_LOCATION, true);
			HT162x_Z_Set(Z6_LOCATION, true);
			HT162x_Z_Set(Z7_LOCATION, true);
			break;
		case PERCENT_85_70:
			HT162x_Z_Set(Z3_LOCATION, true);
			HT162x_Z_Set(Z4_LOCATION, true);
			HT162x_Z_Set(Z5_LOCATION, true);
			HT162x_Z_Set(Z6_LOCATION, true);
			HT162x_Z_Set(Z7_LOCATION, false);
			break;
		case PERCENT_70_55:
			HT162x_Z_Set(Z3_LOCATION, true);
			HT162x_Z_Set(Z4_LOCATION, true);
			HT162x_Z_Set(Z5_LOCATION, true);
			HT162x_Z_Set(Z6_LOCATION, false);
			HT162x_Z_Set(Z7_LOCATION, false);
			break;
		case PERCENT_55_40:
			HT162x_Z_Set(Z3_LOCATION, true);
			HT162x_Z_Set(Z4_LOCATION, true);
			HT162x_Z_Set(Z5_LOCATION, false);
			HT162x_Z_Set(Z6_LOCATION, false);
			HT162x_Z_Set(Z7_LOCATION, false);
			break;
		case PERCENT_40_25:
			HT162x_Z_Set(Z3_LOCATION, true);
			HT162x_Z_Set(Z4_LOCATION, false);
			HT162x_Z_Set(Z5_LOCATION, false);
			HT162x_Z_Set(Z6_LOCATION, false);
			HT162x_Z_Set(Z7_LOCATION, false);
			break;
		case PERCENT_25_00:
			HT162x_Z_Set(Z3_LOCATION, false);
			HT162x_Z_Set(Z4_LOCATION, false);
			HT162x_Z_Set(Z5_LOCATION, false);
			HT162x_Z_Set(Z6_LOCATION, false);
			HT162x_Z_Set(Z7_LOCATION, false);
			break;
	}
}

// System open-up LCD test function. You can change it according to your needs.
void HT162x_OpenUp_Test(void){
	AllElements(1);
	gpio_set_level(BUZZER_PIN, 1);
//	vTaskDelay( 1000 / portTICK_PERIOD_MS );
	ets_delay_us(1000000);

	AllElements(0);
	gpio_set_level(BUZZER_PIN, 0);

	HT162x_RestartUnitLabels();
	HT162x_RestartStaticLabels();

	for(int i = 0; i<10; i++){
		HT162x_CaptureMode_Set(i*11);
		HT162x_ClubSpeed_Set(i*111);
		HT162x_BallSpeed_Set(i*111);
		HT162x_Smash_Set((float)i*(float)11.1);
		HT162x_Launch_Set((float)i*(float)11.1);
		HT162x_Carry_Set(i*111);
		ets_delay_us(250000);
	}

	HT162x_CaptureMode_Set(0);
	HT162x_ClubSpeed_Set(0);
	HT162x_BallSpeed_Set(0);
	HT162x_Smash_Set((float)0.0);
	HT162x_Launch_Set((float)0.0);
	HT162x_Carry_Set(0);
}

// Task creation function.
void HT162x_ActivateTask(void)
{
	xTaskCreatePinnedToCore(&display_task, "display_task", 4096, NULL, DISPLAY_TASK_PRIORITY, &displayTaskHandle, FIRST_CPU);
}

// LCD task
void display_task(void *arg)
{
	while(1)
	{
		switch(HT1622_Config_s->State){
			case HT1622_INIT:
				HT1622_Config_s->State = HT1622_REFRESH;
				HT162x_OpenUp_Test();
				break;
			case HT1622_IDLE:
				if(HT1622_Config_s->RefreshFlag){
					HT1622_Config_s->State = HT1622_REFRESH;
					gpio_set_level(BUZZER_PIN, 1);
				}
				break;
			case HT1622_REFRESH:
				HT1622_Config_s->State = HT1622_IDLE;
				HT1622_Config_s->RefreshFlag = false;
				gpio_set_level(BUZZER_PIN, 0);
				switch(HT1622_Config_s->Mode){
					case CLUB_AND_BALL_SPEED:
						HT162x_CaptureMode_Set(HT1622_Config_s->Mode);
						HT162x_ClubSpeed_Set(HT1622_Values_s->ClubSpeed);
						HT162x_BallSpeed_Set(HT1622_Values_s->BallSpeed);
						HT162x_Smash_Set(HT1622_Values_s->Smash);
						HT162x_Launch_Set(HT1622_Values_s->Launch);
						HT162x_Carry_Set(HT1622_Values_s->Carry);
						break;
					case CLUB_SPEED:
						HT162x_CaptureMode_Set(HT1622_Config_s->Mode);
						HT162x_ClubSpeed_Set(HT1622_Values_s->ClubSpeed);
						HT162x_BallSpeed_Release();
						HT162x_Smash_Release();
						HT162x_Launch_Release();
						HT162x_Carry_Release();
						break;
					case BALL_SPEED:
						HT162x_CaptureMode_Set(HT1622_Config_s->Mode);
						HT162x_ClubSpeed_Release();
						HT162x_BallSpeed_Set(HT1622_Values_s->BallSpeed);
						HT162x_Smash_Release();
						HT162x_Launch_Set(HT1622_Values_s->Launch);
						HT162x_Carry_Set(HT1622_Values_s->Carry);
						break;
				}
				HT162x_RestartUnitLabels();
				HT162x_RestartStaticLabels();
				break;
			default:
				break;
		}
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}
