#include "globalEnums.h"

matchResult vmMatchRecursion(uint8_t *bytecode, uint8_t *ip, char *sp)
{
    for (;;)
    {
        uint8_t instruction = *ip++;
        switch (instruction)
        {
        case OP_CHAR:
        {
            char curChar = *sp;
            char argChar = (char)*ip;
            // Failed to match
            if (argChar != curChar)
                return MATCH_FAIL;
            // Advance both current instruction and character pointers
            ip++;
            sp++;
        }
            continue;

        case OP_JUMP:
        {
            // Read the offset and jump to the instruction
            uint8_t offset = *ip;
            ip = bytecode + offset;
        }
            continue;

        case OP_OR:
        {
            // Get both branch offsets
            uint8_t leftOffset = *ip++;
            uint8_t rightOffset = *ip;
            // Check if following the first offset get a match
            uint8_t *leftIp = bytecode + leftOffset;
            if (vmMatchRecursion(bytecode, leftIp, sp) == MATCH_OK)
                return MATCH_OK;
            // No match? check the second branch
            ip = bytecode + rightOffset;
        }
            continue;

        case OP_MATCH:
        {
            return MATCH_OK;
        }
        default:
            return MATCH_ERROR;
        }
        return MATCH_ERROR;
    }
}

matchResult virtualMachineMatch(uint8_t *bytecode, char *str)
{
    printf("Start matching a string: %s\n", str);
    return vmMatchRecursion(bytecode, bytecode, str);
}