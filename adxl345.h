/*
 * adxl345.h
 *
 *  Created on : Nov 28, 2024
 *  	Author : hayati
 */

#include "main.h"
#include <stdint.h>

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

// @def_group device_address
#define 	ADXL345_DEVICE_ADDRESS		(0xA6)			// Device address (we found, using ADXL345_ScanDeviceID function)

// @def_group register
#define		DEV_ID						(0x00)			// Device ID
#define 	THRESH_TAP					(0x1D)			// Tap threshold
#define 	OFSX						(0x1E)			// X axis offset
#define 	OFSY						(0x1F)			// Y axis offset
#define 	OFSZ						(0x20)			// Z axis offset
#define 	DUR							(0x21)			// Tap duration
#define 	LATENT						(0x22)			// Tap latency
#define		WINDOW						(0x23)			// Tap window
#define 	THRESH_ACT					(0x24)			// Activity threshold
#define 	THRESH_INACT				(0x25)			// Inactivity threshold
#define 	TIME_INACT  				(0x26)			// Inactivity time
#define 	ACT_INACT_CTL   			(0x27)			// Axis enable control for ACT/INACT
#define 	THRESH_FF					(0x28)			// Free-fall threshold
#define 	TIME_FF						(0x29)			// Free-fall time
#define 	TAP_AXES					(0x2A)			// Axis control for Tap/Double Tap
#define 	ACT_TAP_STATUS  			(0x2B)			// Source of Tap/Double Tap
#define 	BW_RATE						(0x2C)			// Data Rate and Power Mode control
#define 	POWER_CTL					(0x2D)			// Power Save features control
#define 	INT_ENABLE					(0x2E)			// Interrupt enable control
#define 	INT_MAP						(0x2F)			// Interrupt mapping control
#define 	INT_SOURCE					(0x30)			// Source of interrupts
#define 	DATA_FORMAT					(0x31)			// Data format control
#define 	DATAX0						(0x32)			// X axis data
#define 	DATAX1						(0x33)			// X axis data
#define 	DATAY0						(0x34)			// Y axis data
#define 	DATAY1						(0x35)			// Y axis data
#define 	DATAZ0						(0x36)			// Z axis data
#define 	DATAZ1						(0x37)			// Z axis data
#define 	FIFO_CTL					(0x38)			// FIFO control
#define 	FIFO_STATUS 				(0x39)			// FIFO status

// @def_group axis
#define 	X							DATAX0
#define 	Y							DATAY0
#define		Z							DATAZ0

// @def_group scale_factor
#define 	SCALE_FACTOR_2G				((float)1 / 256.0)
#define 	SCALE_FACTOR_4G				((float)1 / 128.0)
#define		SCALE_FACTOR_8G				((float)1 / 64.0)
#define 	SCALE_FACTOR_16G			((float)1 / 32.0)


// @def_group wakeup_frequency
#define 	WAKEUP_8HZ					(0x00)
#define 	WAKEUP_4HZ					(0x01)
#define 	WAKEUP_2HZ					(0x02)
#define	 	WAKEUP_1HZ					(0x03)

// @def_group range
#define 	RANGE_2G					(0x00)
#define 	RANGE_4G					(0x01)
#define 	RANGE_8G					(0x02)
#define		RANGE_16G					(0x03)

// @def_group bw_rate
#define 	BANDWITH_RATE_1600			(0x0F)
#define 	BANDWITH_RATE_800			(0x0E)
#define 	BANDWITH_RATE_400			(0x0D)
#define 	BANDWITH_RATE_200			(0x0C)
#define 	BANDWITH_RATE_100			(0x0B)
#define 	BANDWITH_RATE_50			(0x0A)
#define 	BANDWITH_RATE_25			(0x09)

// @def_group on/off state
#define 	DISABLE						(0x00)
#define 	ENABLE						(0x01)


typedef enum
{
	READ_FAIL = 0,
	READ_SUCCESS = 1,
}ADXL345ReadStatus_t;

typedef enum
{
	WRITE_FAIL = 0,
	WRITE_SUCCESS = 1,
}ADXL345WriteStatus_t;

typedef enum
{
	INIT_FAIL = 0,
	INIT_SUCCESS = 1,
}ADXL345InitStatus_t;



typedef struct
{
	uint8_t Wakeup  	: 2;			// Bits 0-1 (@WAKEUP_FREQUENCY) -> 00 : 8Hz, 01 : 4Hz, 10 : 2Hz, 11 : 1Hz
	uint8_t Sleep   	: 1;			// Bit 2 (Sleep)
	uint8_t Measure 	: 1;			// Bit 3 (Measure)	(default 0x01)
	uint8_t Auto_Sleep 	: 1;			// Bit 4 (Auto Sleep)
	uint8_t Link 		: 1;			// Bit 5 (Link)
	uint8_t Reserved 	: 2;			// Bit 6-7 (Reserved)

}PowerControlRegister_t;


typedef struct
{
	uint8_t Range 		: 2;			// Bits 0-1 (Range) 	-> 00 : 2g, 01 : 4g, 10 : 8g, 11 : 16g
	uint8_t Justify 	: 1;			// Bit 2 (Justify)		-> set 0 for right-justified, set 1 for left-justified
	uint8_t FullRes 	: 1;			// Bit 3 (Full_res) 	-> set 0 for 10 bit resolution
	uint8_t Reserved 	: 1;			// Bit 4 (Reserved)
	uint8_t IntInvert 	: 1;			// Bit 5 (Int_Invert) 	-> enable = 1, disable = 0
	uint8_t Spi			: 1;			// Bit 6 (SPI)			-> set 0 for 4 wire SPI mode, set 1 for 3 wire SPI mode
	uint8_t SelfTest	: 1; 			// Bit 7 (Self Test)    -> enable = 1, disable = 0

}DataFormatRegister_t;


typedef struct
{
	uint8_t Rate : 4;					// Bits 0-3 (Rate)		(default 100 Hz, 0x0A)
	uint8_t LowPower : 1;				// Bit 4	(Low Power)
	uint8_t Reserved : 3;				// Bits 5-7 (Reserved)

}BW_RateRegister_t;


int ADXL345_ScanDeviceID(I2C_HandleTypeDef *hi2cx);				// This function find device address

ADXL345InitStatus_t ADXL345_Init(I2C_HandleTypeDef *hi2cx);

//ADXL345ReadStatus_t ADXL345_ReadRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t regAddress, uint16_t sizeofData, uint8_t *dataBuffer);

//ADXL345WriteStatus_t ADXL345_WriteRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t regAddress, uint16_t value);

int16_t ADXL345_GetAxisValue(I2C_HandleTypeDef *hi2cx, uint8_t axis);

float ADXL345_GetGValue(I2C_HandleTypeDef *hi2cx, uint8_t axis, float scaleFactor);


#endif /* INC_ADXL345_H_ */
