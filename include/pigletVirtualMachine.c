#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>

#include "pigletVirtualMachine.h"

#define MAX_TRACE_LEN 16
#define STACK_MAX 256
#define MEMORY_SIZE 65536

#define NEXT_OP() (*vm.ip++)
#define NEXT_ARG() ((void)(vm.ip += 2), (vm.ip[-2] << 8) + vm.ip[-1])
#define PEEK_ARG() ((vm.ip[0] << 8) + vm.ip[1])
#define POP() (*(--vm.stackTop))
#define PUSH(val) (*vm.stackTop = (val), vm.stackTop++)
#define PEEK() (*(vm.stackTop - 1))
#define TOS_PTR() (vm.stackTop - 1)

static struct
{
    // Current instruction pointer
    uint8_t *ip;
    // Fixed-size stack
    uint64_t stack[STACK_MAX];
    uint64_t *stackTop;
    // Operational memory
    uint64_t memory[MEMORY_SIZE];
    // A single register containing the result
    uint64_t result;
} vm;

static void vmReset(uint8_t *bytecode)
{
    vm = (typeof(vm)){
        .stackTop = vm.stack,
        .ip = bytecode};
}

interpretResult vmInterpret(uint8_t *bytecode)
{
    vmReset(bytecode);

    for (;;)
    {
        uint8_t instruction = NEXT_OP();
        switch (instruction)
        {
        case OP_PUSHI:
        {
            // Get the argument, push it onto the stack
            uint16_t arg = NEXT_ARG();
            PUSH(arg);
        }
        break;

        case OP_LOADI:
        {
            uint16_t address = NEXT_ARG();
            uint64_t value = vm.memory[address];
            PUSH(value);
        }
        break;

        case OP_LOADADDI:
        {
            uint16_t address = NEXT_ARG();
            uint64_t value = vm.memory[address];
            *TOS_PTR() += value;
        }
        break;

        case OP_STOREI:
        {
            uint16_t address = NEXT_ARG();
            uint64_t value = POP();
            vm.memory[address] = value;
        }
        break;

        case OP_LOAD:
        {
            uint64_t address = POP();
            uint64_t value = vm.memory[address];
            PUSH(value);
        }
        break;

        case OP_STORE:
        {
            uint64_t value = POP();
            uint16_t address = POP();
            vm.memory[address] = value;
        }
        break;

        case OP_DUP:
        {
            PUSH(PEEK());
        }
        break;

        case OP_DISCARD:
        {
            (void)POP();
        }
        break;

        case OP_ADD:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() += argumentRight;
        }
        break;

        case OP_ADDI:
        {
            uint16_t argumentRight = NEXT_ARG();
            *TOS_PTR() += argumentRight;
        }
        break;

        case OP_SUB:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() -= argumentRight;
        }
        break;

        case OP_DIV:
        {
            uint64_t argumentRight = POP();
            if (argumentRight == 0)
                return ERROR_DIVISION_BY_ZERO;
            *TOS_PTR() /= argumentRight;
        }
        break;

        case OP_MUL:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() *= argumentRight;
        }
        break;

        case OP_JUMP:
        {
            uint16_t target = PEEK_ARG();
            vm.ip = bytecode + target;
        }
        break;

        case OP_JUMP_IF_TRUE:
        {
            uint16_t target = NEXT_ARG();
            if (POP())
                vm.ip = bytecode + target;
        }
        break;

        case OP_JUMP_IF_FALSE:
        {
            uint16_t target = NEXT_ARG();
            if (!POP())
                vm.ip = bytecode + target;
        }
        break;

        case OP_EQUAL:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() = PEEK() == argumentRight;
        }
        break;

        case OP_LESS:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() = PEEK() < argumentRight;
        }
        break;

        case OP_LESS_OR_EQUAL:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() = PEEK() <= argumentRight;
        }
        break;

        case OP_GREATER:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() = PEEK() > argumentRight;
        }
        break;

        case OP_GREATER_OR_EQUAL:
        {
            uint64_t argumentRight = POP();
            *TOS_PTR() = PEEK() >= argumentRight;
        }
        break;

        case OP_GREATER_OR_EQUALI:
        {
            uint64_t argumentRight = NEXT_ARG();
            *TOS_PTR() = PEEK() >= argumentRight;
        }
        break;

        case OP_POP_RES:
        {
            uint64_t result = POP();
            vm.result = result;
        }
        break;

        case OP_DONE:
        {
            return SUCCESS;
        }

        case OP_PRINT:
        {
            uint64_t argument = POP();
            printf("%" PRIu64 "\n", argument);
        }
        break;

        case OP_ABORT:
        {
            return ERROR_END_OF_STREAM;
        }

        case 26 ... 0x1f:
            return ERROR_UNKNOWN_OPCODE;
        default:
            return ERROR_UNKNOWN_OPCODE;
        }
    }
    return ERROR_END_OF_STREAM;
}