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

/* Function Name: instruction2int
 * Parameters: array of characters named instruction
 * Return: -1  or appropriated integers for instructions
 * Description: Setting up the instructions. If there is not matched with defined instructions 
 * and return integers of appropriated instructions if it is defined.
 */


#ifndef VC_COMPILATION_H
#define VC_COMPILATION_H

#include <ctype.h>
#include <malloc.h>
#include "vc_memory.h"

int instruction2int(char *instruction)
{
    if(!strcmp(instruction, "READ")) {
        return 10;
    }
    if(!strcmp(instruction, "WRIT")) {
        return 11;
    }
    if(!strcmp(instruction, "PRNT")) {
        return 12;
    }
    if(!strcmp(instruction, "LOAD")) {
        return 20;
    }
    if(!strcmp(instruction, "STOR")) {
        return 21;
    }
    if(!strcmp(instruction, "SET")) {
        return 22;
    }
    if(!strcmp(instruction, "ADD")) {
        return 30;
    }
    if(!strcmp(instruction, "SUB")) {
        return 31;
    }
    if(!strcmp(instruction, "DIV")) {
        return 32;
    }
    if(!strcmp(instruction, "MULT")) {
        return 33;
    }
    if(!strcmp(instruction, "MOD")) {
        return 34;
    }
    if(!strcmp(instruction, "BRAN")) {
        return 40;
    }
    if(!strcmp(instruction, "BRNG")) {
        return 41;
    }
    if(!strcmp(instruction, "BRZR")) {
        return 42;
    }
    if(!strcmp(instruction, "HALT")) {
        return 99;
    }
    return -1;
}

/* Function Name: parseNum
 * Parameters: array of character  str and int pointer result
 * Return: void
 * Description: parse a string to integer
 */


void parseNum(char *str, int *result)
{
    *result = 0;
    while(*str != '\0' && *str != '\n') {
        if(!isdigit(*str)) {
            *result = -1;
            break;
        }
        *result = 10 * (*result) + (*str - 48);
        str++;
    }
}

/* Function Name: parseOperand
 * Parameters: array of character str, integer pointer instructionCode, and integer pointer operand
 * Return: void
 * Description:  parse a string to operand
 */


void parseOperand(char *str, int *instructionCode, int *operand)
{
    if(*str == '-') {
        if(*instructionCode == 22) {
            parseNum(++str, operand);
            *operand *= -1;
        }
        else {
            *operand = 10000;
        }
    }
    else {
        parseNum(str, operand);
    }
}

/* Function Name: isWhitespace
 * Parameters: array of character str
 * Return: int
 * Description: check if the chars are a tab or linebreak
 */


int isWhitespace(char *str)
{
    return *str == '\t' || *str == '\n';
}

/* Function Name: compile
 * Parameters: Memory *memory
 * Return: void
 * Description: compile into memory
 */


void compile(Memory *memory)
{
    char *buffer = malloc(80 * sizeof(char));
    char *tok;

    memory->instructionCounter = 0;  /* mem address */
    memory->instructionRegister = 0; /* error processing - value of 1 on function exit means error */
    while(fgets(buffer, 80, stdin) && !memory->instructionRegister) {
        /* Accumulator serves as parsing loop controller */
        memory->accumulator = 0;
        for(tok = strtok(buffer, " "); tok != NULL; tok = strtok(NULL, " ")) {
            if(memory->accumulator == 0) {
                /* Save accumulator state in operationCode temporarily */
                memory->operationCode = memory->accumulator;
                parseNum(tok, &memory->accumulator);
                if(memory->accumulator != memory->instructionCounter) {
                    printf("Compilation error! Undefined use at address %d\n", memory->instructionCounter);
                    memory->instructionRegister = 1;
                    memory->accumulator = memory->operationCode;
                    break;
                }
                memory->accumulator = memory->operationCode;
            }
            if(memory->accumulator == 1) {
                memory->operationCode = instruction2int(tok);
                if(memory->operationCode == -1) {
                    printf("Compilation error! Unknown command at address %d\n", memory->instructionCounter);
                    memory->instructionRegister = 1;
                    break;
                }
            }
            if(memory->accumulator == 2) {
                parseOperand(tok, &memory->operationCode, &memory->operand);
                if(memory->operand == 10000) {
                    printf("Compilation error! Undefined use at address %d\n", memory->instructionCounter);
                    memory->instructionRegister = 1;
                    break;
                }
                if(memory->operand > 9999 || memory->operand < -9999) {
                    printf("Compilation error! Word overflow at address %d\n", memory->instructionCounter);
                    memory->instructionRegister = 1;
                    break;
                }
            }
            if(memory->accumulator > 2 && !isWhitespace(tok)) {
                printf("Compilation error! Undefined use at address %d\n", memory->instructionCounter);
                memory->instructionRegister = 1;
                break;
            }
            memory->accumulator++;
        }
        if(memory->accumulator < 2 && !memory->instructionRegister) {
            printf("Compilation error! Undefined use at address %d\n", memory->instructionCounter);
            memory->instructionCounter = 1;
            break;
        }
        if(memory->operationCode == 22) {
            memory->memoryCells[memory->instructionCounter] = memory->operand;
        }
        else {
            memory->memoryCells[memory->instructionCounter] = 100 * memory->operationCode + memory->operand;
        }
        memory->instructionCounter++;
    }
    if(!memory->instructionRegister) {
        memory->accumulator = 0;
        for(memory->instructionCounter = 0; memory->instructionCounter < 100; memory->instructionCounter++) {
            if(memory->memoryCells[memory->instructionCounter] == 9999) {
                memory->accumulator = 1;
            }
        }
        if(!memory->accumulator) {
            printf("Compilation error! No halt command\n");
            memory->instructionRegister = 1;
        }
    }
    free(buffer);
}

#endif







