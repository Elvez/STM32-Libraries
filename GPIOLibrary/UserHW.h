/*
 * UserHW.h
 *
 *  Created on: Jan 27, 2021
 *      Author: elvez
 */

#ifndef USERHW_H_
#define USERHW_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "main.h"
#include <stdlib.h>
#include <utility>
#include <string>
#include <vector>

#define LedPort		GPIOC
#define LedPin		GPIO_PIN_13
#define ButtonPort	GPIOA
#define ButtonPin	GPIO_PIN_0



class smartGPIO
{
public:
	smartGPIO();
	virtual ~smartGPIO();

	typedef struct
	{
		GPIO_TypeDef* portName;
		uint16_t pinNumber;
	} GPIO;

public:
	void strobe(int turns, uint32_t delay);
	void writeHigh(void);
	void writeLow(void);
	bool getPinState(void);
	void setupGPIO(GPIO_TypeDef* portName, uint16_t number);
	void setupGPIO(char portName, uint16_t number);
	void addToStack(GPIO pin);
	void addToStack(char port, uint16_t fromPin, uint16_t toPin);
	std::vector<smartGPIO::GPIO> getStack(void);
	GPIO_TypeDef* getPort(void);
	uint16_t getPinNumber(void);
	void clearStack(void);

private:
	GPIO_TypeDef* port;
	uint16_t pinNum;
	bool alone = true;
	std::vector<GPIO> pairedPins;

	typedef enum
	{
		LOW = GPIO_PIN_RESET,
		HIGH = GPIO_PIN_SET
	} pinState;
};

#endif /* USERHW_H_ */
