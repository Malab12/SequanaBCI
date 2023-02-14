#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "basicVirtualMachine.h"
#include "stackVirtualMachine.h"

int main()
{
    {
        uint8_t code[] = {OP_INC, OP_INC, OP_DEC, OP_DONE};
        interpretResult result = basicVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", basicVirtualMachine.accumulator);

        assert(result == SUCCESS);
        assert(basicVirtualMachine.accumulator == 1);
    }

    {
        uint8_t code[] = {OP_INC, OP_DEC, OP_DEC, OP_DONE};
        interpretResult result = basicVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", basicVirtualMachine.accumulator);

        assert(result == SUCCESS);
        assert(basicVirtualMachine.accumulator == UINT64_MAX);
    }

    {
        /* notice the value after OP_ADDI */
        uint8_t code[] = {OP_ADDI, 10, OP_DEC, OP_DONE};
        interpretResult result = basicVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", basicVirtualMachine.accumulator);

        assert(result == SUCCESS);
        assert(basicVirtualMachine.accumulator == 9);
    }

    {
        /* notice values after OP_ADDI and OP_SUBI  */
        uint8_t code[] = {OP_ADDI, 10, OP_SUBI, 3, OP_DONE};
        interpretResult result = basicVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", basicVirtualMachine.accumulator);

        assert(result == SUCCESS);
        assert(basicVirtualMachine.accumulator == 7);
    }

    {
        /* Push and pop the result */
        uint8_t code[] = {OP_PUSHI, 5, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == SUCCESS);
        assert(stackVirtualMachine.result == 5);
    }

    {
        /* Addition */
        uint8_t code[] = {OP_PUSHI, 10, OP_PUSHI, 5, OP_ADD, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == SUCCESS);
        assert(stackVirtualMachine.result == 15);
    }

    {
        /* Subtraction */
        uint8_t code[] = {OP_PUSHI, 10, OP_PUSHI, 6, OP_SUB, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == SUCCESS);
        assert(stackVirtualMachine.result == 4);
    }

    {
        /* Division */
        uint8_t code[] = {OP_PUSHI, 10, OP_PUSHI, 5, OP_DIV, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == SUCCESS);
        assert(stackVirtualMachine.result == 2);
    }

    {
        /* Division with error */
        uint8_t code[] = {OP_PUSHI, 10, OP_PUSHI, 0, OP_DIV, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == ERROR_DIVISION_BY_ZERO);
    }

    {
        /* Multiplication */
        uint8_t code[] = {OP_PUSHI, 10, OP_PUSHI, 2, OP_MUL, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == SUCCESS);
        assert(stackVirtualMachine.result == 20);
    }

    {
        /* Expression: 2*(11+3) */
        uint8_t code[] = {OP_PUSHI, 2, OP_PUSHI, 11, OP_PUSHI, 3, OP_ADD, OP_MUL, OP_POP_RES, OP_DONE};
        interpretResult result = stackVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", stackVirtualMachine.result);

        assert(result == SUCCESS);
        assert(stackVirtualMachine.result == 28);
    }
    return EXIT_SUCCESS;
}