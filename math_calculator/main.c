

#include "StdTypes.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "KeyPad_Interface.h"
#include "calculator.h"

int main(void)
{
	DIO_init();
	LCD_Init();
	KEYPAD_Init();
	Calculator_init();
    /* Replace with your application code */
    while (1) 
    {
		Calculator_Runnable();
    }
}

