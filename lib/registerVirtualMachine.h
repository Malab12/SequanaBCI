#ifndef REG_VM_H
#define REG_VM_H

#include "globalEnums.h"

#define REGISTER_NUM 15

struct
{
    uint16_t *ip;
    // Register array
    uint64_t registers[REGISTER_NUM];
    // Single registers containing the result
    uint64_t result;
} registerVirtualMachine;

void registerVmReset(void)
{
    puts("Reset registerVM state");
    registerVirtualMachine = (typeof(registerVirtualMachine)){NULL};
}

void decode(uint16_t instruction,
            uint8_t *opCode,
            uint8_t *register0, uint8_t *register1, uint8_t *register2,
            uint8_t *imm)
{
    *opCode = (instruction & 0xF000) >> 12;
    *register0 = (instruction & 0x0F00) >> 8;
    *register1 = (instruction & 0x00F0) >> 4;
    *register2 = (instruction & 0x000F);
    *imm = (instruction & 0x00FF);
}

interpretResult registerVmInterpret(uint16_t *bytecode)
{
    registerVmReset();
    puts("Start interpreting for the registerVirtualMachine");
    registerVirtualMachine.ip = bytecode;

    uint8_t operation, r0, r1, r2, immediate;
    for (;;)
    {
        uint16_t instruction = *registerVirtualMachine.ip++;
        decode(instruction, &operation, &r0, &r1, &r2, &immediate);

        switch (operation)
        {
        case OP_LOADI:
        {
            registerVirtualMachine.registers[r0] = immediate;
        }
        break;

        case OP_ADD:
        {
            registerVirtualMachine.registers[r2] = registerVirtualMachine.registers[r0] + registerVirtualMachine.registers[r1];
        }
        break;

        case OP_SUB:
        {
            registerVirtualMachine.registers[r2] = registerVirtualMachine.registers[r0] - registerVirtualMachine.registers[r1];
        }
        break;

        case OP_MUL:
        {
            registerVirtualMachine.registers[r2] = registerVirtualMachine.registers[r0] * registerVirtualMachine.registers[r1];
        }
        break;

        case OP_DIV:
        {
            if (registerVirtualMachine.registers[r1] == 0)
                return ERROR_DIVISION_BY_ZERO;
            registerVirtualMachine.registers[r2] = registerVirtualMachine.registers[r0] / registerVirtualMachine.registers[r1];
        }
        break;

        case OP_MOV_RES:
        {
            registerVirtualMachine.result = registerVirtualMachine.registers[r0];
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