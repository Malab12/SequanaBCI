#include <stdio.h>

struct
{
    __uint8_t *ip;
    __uint64_t accumulator;
} virtualMachine;

typedef enum
{
    // Increment the register
    OP_INC,
    // Decrement the register
    OP_DEC,
    // Stop execution
    OP_DONE
} opcode;

typedef enum
{
    SUCCESS,
    ERROR_UNKNOWN_OPCODE
} interpretResult;

void vmReset(void)
{
    puts("Resetting the VM state\n");
    virtualMachine = (typeof(virtualMachine)){NULL};
}

interpretResult vmInterpret(__uint8_t *bytecode)
{
    vmReset();

    puts("Start interpreting");
    virtualMachine.ip = bytecode;

    for (;;)
    {
        __uint8_t instruction = *virtualMachine.ip++;

        switch (instruction)
        {
        case OP_INC:
        {
            virtualMachine.accumulator++;
        }
        break;
        case OP_DEC:
        {
            virtualMachine.accumulator--;
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

int main()
{
    printf("Hello World\n");
}