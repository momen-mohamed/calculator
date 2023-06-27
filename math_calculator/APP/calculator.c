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
#include "calculator_private.h"
#define F_CPU 8000000
#include "util/delay.h"




static OPERATION_type operations[OPERATIONS_NUM]; //operations array.
static s32 operands[OPERANDS_NUM] ; //operands array to store all operands.
static s32 result; //storing variable
static u8 operandIndex ;
static u8 operationIndex ;
static Bool_t  operandIsLast; //this flag is raised if last input was operand.
static Bool_t isNegative ; // this flag is raised when minus sign is entered.
static Bool_t didDivByZero; // this flag is raised when user divide by zero.
static Bool_t resultRequested; //this flag is raised when = is pressed.
static Bool_t resultDisplayed; //this flag is raised when result is displayed.
static Bool_t allowDisplay ; //this flag is raised to allow displaying on LCD.




//initialization function.
void Calculator_init(void){
	resetCalculator();
}


//runnable function.
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
		}
		else{
			allowDisplay = TRUE;
		}
		if (resultRequested)
		{
			LCD_GoTo(1,0);
			if (!operandIsLast)
			{
				LCD_WriteString("MISSING OPERAND");
			}else if (didDivByZero)
			{
				LCD_WriteString("DIVIDE BY ZERO");
			}
			else{
				LCD_WriteNumber(result);
			}
			resultRequested = FLASE;
			resultDisplayed = TRUE;
		}
	}
}



void handleNumberPressed(u8 keyPressed){
	s32 concatenationResult;
	
	if (resultDisplayed)
	{
		resetCalculator();
	}
	
	concatenationResult  = (operands[operandIndex] * 10) + (keyPressed - '0')   ;
	if (isNegative)
	{
		concatenationResult = -concatenationResult;
		isNegative = FLASE;
	}
	if (concatenationResult <= MAX_S16 && concatenationResult >= MIN_S16 )
	{
		operands[operandIndex] = concatenationResult;
		operandIsLast = TRUE;
	}else
	{
		playBuzz();
		allowDisplay = FLASE;
	}
}

void handleOperation(u8 operation){
	
	Bool_t handleOperation = TRUE;
	
	//this condition handle the pressing of operation key after displaying result.
	
	if (resultDisplayed && (didDivByZero || !operandIsLast ))
	{
		handleOperation = FLASE;
	}else if (resultDisplayed )
	{
		resetCalculator();
		LCD_WriteString("ANS");
		operands[0] = result;
		operandIsLast = TRUE;
	}
	
	if (operationIndex < 1 && handleOperation)
	{
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
			}
			else{
				isNegative = TRUE;
			}
			break;
		}
	}else if (operation == '-' && handleOperation )
	{
		isNegative = TRUE;
	}
	else{
		
		playBuzz();
		allowDisplay = FLASE;
	}
	
}

void calculate(){
	//this condition handle the pressing of numbers key after displaying result.
	if (resultDisplayed)
	{
		allowDisplay = FLASE;
		playBuzz();
		return;
	}
	u8 i ;
	result = operands[0];
	for (i = 0 ; i < operationIndex ; i++)
	{
		switch(operations[i]){
			case DIV:
			if (operands[i+1] == 0)
			{
				didDivByZero = TRUE;
			}
			else{
				result /= operands[i+1] ;
			}
			break;
			case MUL:
			result *= operands[i+1] ;
			break;
			case ADD:
			result += operands[i+1] ;
			break;
			case SUB:
			result -= operands[i+1] ;
			break;
		}
	}
	resultRequested = TRUE;
}



//this function reset the calculator.
void resetCalculator(){
	u8 i;
	LCD_Clear();
	operandIndex  = 0;
	operationIndex = 0;
	operandIsLast = FLASE;
	isNegative = FLASE ;
	didDivByZero = FLASE;
	resultRequested = FLASE;
	resultDisplayed = FLASE;
	allowDisplay = TRUE;
	
	//these for loop reset operands and operations array ,it facilitate extending calculator and adding more operands and operations.
	for (i = 0 ; i < OPERATIONS_NUM;i++)
	{
		operations[i] = 0;
	}
	for (i=0 ; i < OPERANDS_NUM;i++)
	{
		operands[i] = 0;
	}
}


//this function trigger the buzzer
void playBuzz(){
	DIO_WritePin(PINC5,HIGH);
	_delay_ms(40);
	DIO_WritePin(PINC5,LOW);
}