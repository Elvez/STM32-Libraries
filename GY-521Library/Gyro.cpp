/*
 * Gyro.cpp
 *
 *  Created on: 01-Feb-2021
 *      Author: elvez
 */
#include <stdlib.h>
#include <string.h>
#include <IMULibrary/Gyro.h>

Gyro::Gyro()
{
	defaultConfig.accFS = AFS_2G;
	defaultConfig.clockSource = INTERNAL_8MHZ;
	defaultConfig.sleepMode = false;
	defaultConfig.configDLPF = DLPF_184A_188G_Hz;
	defaultConfig.gyroFS = FS_250;
}

Gyro::~Gyro()
{

}

void Gyro::init(MPU_ConfigTypeDef config)
{
	uint8_t buffer_ = 0;

	writeData(pwrMgmtReg_, 0x80);
	HAL_Delay(100);
	buffer_ = config.clockSource & 0x07;
	buffer_ |= (config.sleepMode << 6) &0x40;
	writeData(pwrMgmtReg_, buffer_);
	HAL_Delay(100);

	buffer_ = 0;
	buffer_ = config.configDLPF & 0x07;
	writeData(configReg_, buffer_);

	buffer_ = 0;
	buffer_ = (config.gyroFS << 3) & 0x18;
	writeData(configGyroReg_, buffer_);

	buffer_ = 0;
	buffer_ = (config.accFS << 3) & 0x18;
	writeData(configAccReg_, buffer_);
	setSampleRareDiv(0x04);

	switch (config.accFS)
	{
		case AFS_2G:
			accelScalingFactor = (2000.0f/32768.0f);
			break;

		case AFS_4G:
			accelScalingFactor = (4000.0f/32768.0f);
			break;

		case AFS_8G:
			accelScalingFactor = (8000.0f/32768.0f);
			break;

		case AFS_16G:
			accelScalingFactor = (16000.0f/32768.0f);
			break;

		default:
			break;
	}

	switch (config.gyroFS)
	{
		case FS_250:
			gyroScalingFactor = 250.0f/32768.0f;
			break;

		case FS_500:
			gyroScalingFactor = 500.0f/32768.0f;
			break;

		case FS_1000:
			gyroScalingFactor = 1000.0f/32768.0f;
			break;

		case FS_2000:
			gyroScalingFactor = 2000.0f/32768.0f;
			break;

		default:
			break;
	}
}

void Gyro::initDefault(void)
{
	init(defaultConfig);
}

Gyro::RawData Gyro::getRawAcc(void)
{
	uint8_t buffer_[2];
	uint8_t acceArr[6], gyroArr[6];

	readData(intStatusReg_, &buffer_[1],1);
	if((buffer_[1] && 0x01))
	{
		readData(xOutAcc_, acceArr, 6);

		rawDataAcc.xData = ((acceArr[0] << 8) + acceArr[1]);
		rawDataAcc.yData = ((acceArr[2] << 8) + acceArr[3]);
		rawDataAcc.zData = ((acceArr[4] << 8) + acceArr[5]);

		readData(xOutGyro_, gyroArr, 6);
		gyroRW[0] = ((gyroArr[0] << 8) + gyroArr[1]);
		gyroRW[1] = ((gyroArr[2] << 8) + gyroArr[3]);
		gyroRW[2] = ((gyroArr[4] << 8) + gyroArr[5]);
	}
	return rawDataAcc;
}

Gyro::RawData Gyro::getRawGyr(void)
{
	rawDataGyro.xData = gyroRW[0];
	rawDataGyro.yData = gyroRW[1];
	rawDataGyro.zData = gyroRW[2];

	return rawDataGyro;
}


Gyro::ScaledData Gyro::getScaledAcc(void)
{
	RawData accelRData;
	accelRData = getRawAcc();

	scaledDataAcc.xData = ((accelRData.xData + 0.0f)*accelScalingFactor);
	scaledDataAcc.yData = ((accelRData.yData + 0.0f)*accelScalingFactor);
	scaledDataAcc.zData = ((accelRData.zData + 0.0f)*accelScalingFactor);

	return scaledDataAcc;
}

Gyro::ScaledData Gyro::getScaledGyr(void)
{
	RawData gyroRaw;
	gyroRaw = getRawGyr();

	scaledDataGyro.xData = (gyroRaw.xData)*gyroScalingFactor;
	scaledDataGyro.yData = (gyroRaw.yData)*gyroScalingFactor;
	scaledDataGyro.zData = (gyroRaw.zData)*gyroScalingFactor;

	return scaledDataGyro;
}
uint8_t Gyro::getSampleRateDiv(void)
{
	uint8_t buffer_ = 0;

	readData(smplrtDivReg_, &buffer_, 1);
	return buffer_;
}

void Gyro::setSampleRareDiv(uint8_t smplrt_)
{
	writeData(smplrtDivReg_, smplrt_);
}

uint8_t Gyro::getFrameSync(void)
{
	uint8_t buffer_ = 0;

	readData(configReg_, &buffer_, 1);
	buffer_ &= 0x38;
	return (buffer_>>3);
}

void Gyro::setFrameSync(Gyro::frameSync fs)
{
	uint8_t buffer_ = 0;

	readData(configReg_, &buffer_,1);
	buffer_ &= ~0x38;
	buffer_ |= (fs <<3);
	writeData(configReg_, buffer_);
}

void Gyro::readData(uint8_t addr, uint8_t *buffer, uint8_t len)
{
	uint8_t txBuffer[2];
	uint8_t MPUADDR = (baseAddr_<<1);

	txBuffer[0] = addr;
	HAL_I2C_Master_Transmit(&mpuI2C, MPUADDR, txBuffer, 1, 10);
	HAL_I2C_Master_Receive(&mpuI2C, MPUADDR, buffer, len, 100);
}

void Gyro::writeData(uint8_t addr, uint8_t data)
{
	uint8_t i2cData[2];
	uint8_t MPUADDR = (baseAddr_<<1);

	i2cData[0] = addr;
	i2cData[1] = data;
	HAL_I2C_Master_Transmit(&mpuI2C, MPUADDR, i2cData, 2, 100);
}

void Gyro::getAccCalib(ScaledData calibrationData)
{
	ScaledData accelScaled;
	accelScaled = getScaledAcc();

	calibrationData.xData = (accelScaled.xData) - accXBias;
	calibrationData.yData = (accelScaled.yData) - accYBias;
	calibrationData.zData = (accelScaled.zData) - accZBias;
}

void Gyro::accelCalib(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	accXBias = (xMax + xMin)/2.0f;
	accYBias = (yMax + yMin)/2.0f;
	accZBias = (zMax + zMin)/2.0f;
}



