/*
 * Messenger.h
 *
 *  Created on: 28-Jan-2021
 *      Author: elvez
 */

#ifndef MESSENGER_H_
#define MESSENGER_H_

//#define DEBUGGER_ENABLED
#define USB_ENABLED
//#define I2C_ENABLED
//#define UART_ENABLED

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "usbd_cdc_if.h"
#include "main.h"
#include <stdlib.h>
#include <stdarg.h>
#include <utility>
#include <string>
#include <vector>

#ifdef UART_ENABLED
	extern UART_HandleTypeDef huart1;
#endif

#ifdef DEBUGGER_ENABLED
	extern UART_HandleTypeDef huart2;
#endif

#ifdef I2C_ENABLED
	extern I2C_HandleTypeDef hi2c1;
#endif


class Messenger
{
public:
	Messenger();
	virtual ~Messenger();

#ifdef UART_ENABLED
	typedef struct
	{
		uint16_t length;
		char* message;
		UART_HandleTypeDef *uart;
	} uartMessage;
#endif

	typedef struct
	{
		char* message;
		uint16_t length;
	} usbMessage;

#ifdef I2C_ENABLED
	typedef struct
	{
		I2C_HandleTypeDef *i2c;
		uint8_t* pData;
		uint16_t length;
		uint32_t timeout;
		uint16_t address;
	} i2cMessage;
#endif

	typedef enum
	{
		USB,
		UART
	} sender;

public:
	void sendMessage(sender thisSender, std::string message);
	void sendMessage(sender thisSender, char* format,...);
#ifdef I2C_ENABLED
	void i2cTransmit(i2cMessage packet);
#endif
	void debugMessage(std::string message);
	void debugMessage(char* format, ...);


private:
	void uartSend(std::string message);
	void usbSend(std::string message);
	void uartSend(char* packet_);
	void usbSend(char* packet_);
	void debugSend(std::string message);
	void debugSend(char* packet_);
};

#endif /* MESSENGER_H_ */
