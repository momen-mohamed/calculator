/*
* calculator_private.h
*
* Created: 6/27/2023 5:46:33 PM
*  Author: mo2a
*/


#ifndef CALCULATOR_PRIVATE_H_
#define CALCULATOR_PRIVATE_H_

#define OPERATIONS_NUM 1
#define OPERANDS_NUM 2
typedef enum{
	DIV,
	MUL,
	ADD,
	SUB
}OPERATION_type;


void handleNumberPressed(u8 keyPressed);
void resetCalculator();
void handleOperation(u8 operation);
void playBuzz();
void calculate();

#endif /* CALCULATOR_PRIVATE_H_ */