#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

struct
{
    uint8_t *ip;
    uint64_t accumulator;
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

interpretResult vmInterpret(uint8_t *bytecode)
{
    vmReset();

    puts("Start interpreting");
    virtualMachine.ip = bytecode;

    for (;;)
    {
        uint8_t instruction = *virtualMachine.ip++;

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
    uint8_t code[] = {OP_INC, OP_INC, OP_DEC, OP_DONE};
    interpretResult result = vmInterpret(code);
    printf("vm state: %" PRIu64 "\n", virtualMachine.accumulator);

    assert(result == SUCCESS);
    assert(virtualMachine.accumulator == 1);

    uint8_t code2[] = {OP_INC, OP_DEC, OP_DEC, OP_DONE};
    interpretResult result2 = vmInterpret(code2);
    printf("vm state: %" PRIu64 "\n", virtualMachine.accumulator);

    assert(result2 == SUCCESS);
    assert(virtualMachine.accumulator == UINT64_MAX);

    return EXIT_SUCCESS;
}