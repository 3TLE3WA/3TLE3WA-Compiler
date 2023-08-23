#include "3tle3wa/pass/interprocedural/sccp/sccp.hh"

#include <cassert>
#include <cstddef>
#include <memory>

#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/uninitvar.hh"
#include "3tle3wa/ir/value/variable.hh"
#include "3tle3wa/pass/interprocedural/dce/dce.hh"

SCCP::Lattice::Lattice() : state(Undefine), value(nullptr) {}
SCCP::Lattice::Lattice(State _state, BaseValuePtr _value) : state(_state), value(_value) {}

SCCP::Lattice SCCP::CreateConstant(BaseValuePtr _value) { return Lattice(Lattice::Constant, _value); }

bool SCCP::Lattice::IsUndefine() const { return state == State::Undefine; }
bool SCCP::Lattice::IsConstant() const { return state == State::Constant; }
bool SCCP::Lattice::IsVariable() const { return state == State::Variable; }

BaseValuePtr SCCP::Lattice::GetValue() const {
    assert(state == Constant && value->IsConstant());
    return value;
}

bool SCCP::Lattice::operator!=(const Lattice &rhs) { return (state != rhs.state) || (value != rhs.value); }

SCCP::Lattice SCCP::Meet(const Lattice &lhs, const Lattice &rhs) {
    if (lhs.state == Lattice::Undefine) {
        return rhs;
    }
    if (rhs.state == Lattice::Undefine) {
        return lhs;
    }
    if (lhs.state == Lattice::Variable || rhs.state == Lattice::Variable) {
        return Variable;
    }
    assert(lhs.state == Lattice::Constant && rhs.state == Lattice::Constant);
    return ((lhs.value == rhs.value) ? lhs : Variable);
}

SCCP::Lattice SCCP::GetLattice(BaseValuePtr value) {
    if (value == nullptr) {
        return Variable;
    } else if (value->IsConstant()) {
        return CreateConstant(value);
    } else {
        return LatticeMap[value];
    }
}

void SCCP::SetLattice(BaseValuePtr value, SCCP::Lattice &lattice) {
    if (value == nullptr) return;
    Lattice old = GetLattice(value);
    if ((old.state != lattice.state) || (old.value != lattice.value)) {
        LatticeMap[value] = lattice;

        for (auto user : value->GetUserList()) {
            SSAWorkList.push(user);
        }
    }
    return;
}

void SCCP::EvaluateOnPhi(PhiInstPtr inst) {
    Lattice lattice = Undefine;
    for (auto [value, block] : inst->GetDataList()) {
        if (ExcutedMap[block]) {
            lattice = Meet(lattice, GetLattice(value));
        }
    }
    SetLattice(inst->GetResult(), lattice);
    return;
}

void SCCP::EvaluateOnAssign(InstPtr inst) {
    if (inst->IsJumpInst()) {
        // jump
        auto jump_inst = std::static_pointer_cast<JumpInst>(inst);
        CFGWorkList.push(jump_inst->GetTarget());
    } else if (inst->IsBranchInst()) {
        // branch
        auto br_inst = std::static_pointer_cast<BranchInst>(inst);
        auto cond = GetLattice(br_inst->GetCondition());

        if (cond.IsConstant()) {
            CFGWorkList.push(std::get<bool>(std::static_pointer_cast<Constant>(cond.GetValue())->GetValue())
                                 ? br_inst->GetTrueTarget()
                                 : br_inst->GetFalseTarget());
        } else {
            CFGWorkList.push(br_inst->GetTrueTarget());
            CFGWorkList.push(br_inst->GetFalseTarget());
        }
    } else if (inst->IsPhiInst()) {
        // phi
        EvaluateOnPhi(std::static_pointer_cast<PhiInst>(inst));
    } else {
        auto result = inst->GetResult();
        Lattice new_ = GetLattice(result);
        if (inst->IsOneOprandInst()) {
            // load, bitcast, sitofp, fptosi, zext, fneg
            auto unary_inst = std::static_pointer_cast<UnaryInstruction>(inst);

            auto opr_lattice = GetLattice(unary_inst->GetOprand());
            if (opr_lattice.IsConstant()) {
                new_ = CreateConstant(DoUnaryFlod(unary_inst->GetOpCode(), opr_lattice.GetValue()));
            } else {
                new_ = Variable;
            }
        } else if (inst->IsTwoOprandInst()) {
            // add, sub, mul, div, rem, lshift, rshift
            // lth, leq, gth, geq, equ, neq
            auto binary_inst = std::static_pointer_cast<BinaryInstruction>(inst);
            auto opcode = binary_inst->GetOpCode();

            auto lhs_lattice = GetLattice(binary_inst->GetLHS());
            auto rhs_lattice = GetLattice(binary_inst->GetRHS());

            auto lhs = lhs_lattice.IsConstant() ? lhs_lattice.GetValue() : binary_inst->GetLHS();
            auto rhs = rhs_lattice.IsConstant() ? rhs_lattice.GetValue() : binary_inst->GetRHS();

            auto replacer = DoBinaryFlod(opcode, lhs, rhs);

            if (replacer == nullptr) {
                new_ = Variable;
            } else if (replacer->IsConstant()) {
                new_ = CreateConstant(replacer);
            } else if (replacer->IsVariable()) {
                new_ = GetLattice(replacer);
            } else {
                assert(false);
            }

            // if (lhs_lattice.IsConstant() && rhs_lattice.IsConstant()) {
            //     new_ = CreateConstant(
            //         DoBinaryFlod(binary_inst->GetOpCode(), lhs_lattice.GetValue(), rhs_lattice.GetValue()));
            // } else {
            //     new_ = Variable;
            // }
        } else {
            // ret, alloca, store, gep, call
            if (result) {
                new_ = Variable;
            }
        }

        SetLattice(result, new_);
    }
    return;
}

