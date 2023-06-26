/*
* Calculator.c
*
* Created: 6/18/2023 8:28:01 PM
*  Author: momen mohamed
*/
#include "StdTypes.h"
#include "DIO_Interface.h"
#include "KeyPad_Interface.h"
#include "LCD_Interface.h"
#define F_CPU 8000000
#include "util/delay.h"

typedef enum{
	DIV,
	MUL,
	ADD,
	SUB
}OPERATION_type;


static OPERATION_type operations[3];
static s32 operands[3] ;
static s32 result;
static u8 operandIndex ;
static u8 operationIndex ;
static Bool_t  operandIsLast,isNegative ;
static u8 cursorPosition;
static Bool_t didDivByZero,resultRequested,resultDisplayed,allowDisplay ;





void handleNumberPressed(u8 keyPressed);
void resetCalculator();
void handleOperation(u8 operation);
Bool_t checkNumKeyPressed(u8 keyPressed);

void calculate();

void Calculator_init(void){
	resetCalculator();
}

void Calculator_Runnable(void){
	
	u8 keyPressed;
	keyPressed = KEYPAD_GetKey();
	if (keyPressed != 'T')
	{
		switch(keyPressed){
			case  '0'...'9':
			handleNumberPressed(keyPressed);
			break;
			case '=':
			calculate();
			break;
			case 'C':
			resetCalculator();
			allowDisplay = FLASE;
			default:
			handleOperation(keyPressed);
			break;
		}
		if (allowDisplay)
		{
			LCD_WriteChar(keyPressed);
			cursorPosition++;
		}
		else{
			allowDisplay = TRUE;
		}
		
		if (resultRequested)
		{
			LCD_GoTo(1,0);
			LCD_WriteNumber(result);
			cursorPosition = 0;
			resultRequested = FLASE;
			resultDisplayed = TRUE;
		}
	}
	
	
}



void handleNumberPressed(u8 keyPressed){
	s32 concatenationResult;
	concatenationResult  = (operands[operandIndex] * 10) + (keyPressed - '0')   ;
	if (isNegative)
	{
		concatenationResult = -concatenationResult;
		isNegative = FLASE;
	}
	if (concatenationResult < (s16)32767 && concatenationResult > (s16)(-32767) )
	{
		operands[operandIndex] = concatenationResult;
		operandIsLast = TRUE;
	}else
	{
		allowDisplay = FLASE;
	}
}


void calculate(){
	//u8 i ;
	//for (i =0 ; i < operandIndex ; i++)
	//{
	//switch(operations[i])
	//}
	result = 100;
	resultRequested = TRUE;
}


void handleOperation(u8 operation){
	switch(operation){
		case  '/':
		if (operandIsLast)
		{
			operations[operationIndex++] = DIV;
			operandIndex ++;
			operandIsLast = FLASE;
		}
		break;
		case '*':
		if (operandIsLast)
		{
			operations[operationIndex++] = MUL;
			operandIndex ++;
			operandIsLast = FLASE;
		}
		break;
		case '+':
		if (operandIsLast)
		{
			operations[operationIndex++] = ADD;
			operandIndex ++;
			operandIsLast = FLASE;
		}
		break;
		case '-':
		if (operandIsLast)
		{
			operations[operationIndex++] = SUB;
			operandIndex ++;
			operandIsLast = FLASE;
			}else{
			isNegative = TRUE;
		}
		break;
	}
}

void resetCalculator(){
	u8 i;
	LCD_Clear();
	operandIndex  = 0;
	operationIndex = 0;
	operandIsLast = FLASE;
	isNegative = FLASE ;
	cursorPosition = 0;
	result = 0;
	didDivByZero = FLASE;
	resultRequested = FLASE;
	resultDisplayed = FLASE;
	allowDisplay = TRUE;
	for (i = 0 ; i < 3;i++)
	{
		operands[i] = 0;
		operations[i] = 0;
	}
}

