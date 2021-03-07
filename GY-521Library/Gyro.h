/*
 * Gyro.h
 *
 *  Created on: 01-Feb-2021
 *      Author: elvez
 */

#ifndef INC_IMULIBRARY_GYRO_H_
#define INC_IMULIBRARY_GYRO_H_

#include "main.h"
#include "GpioLibrary/UserHW.h"
#include "ComLibrary/Messenger.h"

#define whoAmI_				0x75
#define baseAddr_			0x68
#define pwrMgmtReg_			0x6B
#define configReg_			0x1A
#define configGyroReg_		0x1B
#define configAccReg_		0x1C
#define smplrtDivReg_		0x19
#define intStatusReg_		0x3A
#define xOutAcc_			0x3B
#define tempOutReg_			0x41
#define xOutGyro_			0x43
#define fifoEnReg_	 		0x23
#define intEnableReg_ 		0x38
#define i2cCMACO_	 		(fifoEnReg_)
#define userCtrlReg_		0x6A
#define fifoCountReg_	 	0x72
#define fifoRWReg_	 		0x74


class Gyro {
public:
	Gyro();
	virtual ~Gyro();

	typedef struct
	{
		uint8_t 	clockSource;
		uint8_t 	gyroFS;
		uint8_t		accFS;
		uint8_t 	configDLPF;
		bool 		sleepMode;
	} MPU_ConfigTypeDef;

	typedef struct
	{
		int16_t xData;
		int16_t yData;
		int16_t zData;
	} RawData;

	typedef struct
	{
		float xData;
		float yData;
		float zData;
	} ScaledData;

	typedef enum
	{
		INTERNAL_8MHZ 		= 0x00,
		X_AXIS_REF			= 0x01,
		Y_AXIS_REF			= 0x02,
		Z_AXIS_REF			= 0x03,
		EXTERNAL_32KHZ		= 0x04,
		EXTERNAL_19_2MHZ	= 0x05,
		TIM_GENT_INREST		= 0x07
	} clockSelect;

	typedef enum
	{
		FS_250 	= 0x00,
		FS_500 	= 0x01,
		FS_1000 = 0x02,
		FS_2000	= 0x03
	} gyroConfig;

	typedef enum
	{
		AFS_2G	= 0x00,
		AFS_4G,
		AFS_8G,
		AFS_16G
	} accelConfig;

	typedef enum
	{
		DLPF_260A_256G_Hz	= 0x00,
		DLPF_184A_188G_Hz 	= 0x01,
		DLPF_94A_98G_Hz 	= 0x02,
		DLPF_44A_42G_Hz 	= 0x03,
		DLPF_21A_20G_Hz 	= 0x04,
		DLPF_10_Hz 			= 0x05,
		DLPF_5_Hz 			= 0x06
	} digitalLPF;

	typedef enum
	{
		INPUT_DISABLE   = 0x00,
		TEMP_OUT_L		= 0x01,
		GYRO_XOUT_L		= 0x02,
		GYRO_YOUT_L		= 0x03,
		GYRO_ZOUT_L		= 0x04,
		ACCEL_XOUT_L	= 0x05,
		ACCEL_YOUT_L	= 0x06,
		ACCEL_ZOUT_L	= 0x07
	} frameSync;

	void initDefault(void);
	void init(MPU_ConfigTypeDef config);
	RawData getRawAcc(void);
	RawData getRawGyr(void);
	ScaledData getScaledAcc(void);
	ScaledData getScaledGyr(void);
	uint8_t getSampleRateDiv(void);
	void setSampleRareDiv(uint8_t smplrt_);
	uint8_t getFrameSync(void);
	void setFrameSync(Gyro::frameSync fs);


private:
	void readData(uint8_t addr, uint8_t *buffer, uint8_t len);
	void writeData(uint8_t addr, uint8_t data);
	void getAccCalib(ScaledData calibrationData);
	void accelCalib(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

	int16_t gyroRW[3];
	RawData rawDataAcc;
	ScaledData scaledDataAcc;
	RawData rawDataGyro;
	ScaledData scaledDataGyro;
	I2C_HandleTypeDef mpuI2C = hi2c1;
	float accelScalingFactor, gyroScalingFactor;
	float accXBias = 0.0f;
	float accYBias = 0.0f;
	float accZBias = 0.0f;
	MPU_ConfigTypeDef defaultConfig;
	Messenger gyroRXTX;

};

#endif /* INC_IMULIBRARY_GYRO_H_ */
