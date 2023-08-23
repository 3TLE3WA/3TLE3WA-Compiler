#pragma once

#include <list>
#include <map>
#include <unordered_map>

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/baseValue.hh"

namespace GVN {

struct BinVNExpr {
    OpCode opcode;
    BaseValue *lhs;
    BaseValue *rhs;

    bool operator==(const BinVNExpr &) const;
};

struct BinVNExprHasher {
    size_t operator()(const BinVNExpr &) const;
};

typedef std::unordered_map<BinVNExpr, BaseValuePtr, BinVNExprHasher> BinVNTable;

struct GepVNExpr {
    size_t off_size;
    BaseValue *base_addr;
    BaseValue *last_off;

    bool operator==(const GepVNExpr &) const;
};

struct GepVNExprHasher {
    size_t operator()(const GepVNExpr &) const;
};

typedef std::unordered_map<GepVNExpr, BaseValuePtr, GepVNExprHasher> GepVNTable;

struct MemoryVNExpr {
    BaseValue *base_addr;
    std::list<BaseValue *> offsets;

    bool operator==(const MemoryVNExpr &) const;
};

struct MemoryVNExprHasher {
    size_t operator()(const MemoryVNExpr &) const;
};

typedef std::unordered_map<MemoryVNExpr, BaseValuePtr, MemoryVNExprHasher> MemoryVNTable;

struct UnaryVNExpr {
    OpCode opcode;
    BaseValue *oprand;

    bool operator==(const UnaryVNExpr &) const;
};

struct UnaryVNExprHasher {
    size_t operator()(const UnaryVNExpr &) const;
};

typedef std::unordered_map<UnaryVNExpr, BaseValuePtr, UnaryVNExprHasher> UnaryVNTable;

struct VNScope {
    BinVNTable bin_map;      // GVN
    GepVNTable gep_map;      // GVN
    MemoryVNTable mem_map;   // LVN
    UnaryVNTable unary_map;  // GVN

    VNScope *outer;

    VNScope(VNScope *);

    BaseValuePtr Get(InstPtr);
    void Set(InstPtr);
};

typedef std::unordered_map<BaseValuePtr, BaseValuePtr> ValueNumber;

static ValueNumber VN;

BaseValuePtr GetVN(BaseValuePtr);

bool IsMeaingLess(InstPtr);
bool IsRedundant(CfgNodePtr, InstPtr);

bool IsPhiOprandSame(InstPtr);

void AdjustPhiInst(CfgNodePtr, PhiInstPtr);

void DoDVNT(CfgNodePtr, VNScope *, SymbolTable &);
void DVNT(NormalFuncPtr, SymbolTable &);
}  // namespace GVN
