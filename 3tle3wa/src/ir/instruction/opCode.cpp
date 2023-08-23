#include "3tle3wa/ir/instruction/opCode.hh"

bool IsCommutative(OpCode opcode) {
    switch (opcode) {
        case OP_ADD:
        case OP_MUL:
        case OP_EQU:
        case OP_NEQ:
            return true;
        default:
            return false;
    }
}