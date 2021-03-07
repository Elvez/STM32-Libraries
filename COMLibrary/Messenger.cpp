/*
 * Messenger.cpp
 *
 *  Created on: 28-Jan-2021
 *      Author: elvez
 */

#include "ComLibrary/Messenger.h"



Messenger::Messenger() {
	// TODO Auto-generated constructor stub

}

Messenger::~Messenger() {
	// TODO Auto-generated destructor stub
}

void Messenger::uartSend(std::string message)
{
#ifdef UART_ENABLED
	uartMessage packet;

	packet.length = message.size();
	packet.message = &message[0];
	packet.uart = &huart1;

	HAL_UART_Transmit(packet.uart, (uint8_t*)packet.message, packet.length, HAL_MAX_DELAY);
#endif
}

void Messenger::uartSend(char* packet_)
{
#ifdef UART_ENABLED
	uartMessage packet;

	packet.length = strlen(packet_);
	packet.message = packet_;
	packet.uart = &huart1;

	HAL_UART_Transmit(packet.uart, (uint8_t*)packet.message, packet.length, HAL_MAX_DELAY);
#endif
}

void Messenger::usbSend(std::string message)
{
	usbMessage packet;

	packet.length = message.size();
	packet.message = &message[0];
	uint8_t* txPacket = (uint8_t*)packet.message;

	CDC_Transmit_FS(txPacket, packet.length);
}

void Messenger::usbSend(char* packet_)
{
	usbMessage packet;

	packet.length = strlen(packet_);
	packet.message = packet_;
	uint8_t* txPacket = (uint8_t*)packet.message;

	CDC_Transmit_FS(txPacket, packet.length);
}

void Messenger::sendMessage(sender thisSender, std::string message)
{
#ifndef UART_ENABLED
#ifdef USB_ENABLED
	thisSender = USB;
#endif
#endif

	switch(thisSender)
	{
	case USB:
		usbSend(message);
		break;
	case UART:
		uartSend(message);
		break;
	default:
		break;
	}
}

#ifdef I2C_ENABLED
void Messenger::i2cTransmit(i2cMessage packet)
{
	HAL_I2C_Master_Transmit(packet.i2c, packet.address, packet.pData, packet.length, packet.timeout);
}
#endif

void Messenger::sendMessage(sender thisSender, char* format, ...)
{
#ifndef UART_ENABLED
#ifdef USB_ENABLED
	thisSender = USB;
#endif
#endif

	char message[100];
	va_list args;
	va_start(args, format);
	vsprintf(message, format, args);

	switch(thisSender)
	{
	case USB:
		usbSend(message);
		break;
	case UART:
		uartSend(message);
		break;
	default:
		break;
	}
	va_end(args);
}

void Messenger::debugMessage(std::string message)
{
	debugSend(message);
}

void Messenger::debugMessage(char* format, ...)
{
	char message[100];
	va_list args;
	va_start(args, format);
	vsprintf(message, format, args);

	debugSend(message);

	va_end(args);
}

void Messenger::debugSend(std::string message)
{
#ifdef DEBUGGER_ENABLED
	uartMessage packet;

	packet.length = message.size();
	packet.message = &message[0];
	packet.uart = &huart2;

	HAL_UART_Transmit(packet.uart, (uint8_t*)packet.message, packet.length, HAL_MAX_DELAY);
#endif
}

void Messenger::debugSend(char* packet_)
{
#ifdef DEBUGGER_ENABLED
	uartMessage packet;

	packet.length = strlen(packet_);
	packet.message = packet_;
	packet.uart = &huart2;

	HAL_UART_Transmit(packet.uart, (uint8_t*)packet.message, packet.length, HAL_MAX_DELAY);
#endif
}

