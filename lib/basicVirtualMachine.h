#ifndef BASE_VM_H
#define BASE_VM_H

#include "globalEnums.h"

struct
{
    uint8_t *ip;
    uint64_t accumulator;
} basicVirtualMachine;

void basicVmReset(void)
{
    puts("Resetting the VM state");
    basicVirtualMachine = (typeof(basicVirtualMachine)){NULL};
}

interpretResult basicVmInterpret(uint8_t *bytecode)
{
    basicVmReset();

    puts("Start interpreting");
    basicVirtualMachine.ip = bytecode;

    for (;;)
    {
        uint8_t instruction = *basicVirtualMachine.ip++;

        switch (instruction)
        {
        case OP_INC:
        {
            basicVirtualMachine.accumulator++;
        }
        break;
        case OP_DEC:
        {
            basicVirtualMachine.accumulator--;
        }
        break;
        case OP_ADDI:
        {
            uint8_t argument = *basicVirtualMachine.ip++;
            basicVirtualMachine.accumulator += argument;
        }
        break;
        case OP_SUBI:
        {
            uint8_t argument = *basicVirtualMachine.ip++;
            basicVirtualMachine.accumulator -= argument;
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