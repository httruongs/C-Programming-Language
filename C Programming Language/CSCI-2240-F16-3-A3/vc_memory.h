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


/* Function Name:  Print the statement
 * Parameters: use the name of memory to access into the memory members
 * Return: void
 * Description: Create the memory structure and printout the statement
 */

#ifndef VC_MEMORY_H
#define VC_MEMORY_H

typedef struct memory {
    int memoryCells[100];
    int accumulator;
    int instructionCounter;
    int instructionRegister;
    int operationCode;
    int operand;
} Memory;

void printState(Memory *memory)
{
    printf("REGISTERS:\n");
    printf("%-24s%+05d\n", "accumulator", memory->accumulator);
    printf("%-24s%5.2d\n", "instructionCounter", memory->instructionCounter);
    printf("%-24s%+05d\n", "instructionRegister", memory->instructionRegister);
    printf("%-24s%5.2d\n", "operationCode", memory->operationCode);
    printf("%-24s%5.2d\n", "operand", memory->operand);
    printf("MEMORY:\n");
    printf("  ");
    for(memory->accumulator = 0; memory->accumulator < 10; memory->accumulator++) {
        printf("%6c", memory->accumulator + 48);
    }
    printf("\n");
    for(memory->accumulator = 0; memory->accumulator < 10; memory->accumulator++) {
        printf("%2d", memory->accumulator * 10);
        for(memory->instructionCounter = 0; memory->instructionCounter < 10; memory->instructionCounter++) {
            printf(" %+05d", memory->memoryCells[10 * memory->accumulator + memory->instructionCounter]);
        }
        printf("\n");
    }
}

#endif
