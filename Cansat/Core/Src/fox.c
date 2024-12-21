#include "fox.h"
#include "usbd_cdc_if.h"

#include <string.h>


//#####################################################################################################
// check if VCOM is opened

uint8_t usbIsConnected()
{
	return VCP_OPEN;
}

//#####################################################################################################
// read USB

FIFO RX_FIFO = {.head=0, .tail=0};
uint8_t usbReadLine(char rxBuffer[], uint32_t Len)
{
	uint32_t count=0;
	/* Check inputs */
	memset((uint8_t*)rxBuffer, 0, Len);
	while (Len--)
	{
		if (RX_FIFO.head==RX_FIFO.tail) return count;
		count++;
		*rxBuffer++=RX_FIFO.data[RX_FIFO.tail];
		RX_FIFO.tail=FIFO_INCR(RX_FIFO.tail);
	}
	return count;
}

//#####################################################################################################
// send text over USB

void usbSendString(char txBuffer[])
{
	while(CDC_Transmit_FS((uint8_t*)txBuffer, strlen(txBuffer)) == USBD_BUSY && usbIsConnected());
}

void usbSendStringln(char txBuffer[])
{
	usbSendString(txBuffer);
	usbSendString("\n\r");
}

//#####################################################################################################
// send values over USB

void usbSendBin(uint8_t byte)
{
	char txBuffer[10];
	memset(txBuffer, 0, 10);
	for(uint8_t i=0; i<8; i++)
		txBuffer[i] = ((byte & (1<<(7 - i))) >= 1) ? '1' : '0';
	usbSendString(txBuffer);
}

void usbSendBinln(uint8_t byte)
{
	char txBuffer[10];
	memset(txBuffer, 0, 10);
	for(uint8_t i=0; i<8; i++)
		txBuffer[i] = ((byte & (1<<(7 - i))) >= 1) ? '1' : '0';
	usbSendStringln(txBuffer);
}

void usbSendInt(int value)
{
	char txBuffer[16];
	sprintf(txBuffer, "%d", value);
	usbSendString(txBuffer);
}

void usbSendIntln(int value)
{
	char txBuffer[16];
	sprintf(txBuffer, "%i\n\r", value);
	usbSendString(txBuffer);
}

void usbSendFloat(float value)
{
	char txBuffer[16];
	sprintf(txBuffer, "%f", value);
	usbSendString(txBuffer);
}

void usbSendFloatln(float value)
{
	char txBuffer[16];
	sprintf(txBuffer, "%f\n\r", value);
	usbSendString(txBuffer);
}

//#####################################################################################################
// set led GPIO

void ledWrite(uint16_t pin, PinState state)
{
	switch(state)
	{
	case LOW:
		HAL_GPIO_WritePin(GPIOC, pin, GPIO_PIN_RESET);
		break;
	case HIGH:
		HAL_GPIO_WritePin(GPIOC, pin, GPIO_PIN_SET);
		break;
	case TOGGLE:
		HAL_GPIO_TogglePin(GPIOC, pin);
		break;
	}
}

//#####################################################################################################
// GPIO functions

//void gpioWrite(uint16_t pin, PinState state)
//{
//	GPIO_TypeDef* GPIOx;
//	if(pin < PB4) GPIOx = GPIOA;
//	else GPIOx = GPIOB;
//
//	switch(state)
//	{
//	case LOW:
//		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
//		break;
//	case HIGH:
//		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
//		break;
//	case TOGGLE:
//		HAL_GPIO_TogglePin(GPIOx, pin);
//		break;
//	}
//}
//
//uint8_t gpioRead(uint16_t pin)
//{
//	GPIO_TypeDef* GPIOx;
//	if(pin < PB4) GPIOx = GPIOA;
//	else GPIOx = GPIOB;
//	return GPIOx->IDR & pin;
//}

uint16_t analogRead(uint16_t pin)
{
	return (uint16_t)0;
}

//#####################################################################################################
// nRF24L01































//#####################################################################################################
// end

