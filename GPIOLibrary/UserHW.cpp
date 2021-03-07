/*
 * UserHW.cpp
 *
 *  Created on: Jan 27, 2021
 *      Author: elvez
 */

#include "GpioLibrary/UserHW.h"

smartGPIO::smartGPIO()
{

}

smartGPIO::~smartGPIO()
{

}

void smartGPIO::writeHigh(void)
{
	pinState state_ = HIGH;
	HAL_GPIO_WritePin(port, pinNum, GPIO_PinState(state_));
}

void smartGPIO::writeLow(void)
{
	pinState state_ = LOW;
	HAL_GPIO_WritePin(port, pinNum, GPIO_PinState(state_));
}

void smartGPIO::strobe(int turns, uint32_t delay)
{
	while(turns >= -1)
	{
		HAL_GPIO_TogglePin(port, pinNum);
		HAL_Delay(delay);
		HAL_GPIO_TogglePin(port, pinNum);
		HAL_Delay(delay);

		turns -= 1;
	}
}

bool smartGPIO::getPinState(void)
{
	pinState state;
	state = pinState(HAL_GPIO_ReadPin(port, pinNum));

	if(state)
		return true;
	else
		return false;
}

void smartGPIO::setupGPIO(GPIO_TypeDef* portName, uint16_t number)
{
	alone = true;
	pinNum = number;
	port = portName;
}

void smartGPIO::setupGPIO(char portName, uint16_t number)
{
	alone = true;
	GPIO_TypeDef* port_;

	switch(portName)
	{
	case 'A':
		port_ = GPIOA;
		break;
	case 'B':
		port_ = GPIOB;
		break;
	case 'C':
		port_ = GPIOC;
		break;
	case 'D':
		port_ = GPIOD;
		break;
	case 'E':
		port_ = GPIOE;
		break;
	case 'H':
		port_ = GPIOH;
		break;
	default:
		break;
	}

	switch(number)
	{
	case 0:
		number = GPIO_PIN_0;
		break;
	case 1:
		number = GPIO_PIN_1;
		break;
	case 2:
		number = GPIO_PIN_2;
		break;
	case 3:
		number = GPIO_PIN_3;
		break;
	case 4:
		number = GPIO_PIN_4;
		break;
	case 5:
		number = GPIO_PIN_5;
		break;
	case 6:
		number = GPIO_PIN_6;
		break;
	case 7:
		number = GPIO_PIN_7;
		break;
	case 8:
		number = GPIO_PIN_8;
		break;
	case 9:
		number = GPIO_PIN_9;
		break;
	case 10:
		number = GPIO_PIN_10;
		break;
	case 11:
		number = GPIO_PIN_11;
		break;
	case 12:
		number = GPIO_PIN_12;
		break;
	case 13:
		number = GPIO_PIN_13;
		break;
	case 14:
		number = GPIO_PIN_14;
		break;
	case 15:
		number = GPIO_PIN_15;
		break;
	}

	pinNum = number;
	port = port_;
}

void smartGPIO::addToStack(GPIO pin)
{
	alone = false;

	pairedPins.push_back(pin);
}

void smartGPIO::addToStack(char port, uint16_t fromPin, uint16_t toPin)
{
	alone = false;

	for(int iter = fromPin; iter <= toPin; iter++)
	{
		GPIO pin_;
		setupGPIO(port, iter);
		pairedPins.push_back(pin_);
	}
}

GPIO_TypeDef* smartGPIO::getPort(void)
{
	return port;
}

uint16_t smartGPIO::getPinNumber(void)
{
	return pinNum;
}

std::vector<smartGPIO::GPIO> smartGPIO::getStack(void)
{
	return pairedPins;
}

void smartGPIO::clearStack(void)
{
	pairedPins.clear();
}
