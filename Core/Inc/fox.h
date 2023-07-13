#ifndef FOX
#define FOX
#include "stm32f4xx_hal.h"

// pin state type
typedef enum
{
	LOW,
	HIGH,
	TOGGLE,
} PinState;

// check if the VCOM is opened
uint8_t usbIsConnected();

// read from USB
uint8_t usbReadLine(char rxBuffer[], uint32_t Len);

// send text over USB
void usbSendString(char txBuffer[]);
void usbSendStringln(char txBuffer[]);

// send values over USB
void usbSendBin(uint8_t byte);
void usbSendBinln(uint8_t byte);
void usbSendFloat(float value);
void usbSendFloatln(float value);
void usbSendInt(int value);
void usbSendIntln(int value);

void ledWrite(uint16_t pin, PinState state);

#endif
