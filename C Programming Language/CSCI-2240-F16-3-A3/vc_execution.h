/* Name : Huong Truong
 * Class : CSCI 2240-003
 * Program # : 3
 * Due Date : Oct 24 2016
 *
 * Honor Pledge: On my honor as a student of the University
 * of Nebraska at Omaha, I have neither given nor received
 * unauthorized help on this homework assignment.
 *
 * NAME: Huong Truong
 * EMAIL: httruong@unomaha.edu
 * NUID: 52233854
 * Colleagues: None
 *
 */

/* Function Name: Halt Execution
 * Parameters: Memory *memory
 * Return: void
 * Description: halt execution
 */


#ifndef VC_EXECUTION_H
#define VC_EXECUTION_H

#include <stdio.h>
#include "vc_memory.h"

void halt(Memory *memory)
{
    printState(memory);
    memory->operationCode = 99;
}

/* Function Name: isValidChar
 * Parameters: array of integer named character
 * Return: int
 * Description: validate the inputs are valid and in the range of characters.
 */


int isValidChar(int *character)
{
    return *character == 0 || *character == 10 || (*character >= 65 && *character <= 90);
}

/* Function Name: isValidAddress
 * Parameters: array of integer named word
 * Return: int
 * Description: check to see if a word is a valid address
 */


int isValidAddress(int *word)
{
    return *word >= 0 && *word < 100;
}

/* Function Name: overflowPresent
 * Parameters: array of integer named memoryValue
 * Return: int
 * Description: check if there is an overflow
 */


int overflowPresent(int *memoryValue)
{
    return *memoryValue > 9999 || *memoryValue < -9999;
}

/* Function Name: runCurrent
 * Parameters: Memory *memory
 * Return: void
 * Description: simulate assembly commands
 */


void runCurrent(Memory *memory)
{
    switch(memory->operationCode) {
        case 10:
            /* READ */
            scanf("%d", &memory->memoryCells[memory->operand]);
            if(overflowPresent(&memory->memoryCells[memory->operand])) {
                printf("Runtime error! Word overflow at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            break;
        case 11:
            /* WRITE */
            printf("%d", memory->memoryCells[memory->operand]);
            break;
        case 12:
            /* PRINT */
            while(1) {
                if(!isValidAddress(&memory->operand)) {
                    printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                    halt(memory);
                    break;
                }
                if(((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) != 0    &&
                   ((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) != 10   &&
                   ( ((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) < 65  ||
                     ((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) > 90 )){
                    printf("Runtime error! Unkown character at address %d\n", memory->instructionCounter);
                    halt(memory);
                    break;
                }
                else {
                    if(((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100)  == 0) {
                        break;
                    }
                    putc((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100, stdout);
                }
                if(((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) != 0    &&
                   ((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) != 10   &&
                   ( ((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) < 65  ||
                     ((memory->memoryCells[memory->operand] - (memory->memoryCells[memory->operand] % 100)) / 100) > 90 )){
                    printf("Runtime error! Unknown character at address %d\n", memory->instructionCounter);
                    halt(memory);
                    break;
                }
                else {
                    if(memory->memoryCells[memory->operand] % 100 == 0) {
                        break;
                    }
                    putc(memory->memoryCells[memory->operand] % 100, stdout);
                }
                memory->operand++;
            }
            break;
        case 20:
            /* LOAD */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
                break;
            }
            memory->accumulator = memory->memoryCells[memory->operand];
            break;
        case 21:
            /* STORE */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
                break;
            }
            memory->memoryCells[memory->operand] = memory->accumulator;
            break;
        case 30:
            /* ADD */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else {
                memory->accumulator += memory->memoryCells[memory->operand];
                if(overflowPresent(&memory->accumulator)) {
                    printf("Runtime error! Word overflow at address %d\n", memory->instructionCounter);
                    halt(memory);
                }
            }
            break;
        case 31:
            /* SUB */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else {
                memory->accumulator -= memory->memoryCells[memory->operand];
                if(overflowPresent(&memory->accumulator)) {
                    printf("Runtime error! Word oveflow at address %d\n", memory->instructionCounter);
                    halt(memory);
                }
            }
            break;
        case 32:
            /* DIV */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else {
                if(memory->memoryCells[memory->operand] == 0) {
                    printf("Runtime error! Division by zero at address %d\n", memory->instructionCounter);
                    halt(memory);
                }
                else {
                    memory->accumulator /= memory->memoryCells[memory->operand];
                }
            }
            break;
        case 33:
            /* MULT */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else {
                memory->accumulator *= memory->memoryCells[memory->operand];
                if(overflowPresent(&memory->accumulator)) {
                    printf("Runtime error! Word oveflow at address %d\n", memory->instructionCounter);
                    halt(memory);
                }
            }
            break;
        case 34:
            /* MOD */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else {
                if(memory->memoryCells[memory->operand] == 0) {
                    printf("Runtime error! Division by zero at address %d\n", memory->instructionCounter);
                    halt(memory);
                }
                else {
                    memory->accumulator %= memory->memoryCells[memory->operand];
                }
            }
            break;
        case 40:
            /* BRANCH */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else {
                /* instructionCounter is incremented in the outer loop */
                memory->instructionCounter = memory->operand - 1;
            }
            break;
        case 41:
            /* BRANCH IF NEGATIVE */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else if(memory->accumulator < 0) {
                /* instructionCounter is incremented in the outer loop */
                memory->instructionCounter = memory->operand - 1;
            }
            break;
        case 42:
            /* BRANCH IF ZERO */
            if(!isValidAddress(&memory->operand)) {
                printf("Runtime error! Segmentation fault at address %d\n", memory->instructionCounter);
                halt(memory);
            }
            else if(memory->accumulator == 0) {
                memory->instructionCounter = memory->operand - 1;
            }
            break;
        case 99:
            /* HALT */
            halt(memory);
            break;
        default:
            printf("Runtime error! Unknown command at address %d\n", memory->instructionCounter);
            halt(memory);
            break;
    }
}

/* Function Name: execute
 * Parameters: Memory *memory
 * Return: void
 * Description: execute the program provided instructions from a file
 */

void execute(Memory *memory)
{
    stdin = fopen("/dev/tty", "r"); 
    memory->instructionCounter = 0;
    while(1) {
        memory->instructionRegister = memory->memoryCells[memory->instructionCounter];
        memory->operand = memory->instructionRegister % 100;
        memory->operationCode = (memory->instructionRegister - memory->operand) / 100;
        runCurrent(memory);
        if(memory->operationCode == 99) {
            break;
        }
        memory->instructionCounter++;
    }
}

#endif






