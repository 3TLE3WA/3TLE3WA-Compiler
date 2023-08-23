#include "3tle3wa/pass/Pass.hh"

#include "3tle3wa/pass/interprocedural/dce/dce.hh"
#include "3tle3wa/pass/interprocedural/loop/loopunrolling.hh"

Optimization::Optimization(CompilationUnit &_comp_unit) : comp_unit(_comp_unit) {}

void Optimization::DoOptimization() {
    GVL::GlbValueLocalization(comp_unit);

    for (auto &&func : comp_unit.GetNormalFuncTable()) {
        SideEffect::SideEffectAnalysis(comp_unit, func);

        Variable::SetVarIdx(func->GetVarIdx());
        BasicBlock::SetBlkIdx(func->GetBlkIdx());

        Dominance::DominanceAnalysis(func);

        SSA::SSAConstruction(func);

        GVN::DVNT(func, comp_unit.getGlbTable());

        StructureAnalysis::LoopAnalysis(func);

        LoopInvariant::LoopInvariant(func);

        LoopMemset::LoopMemset(func, comp_unit);

        LoopUnrolling::LoopUnrolling(func);

        SCCP::SCCP(func);

        DCE::DCE(func);

        // Inline::InlineOptFunc(func, comp_unit.getGlbTable());

        DCE::DCE(func);

        PeepHole::PeepHole4Gep(func, comp_unit.getGlbTable());

        HoistLocalArray::HoistLocalArray(func, comp_unit.getGlbTable());

        PeepHole::PeepHoleOpt(func);

        PeepHole::PeepHole4Gep(func, comp_unit.getGlbTable());

        GVN::DVNT(func, comp_unit.getGlbTable());

        PeepHole::PeepHoleOpt(func);

        DCE::DCE(func);

        InstComb::InstCombine(func);

        GVN::DVNT(func, comp_unit.getGlbTable());

        LoopUnrolling::DynamicUnrolling(func);

        // SSA::SSADestruction(func);

        DCE::DCE(func);

        TailCall::TailCallAnalysis(func);

        RecursionOpt::DoRecursionOpt(func, comp_unit.getGlbTable());

        func->SetVarIdx(Variable::GetVarIdx());
        func->SetBlkIdx(BasicBlock::GetBlkIdx());

        EDP::EliminateDeadParameter(func);
    }
}