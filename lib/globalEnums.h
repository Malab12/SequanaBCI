#ifndef GLOB_ENUM_H
#define GLOB_ENUM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

typedef enum
{
    // Increment the register
    OP_INC,
    // Decrement the register
    OP_DEC,
    // Add the immediate argument to the register
    OP_ADDI,
    // Subtract the immediate argument from the register
    OP_SUBI,
    // push the immediate argument onto the stack
    OP_PUSHI,
    // load an immediate value into r0
    OP_LOADI,
    // Stack: pop two values from the stack, add and push the result
    // Register: add values in r0, r1 registers and put them in r2
    OP_ADD,
    // Stack: pop two values from the stack, subtract and push the result
    // Register: subtract values in r0, r1 registers and put them into r2
    OP_SUB,
    // Stack: pop two values from the stack, multiply and push the result
    // Register: multiply values in ro, r1 registers and put them into r2
    OP_MUL,
    // Stack: pop two values from the stack, divide and push the result
    // Register: divide values in r0, r1 registers and put them into r2
    OP_DIV,
    // pop the top of the stack and set it as execution result
    OP_POP_RES,
    // Move a value from r0 register into the result register
    OP_MOV_RES,
    // stop execution
    OP_DONE
} opCodes;

typedef enum
{
    SUCCESS,
    ERROR_DIVISION_BY_ZERO,
    ERROR_UNKNOWN_OPCODE
} interpretResult;

#endif