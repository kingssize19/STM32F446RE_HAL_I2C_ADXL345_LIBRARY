/*
 * adxl345.c
 *
 *  Created on: Nov 28, 2024
 *      Author: hayati
 */

#include "adxl345.h"


extern I2C_HandleTypeDef hi2c1;


// Device bağlı mı, adresini alabiliyor muyuz onu kontrol eden fonksiyon. (0xA6 değerini bulduk address değeri)
int ADXL345_ScanDeviceID(I2C_HandleTypeDef *hi2cx)
{

	for (uint8_t address = 0; address < 255; ++address)
	{
		if (HAL_I2C_IsDeviceReady(hi2cx, address, 1, 1000) == HAL_OK)
		{
			return address;
		}
	}

	return -1;

}

static ADXL345ReadStatus_t ADXL345_ReadRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t regAddress, uint16_t sizeofData, uint8_t *dataBuffer)
{
	if (HAL_I2C_Mem_Read(hi2cx, ADXL345_DEVICE_ADDRESS, regAddress, 1, dataBuffer, sizeofData, 1000) == HAL_OK)
	{
		return READ_SUCCESS;
	}

	return READ_FAIL;
}

static ADXL345WriteStatus_t ADXL345_WriteRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t regAddress, uint16_t value)
{
	uint8_t data[2] = {0};
	data[0] = regAddress;
	data[1] = value;

	if (HAL_I2C_Master_Transmit(hi2cx, ADXL345_DEVICE_ADDRESS, data, sizeof(data), 1000) == HAL_OK)
	{
		return WRITE_SUCCESS;
	}

	return WRITE_FAIL;
}

ADXL345InitStatus_t ADXL345_Init(I2C_HandleTypeDef *hi2cx)
{
	uint8_t dataBuffer = 0;

	ADXL345_ReadRegisterData(hi2cx, DEV_ID, 1, &dataBuffer);

	if (dataBuffer != 0xE5)
	{
		return INIT_FAIL;
	}

	uint8_t tempReg = 0;
	PowerControlRegister_t powerControl = {0};

	powerControl.Wakeup = WAKEUP_8HZ;
	powerControl.Sleep = DISABLE;
	powerControl.Measure = ENABLE;
	powerControl.Auto_Sleep = DISABLE;
	powerControl.Link = DISABLE;
	powerControl.Reserved = DISABLE;

	tempReg = *((uint8_t *)&powerControl);
	ADXL345_WriteRegisterData(hi2cx, POWER_CTL, tempReg);


	DataFormatRegister_t dataFormatControl = {0};

	dataFormatControl.Range = RANGE_4G;
	dataFormatControl.Justify = DISABLE;
	dataFormatControl.FullRes = DISABLE;
	dataFormatControl.Reserved = DISABLE;
	dataFormatControl.IntInvert = DISABLE;
	dataFormatControl.Spi = DISABLE;
	dataFormatControl.SelfTest = DISABLE;

	tempReg = *((uint8_t *)&dataFormatControl);
	ADXL345_WriteRegisterData(hi2cx, DATA_FORMAT, tempReg);

	BW_RateRegister_t bwRateControl = {0};

	bwRateControl.Rate = BANDWITH_RATE_800;
	bwRateControl.LowPower = DISABLE;
	bwRateControl.Reserved = DISABLE;

	tempReg = *((uint8_t *)&bwRateControl);
	ADXL345_WriteRegisterData(hi2cx, BW_RATE, tempReg);


	return INIT_SUCCESS;
}


int16_t ADXL345_GetAxisValue(I2C_HandleTypeDef *hi2cx, uint8_t axis)
{
	uint8_t data[2] = {0};

	int16_t outputData = 0;

	ADXL345_ReadRegisterData(hi2cx ,axis, 2, data);

	outputData = ((data[1] << 8) | (data[0]));

	return outputData;
}

float ADXL345_GetGValue(I2C_HandleTypeDef *hi2cx, uint8_t axis, float scaleFactor)
{
	int16_t outputData = 0;
	float gData = 0;

	outputData = ADXL345_GetAxisValue(hi2cx, axis);

	gData = (float) (outputData * scaleFactor);

	return gData;
}











