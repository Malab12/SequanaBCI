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