#pragma once

#include <set>
#include <variant>

#include "3tle3wa/backend/rl/RLGen.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/use.hh"
#include "3tle3wa/pass/analysis/structure/structure.hh"

typedef std::vector<BaseValuePtr> Operands;
namespace LoopUnrolling {

constexpr size_t MAX_UNROLL_TIME = 3;

static std::set<BaseValuePtr> phi_results;
static std::set<BaseValuePtr> loop_variants;
static std::set<InstPtr> phi_to_update;
static std::unordered_map<BaseValuePtr, BaseValuePtr> phi_source_defined_in_loop;
static std::unordered_map<BaseValuePtr, BaseValuePtr> phi_source_defined_out_loop;
static std::unordered_map<BaseValuePtr, BaseValuePtr> phi_source_updated;
static std::unordered_map<BaseValuePtr, BaseValuePtr> old_to_new;  // old variant mapping to updated one
static std::unordered_map<CfgNodePtr, CfgNodePtr> block_mapping;

void LoopUnrolling(NormalFuncPtr);
bool ExpandLoop(NormalFuncPtr, Loop *);
void FullyExpand(NormalFuncPtr, int, Loop *);
void FullyExpand_Multi_Blks(NormalFuncPtr, int, Loop *);
int LoopTime(Loop *);
int ConstCheck(InstPtr);
void AddBranchInst(CfgNodePtr &, bool);
void AddJmpInst(CfgNodePtr &);
void UpdatePhiInst(CfgNodePtr &);
OpCode FlipComparisonOp(OpCode);
Operands InstOperandsInVector(InstPtr);
BaseValuePtr InstCopy(InstPtr &, CfgNodePtr &, bool);
BaseValuePtr OperandUpdate(BaseValuePtr, bool);
CfgNodePtr CfgNodeUpdate(const CfgNodePtr);

void DynamicUnrolling(NormalFuncPtr &);
}  // namespace LoopUnrolling