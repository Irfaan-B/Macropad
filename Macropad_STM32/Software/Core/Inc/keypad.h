#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f1xx.h"

char Keypad_Scan(void);
void Keypad_SendKeyViaUSB(char key);

#endif