void SCCP::SCCP(NormalFuncPtr func) {
    for (auto param : func->GetParamList()) {
        LatticeMap.insert({param, Variable});
    }

    CFGWorkList.push(func->GetEntryNode());

    while (!CFGWorkList.empty() || !SSAWorkList.empty()) {
        if (!CFGWorkList.empty()) {
            auto node = CFGWorkList.front();
            CFGWorkList.pop();

            for (auto inst : node->GetInstList()) {
                if (inst->IsPhiInst()) {
                    EvaluateOnPhi(std::static_pointer_cast<PhiInst>(inst));
                }
            }

            if (!ExcutedMap[node]) {
                ExcutedMap[node] = true;
                for (auto inst : node->GetInstList()) {
                    EvaluateOnAssign(inst);
                }
            }
        } else if (!SSAWorkList.empty()) {
            auto inst = SSAWorkList.front();
            SSAWorkList.pop();

            if (ExcutedMap[inst->GetParent()]) {
                EvaluateOnAssign(inst);
            }
        }
    }

    auto &&node_list = func->GetSequentialNodes();

    for (auto &&iter = node_list.begin(); iter != node_list.end();) {
        auto node = (*iter);
        if (ExcutedMap[node] == true) {
            auto last_inst = node->GetLastInst();
            if (last_inst->IsBranchInst()) {
                auto &&br_inst = std::static_pointer_cast<BranchInst>(last_inst);
                auto &&cond = GetLattice(br_inst->GetCondition());
                auto &&iftrue = br_inst->GetTrueTarget();
                auto &&iffalse = br_inst->GetFalseTarget();
                assert(cond.IsConstant() || cond.IsVariable());

                if (iftrue == iffalse) {
                    node->RemoveLastInst();
                    node->InsertInstBack(JumpInst::CreatePtr(iftrue, node));
                } else if (cond.IsConstant()) {
                    br_inst->GetCondition()->RemoveUser(br_inst);
                    auto condition = std::get<bool>(std::static_pointer_cast<Constant>(cond.GetValue())->GetValue());
                    if (condition) {
                        node->RemoveLastInst();
                        node->InsertInstBack(JumpInst::CreatePtr(iftrue, node));
                        node->RmvSuccessor(iffalse);
                        iffalse->RmvPredecessor(node);
                    } else {
                        node->RemoveLastInst();
                        node->InsertInstBack(JumpInst::CreatePtr(iffalse, node));
                        node->RmvSuccessor(iftrue);
                        iftrue->RmvPredecessor(node);
                    }
                }
            }
            ++iter;
        } else {
            iter = node_list.erase(iter);
        }
    }

    DCE::EliminateUnreachableCode(func);

    for (auto &&node : node_list) {
        assert(ExcutedMap[node]);
        auto &&inst_list = node->GetInstList();
        for (auto &&iter = inst_list.begin(); iter != inst_list.end();) {
            auto inst = (*iter);
            auto result = inst->GetResult();
            auto lattice = GetLattice(result);
            if (lattice.IsConstant()) {
                ReplaceSRC(result, lattice.GetValue());
                RemoveInst(inst);
                iter = inst_list.erase(iter);
            } else {
                assert(lattice.IsVariable());
                if (inst->IsPhiInst()) {
                    auto phi_inst = std::static_pointer_cast<PhiInst>(inst);

                    auto &&datalist_ref = phi_inst->GetRefList();
                    for (auto &&it = datalist_ref.begin(); it != datalist_ref.end();) {
                        auto &&predecessors = node->GetPredecessors();

                        auto [value, block] = (*it);
                        if (ExcutedMap[block] == true && predecessors.find(block) != predecessors.end()) {
                            ++it;
                        } else {
                            it = datalist_ref.erase(it);
                        }
                    }
                    if (datalist_ref.size() == 1) {
                        ReplaceSRC(result, datalist_ref.begin()->first);
                        RemoveInst(inst);
                        iter = inst_list.erase(iter);
                        continue;
                    }
                }
                ++iter;
            }
        }
    }

    LatticeMap.clear();
    ExcutedMap.clear();
    return;
}
