#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "basicVirtualMachine.h"
#include "stackVirtualMachine.h"
#include "registerVirtualMachine.h"
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

#define ENCODE_OP(op) \
    ((op) << 12)
#define ENCODE_OP_REG(op, reg) \
    (ENCODE_OP(op) | ((reg) << 8))
#define ENCODE_OP_REG_IMM(op, reg, imm) \
    (ENCODE_OP(op) | ((reg) << 8) | (imm))
#define ENCODE_OP_REGS(op, reg0, reg1, reg2) \
    (ENCODE_OP(op) | ((reg0) << 8) | ((reg1) << 4) | (reg2))

    {
        /* Load an immediate value into r4, then MOV it to result  */
        uint16_t code[] = {
            ENCODE_OP_REG_IMM(OP_LOADI, 3, 5),
            ENCODE_OP_REG(OP_MOV_RES, 3),
            ENCODE_OP(OP_DONE)};
        interpretResult result = registerVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", registerVirtualMachine.result);

        assert(result == SUCCESS);
        assert(registerVirtualMachine.result == 5);
    }

    {
        /* Load an immediate values into r3,r2, add the result in r1, then mov to result  */
        uint16_t code[] = {
            ENCODE_OP_REG_IMM(OP_LOADI, 3, 5),
            ENCODE_OP_REG_IMM(OP_LOADI, 2, 10),
            ENCODE_OP_REGS(OP_ADD, 2, 3, 1),

            ENCODE_OP_REG(OP_MOV_RES, 1),
            ENCODE_OP(OP_DONE)};
        interpretResult result = registerVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", registerVirtualMachine.result);

        assert(result == SUCCESS);
        assert(registerVirtualMachine.result == 15);
    }

    {
        /* Load immediate values into r0,r1, subtract r1 from r0 into r2, then mov to result */
        uint16_t code[] = {
            ENCODE_OP_REG_IMM(OP_LOADI, 0, 7),
            ENCODE_OP_REG_IMM(OP_LOADI, 1, 3),
            ENCODE_OP_REGS(OP_SUB, 0, 1, 2),

            ENCODE_OP_REG(OP_MOV_RES, 2),
            ENCODE_OP(OP_DONE)};
        interpretResult result = registerVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", registerVirtualMachine.result);

        assert(result == SUCCESS);
        assert(registerVirtualMachine.result == 4);
    }

    {
        /* Load immediate values into r0,r1, divide r1 by r0 into r2, then mov to result  */
        uint16_t code[] = {
            ENCODE_OP_REG_IMM(OP_LOADI, 0, 6),
            ENCODE_OP_REG_IMM(OP_LOADI, 1, 2),
            ENCODE_OP_REGS(OP_DIV, 0, 1, 2),

            ENCODE_OP_REG(OP_MOV_RES, 2),
            ENCODE_OP(OP_DONE)};
        interpretResult result = registerVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", registerVirtualMachine.result);

        assert(result == SUCCESS);
        assert(registerVirtualMachine.result == 3);
    }

    {
        /* Load immediate values into r0,r1, multiply r1 by r0 into r2, then mov to result  */
        uint16_t code[] = {
            ENCODE_OP_REG_IMM(OP_LOADI, 0, 6),
            ENCODE_OP_REG_IMM(OP_LOADI, 1, 2),
            ENCODE_OP_REGS(OP_MUL, 0, 1, 2),

            ENCODE_OP_REG(OP_MOV_RES, 2),
            ENCODE_OP(OP_DONE)};
        interpretResult result = registerVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", registerVirtualMachine.result);

        assert(result == SUCCESS);
        assert(registerVirtualMachine.result == 12);
    }

    {
        /* Expression: 2*(11+3) */
        uint16_t code[] = {
            ENCODE_OP_REG_IMM(OP_LOADI, 1, 11),
            ENCODE_OP_REG_IMM(OP_LOADI, 2, 3),
            ENCODE_OP_REGS(OP_ADD, 1, 2, 3),

            ENCODE_OP_REG_IMM(OP_LOADI, 2, 2),
            ENCODE_OP_REGS(OP_MUL, 2, 3, 0),

            ENCODE_OP_REG(OP_MOV_RES, 0),
            ENCODE_OP(OP_DONE)};
        interpretResult result = registerVmInterpret(code);
        printf("vm state: %" PRIu64 "\n", registerVirtualMachine.result);

        assert(result == SUCCESS);
        assert(registerVirtualMachine.result == 28);
    }

    return EXIT_SUCCESS;

#undef ENCODE_OP
#undef ENCODE_OP_REG
#undef ENCODE_OP_REG_IMM
#undef ENCODE_OP_REGS
    return EXIT_SUCCESS;
}