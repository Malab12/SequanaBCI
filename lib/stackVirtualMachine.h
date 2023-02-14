#ifndef STACK_VM_H
#define STACK_VM_H

#include "globalEnums.h"

#define STACK_MAX 256
struct
{
    // Information pointer
    uint8_t *ip;
    // Fixed-size stack
    uint64_t stack[STACK_MAX];
    uint64_t *stackTop;
    //
    uint64_t result;
} stackVirtualMachine;

void stackVmReset(void)
{
    puts("Resetting stack virtual machine state");
    stackVirtualMachine = (typeof(stackVirtualMachine)){NULL};
    stackVirtualMachine.stackTop = stackVirtualMachine.stack;
}

void stackVmPush(uint64_t value)
{
    *stackVirtualMachine.stackTop = value;
    stackVirtualMachine.stackTop++;
}

uint64_t stackVmPop(void)
{
    stackVirtualMachine.stackTop--;
    return *stackVirtualMachine.stackTop;
}

interpretResult stackVmInterpret(uint8_t *bytecode)
{
    stackVmReset();

    puts("Starting interpreting for stack based VM");
    stackVirtualMachine.ip = bytecode;

    for (;;)
    {
        uint8_t instruction = *stackVirtualMachine.ip++;
        switch (instruction)
        {
        case OP_PUSHI:
        {
            // Get the argument and push it onto the stack
            uint8_t argument = *stackVirtualMachine.ip++;
            stackVmPush(argument);
        }
        break;
        case OP_ADD:
        {
            uint64_t argumentRight = stackVmPop();
            uint64_t argumentLeft = stackVmPop();
            uint64_t result = argumentLeft + argumentRight;
            stackVmPush(result);
        }
        break;
        case OP_SUB:
        {
            uint64_t argumentRight = stackVmPop();
            uint64_t argumentLeft = stackVmPop();
            uint64_t result = argumentLeft - argumentRight;
            stackVmPush(result);
        }
        break;
        case OP_MUL:
        {
            uint64_t argumentRight = stackVmPop();
            uint64_t argumentLeft = stackVmPop();
            uint64_t result = argumentLeft * argumentRight;
            stackVmPush(result);
        }
        break;
        case OP_DIV:
        {
            uint64_t argumentRight = stackVmPop();
            if (argumentRight == 0)
                return ERROR_DIVISION_BY_ZERO;
            uint64_t argumentLeft = stackVmPop();
            uint64_t result = argumentLeft / argumentRight;
            stackVmPush(result);
        }
        break;
        case OP_POP_RES:
        {
            uint64_t result = stackVmPop();
            stackVirtualMachine.result = result;
        }
        break;
        case OP_DONE:
        {
            return SUCCESS;
        }
        default:
            return ERROR_UNKNOWN_OPCODE;
        }
    }
    return SUCCESS;
}

#endif