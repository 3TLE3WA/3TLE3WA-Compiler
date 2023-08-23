#include "3tle3wa/pass/interprocedural/instcomb/instcomb.hh"

#include <fcntl.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <variant>

#include "3tle3wa/ir/instruction/compareInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/pass/interprocedural/dce/dce.hh"

InstComb::BinType InstComb::GetBinType(const BaseValue *lhs, const BaseValue *rhs) {
    if (lhs->IsVariable() && rhs->IsVariable()) {  // bin_inst = variable OP variable
        return BinType::LVRV;
    } else if (lhs->IsVariable() && rhs->IsConstant()) {  // bin_inst = variable OP constant
        return BinType::LVRC;
    } else if (lhs->IsConstant() && rhs->IsVariable()) {  // bin_inst = constant OP variable
        return BinType::LCRV;
    } else if (lhs->IsConstant() && rhs->IsConstant()) {  // bin_inst = constant OP constant
        return BinType::LCRC;
    }
    assert(false);
}

void InstComb::InstCombine(NormalFuncPtr func) {
    auto &&all_ndoes = func->GetSequentialNodes();

    // move constant or low-addr oprand to rhs
    for (auto &&node : all_ndoes) {
        for (auto &&inst : node->GetInstList()) {
            if (inst->IsTwoOprandInst()) {
                std::static_pointer_cast<BinaryInstruction>(inst)->SwapOprand();
            }
        }
    }

    for (auto &&node : all_ndoes) {
        auto &&inst_list = node->GetInstList();
        for (auto &&iter = inst_list.begin(); iter != inst_list.end(); ++iter) {
            auto &&inst = (*iter);

            // only combine binary-inst
            if (inst->IsTwoOprandInst() == false) continue;

            // process binary-inst
            auto &&bin_inst = std::static_pointer_cast<BinaryInstruction>(inst);
            auto bin_opcode = bin_inst->GetOpCode();

            auto &&bin_lhs = bin_inst->GetLHS();
            auto &&bin_rhs = bin_inst->GetRHS();

            auto &&lhs_inst = bin_lhs->GetParent();
            auto &&rhs_inst = bin_rhs->GetParent();

            // both lhs and rhs come from constant or parameter, cannot do combine
            if (lhs_inst == nullptr && rhs_inst == nullptr) continue;

            if (bin_inst->IsICmpInst()) {
                auto &&icmp_inst = std::static_pointer_cast<ICmpInst>(bin_inst);
                if (icmp_inst->IsIntegerNot() && lhs_inst->IsTwoOprandInst()) {
                    auto &&bin_lhs_inst = std::static_pointer_cast<BinaryInstruction>(lhs_inst);
                    if (bin_lhs_inst->IsICmpInst()) {
                        auto &&lhs_icmp_inst = std::static_pointer_cast<ICmpInst>(bin_lhs_inst);
                        if (lhs_icmp_inst->IsIntegerNot()) {
                            lhs_icmp_inst->SetOpCode(OP_NEQ);
                            ReplaceSRC(icmp_inst->GetResult(), lhs_icmp_inst->GetResult());
                        }
                    }
                } else if (icmp_inst->IsIntegerBool() && lhs_inst->IsTwoOprandInst()) {
                    auto &&bin_lhs_inst = std::static_pointer_cast<BinaryInstruction>(lhs_inst);
                    if (bin_lhs_inst->IsICmpInst()) {
                        auto &&lhs_icmp_inst = std::static_pointer_cast<ICmpInst>(bin_lhs_inst);
                        if (lhs_icmp_inst->IsIntegerBool()) {
                            ReplaceSRC(icmp_inst->GetResult(), lhs_icmp_inst->GetResult());
                        }
                    } else if (bin_lhs_inst->IsFCmpInst()) {
                        auto &&lhs_fcmp_inst = std::static_pointer_cast<FCmpInst>(bin_lhs_inst);
                        if (lhs_fcmp_inst->IsFloatBool()) {
                            ReplaceSRC(icmp_inst->GetResult(), lhs_fcmp_inst->GetResult());
                        }
                    }
                }
                continue;
            }

            bool iop = bin_inst->IsIBinaryInst();
            auto bin_type = GetBinType(bin_lhs.get(), bin_rhs.get());

            // Add, Sub, Mul, Div
            if (!(OP_ADD <= bin_opcode && bin_opcode <= OP_DIV)) continue;

            if (bin_type == LVRV) {
                // bin_inst = variable OP variable
                if (lhs_inst && lhs_inst->IsTwoOprandInst() && lhs_inst->GetParent() == node) {
                    auto &&bin_lhs_inst = std::static_pointer_cast<BinaryInstruction>(lhs_inst);
                    auto bin_lhs_opcode = bin_lhs_inst->GetOpCode();

                    if (!(OP_ADD <= bin_lhs_opcode && bin_lhs_opcode <= OP_DIV)) {
                        continue;
                    }

                    auto &&bin_lhs_lhs = bin_lhs_inst->GetLHS();
                    auto &&bin_lhs_rhs = bin_lhs_inst->GetRHS();

                    assert(bin_lhs == bin_lhs_inst->GetResult());
                    assert((bin_inst->IsIBinaryInst() && bin_lhs_inst->IsIBinaryInst()) ||
                           (bin_inst->IsFBinaryInst() && bin_lhs_inst->IsFBinaryInst()));

                    auto lhs_bin_type = GetBinType(bin_lhs_lhs.get(), bin_lhs_rhs.get());

                    if (rhs_inst && rhs_inst->IsTwoOprandInst() && rhs_inst->GetParent() == node) {
                        auto &&bin_rhs_inst = std::static_pointer_cast<BinaryInstruction>(rhs_inst);
                        auto bin_rhs_opcode = bin_rhs_inst->GetOpCode();

                        if (!(OP_ADD <= bin_rhs_opcode && bin_rhs_opcode <= OP_DIV)) {
                            continue;
                        }

                        auto &&bin_rhs_lhs = bin_rhs_inst->GetLHS();
                        auto &&bin_rhs_rhs = bin_rhs_inst->GetRHS();

                        assert(bin_rhs == bin_rhs_inst->GetResult());
                        assert((bin_inst->IsIBinaryInst() && bin_rhs_inst->IsIBinaryInst()) ||
                               (bin_inst->IsFBinaryInst() && bin_rhs_inst->IsFBinaryInst()));

                        auto rhs_bin_type = GetBinType(bin_rhs_lhs.get(), bin_rhs_rhs.get());

                        bool combine1 = (bin_lhs_lhs == bin_lhs_rhs) && (bin_lhs_lhs == bin_rhs);
                        bool combine2 = (bin_lhs == bin_rhs_lhs) && (bin_rhs_lhs == bin_rhs_rhs);

                        if (combine1 || combine2) {
                            bin_lhs->RemoveUser(bin_inst);
                            bin_rhs->RemoveUser(bin_inst);
                            if (combine1) {
                                if (bin_opcode == OP_ADD && bin_lhs_opcode == OP_ADD) {
                                    bin_inst->SetOpCode(OP_MUL);
                                    bin_inst->SetLHS(bin_rhs);
                                    bin_rhs->InsertUser(bin_inst);
                                    bin_inst->SetRHS(iop ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(3))
                                                         : ConstantAllocator::FindConstantPtr(static_cast<float>(3)));
                                } else if (bin_opcode == OP_ADD && bin_lhs_opcode == OP_SUB) {
                                } else if (bin_opcode == OP_SUB && bin_lhs_opcode == OP_ADD) {
                                } else if (bin_opcode == OP_SUB && bin_lhs_opcode == OP_SUB) {
                                }
                            } else {
                                if (bin_opcode == OP_ADD && bin_rhs_opcode == OP_ADD) {
                                    bin_inst->SetOpCode(OP_MUL);
                                    bin_inst->SetLHS(bin_lhs);
                                    bin_lhs->InsertUser(bin_inst);
                                    bin_inst->SetRHS(iop ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(3))
                                                         : ConstantAllocator::FindConstantPtr(static_cast<float>(3)));
                                } else if (bin_opcode == OP_ADD && bin_rhs_opcode == OP_SUB) {
                                } else if (bin_opcode == OP_SUB && bin_rhs_opcode == OP_ADD) {
                                } else if (bin_opcode == OP_SUB && bin_rhs_opcode == OP_SUB) {
                                }
                            }
                            continue;
                        }

                        CRVC_UNUSE bool combine3 = (lhs_bin_type == LVRC) && (bin_lhs_lhs == bin_rhs);
                        bool combine4 = (rhs_bin_type == LVRC) && (bin_rhs_lhs == bin_lhs);
                        if (combine4) {
                            if (bin_opcode == OP_ADD && bin_rhs_opcode == OP_MUL) {
                                bin_inst->SetOpCode(OP_MUL);
                                bin_rhs->RemoveUser(bin_inst);

                                auto &&bin_rhs_rhs_constant = std::static_pointer_cast<Constant>(bin_rhs_rhs);

                                auto &&constant = iop ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(
                                                            std::get<int32_t>(bin_rhs_rhs_constant->GetValue()) + 1))
                                                      : ConstantAllocator::FindConstantPtr(static_cast<float>(
                                                            std::get<float>(bin_rhs_rhs_constant->GetValue()) + 1));

                                bin_inst->SetRHS(constant);

                                continue;
                            }
                        }
                    } else if (rhs_inst == nullptr) {
                    }
                } else if (rhs_inst && rhs_inst->IsTwoOprandInst() && rhs_inst->GetParent() == node) {
                }
            } else if (bin_type == LVRC && lhs_inst && lhs_inst->IsTwoOprandInst() && lhs_inst->GetParent() == node) {
                // bin_inst = variable OP constant
                auto &&bin_rhs_constant = std::static_pointer_cast<Constant>(bin_rhs);

                auto &&bin_lhs_inst = std::static_pointer_cast<BinaryInstruction>(lhs_inst);
                auto bin_lhs_opcode = bin_lhs_inst->GetOpCode();

                if (!(OP_ADD <= bin_lhs_opcode && bin_lhs_opcode <= OP_DIV)) {
                    continue;
                }

                auto &&bin_lhs_lhs = bin_lhs_inst->GetLHS();
                auto &&bin_lhs_rhs = bin_lhs_inst->GetRHS();

                assert(bin_lhs == bin_lhs_inst->GetResult());
                assert((bin_inst->IsIBinaryInst() && bin_lhs_inst->IsIBinaryInst()) ||
                       (bin_inst->IsFBinaryInst() && bin_lhs_inst->IsFBinaryInst()));

                auto lhs_bin_type = GetBinType(bin_lhs_lhs.get(), bin_lhs_rhs.get());
                if (lhs_bin_type == LVRC) {
                    auto &&bin_lhs_rhs_constant = std::static_pointer_cast<Constant>(bin_lhs_rhs);

                    ConstType value;
                    bool combine = true;

                    std::visit(
                        [&value, &combine, &iop, op1 = bin_opcode, op2 = bin_lhs_opcode](auto &&lhs, auto &&rhs) {
                            using type_l = std::decay_t<decltype(lhs)>;
                            using type_r = std::decay_t<decltype(rhs)>;
                            assert((std::is_same_v<type_l, type_r>));

                            if ((op1 == OP_ADD && op2 == OP_ADD) || (op1 == OP_SUB && op2 == OP_SUB)) {
                                value = lhs + rhs;
                            } else if ((op1 == OP_ADD && op2 == OP_SUB) || (op1 == OP_SUB && op2 == OP_ADD)) {
                                value = lhs - rhs;
                            } else if (op1 == OP_MUL && op2 == OP_MUL) {
                                value = lhs * rhs;
                            } else if (op1 == OP_MUL && op2 == OP_DIV) {
                                if (iop) {
                                    combine = false;
                                } else {
                                    value = lhs / rhs;
                                }
                            } else if (op1 == OP_DIV && op2 == OP_MUL) {
                                if (rhs < lhs) {
                                    combine = false;
                                } else {
                                    if (iop) {
                                        if (static_cast<int32_t>(rhs) % static_cast<int32_t>(lhs) == 0) {
                                            value = rhs / lhs;
                                        } else {
                                            combine = false;
                                        }
                                    } else {
                                        value = rhs / lhs;
                                    }
                                }
                            } else if (op1 == OP_DIV && op2 == OP_DIV) {
                                value = lhs * rhs;
                            } else {
                                combine = false;
                            }
                        },
                        bin_rhs_constant->GetValue(), bin_lhs_rhs_constant->GetValue());

                    if (combine) {
                        auto &&constant = ConstantAllocator::FindConstantPtr(value);
                        bin_lhs_lhs->RemoveUser(bin_lhs_inst);
                        bin_lhs_lhs->InsertUser(bin_inst);
                        bin_inst->SetLHS(bin_lhs_lhs);
                        bin_inst->SetRHS(constant);
                    }
                } else if (lhs_bin_type == LCRV) {
                    auto &&bin_lhs_lhs_constant = std::static_pointer_cast<Constant>(bin_lhs_lhs);

                    ConstType value;
                    int32_t combine = 0;

                    std::visit(
                        [&value, &combine, &iop, op1 = bin_opcode, op2 = bin_lhs_opcode](auto &&lhs, auto &&rhs) {
                            using type_l = std::decay_t<decltype(lhs)>;
                            using type_r = std::decay_t<decltype(rhs)>;
                            assert((std::is_same_v<type_l, type_r>));

                            if (op1 == OP_ADD && (op2 == OP_ADD || op2 == OP_SUB)) {
                                value = lhs + rhs;
                                combine = (op2 == OP_ADD) ? 1 : 2;
                            } else if (op1 == OP_SUB && (op2 == OP_ADD || op2 == OP_SUB)) {
                                value = rhs - lhs;
                                combine = (op2 == OP_ADD) ? 1 : 2;
                            } else if (op1 == OP_MUL && (op2 == OP_MUL || op2 == OP_DIV)) {
                                if (op2 == OP_MUL) {
                                    value = lhs * rhs;
                                    combine = (op2 == OP_MUL) ? 3 : 4;
                                } else {
                                    if (iop == false) {
                                        value = rhs * lhs;
                                        combine = (op2 == OP_MUL) ? 3 : 4;
                                    }
                                }
                            } else if (op1 == OP_DIV && (op2 == OP_MUL || op2 == OP_DIV)) {
                                if (iop) {
                                    if (static_cast<int32_t>(rhs) % static_cast<int32_t>(lhs) == 0) {
                                        value = rhs / lhs;
                                        combine = (op2 == OP_MUL) ? 3 : 4;
                                    }
                                } else {
                                    value = rhs / lhs;
                                    combine = (op2 == OP_MUL) ? 3 : 4;
                                }
                            }
                        },
                        bin_rhs_constant->GetValue(), bin_lhs_lhs_constant->GetValue());

                    if (combine) {
                        auto &&constant = ConstantAllocator::FindConstantPtr(value);
                        bin_lhs_rhs->RemoveUser(bin_lhs_inst);
                        bin_lhs_rhs->InsertUser(bin_inst);
                        if (combine == 1 || combine == 3) {
                            bin_inst->SetOpCode(combine == 1 ? OP_ADD : OP_MUL);
                            bin_inst->SetLHS(bin_lhs_rhs);
                            bin_inst->SetRHS(constant);
                        } else if (combine == 2 || combine == 4) {
                            bin_inst->SetOpCode(combine == 2 ? OP_SUB : OP_DIV);
                            bin_inst->SetLHS(constant);
                            bin_inst->SetRHS(bin_lhs_rhs);
                        } else {
                            assert(false);
                        }
                    }
                }
            } else if (bin_type == LCRV && rhs_inst && rhs_inst->IsTwoOprandInst() && rhs_inst->GetParent() == node) {
                // bin_inst = constant OP variable
                auto &&bin_lhs_constant = std::static_pointer_cast<Constant>(bin_lhs);

                auto &&bin_rhs_inst = std::static_pointer_cast<BinaryInstruction>(rhs_inst);
                auto bin_rhs_opcode = bin_rhs_inst->GetOpCode();

                if (!(OP_ADD <= bin_rhs_opcode && bin_rhs_opcode <= OP_DIV)) {
                    continue;
                }

                auto &&bin_rhs_lhs = bin_rhs_inst->GetLHS();
                auto &&bin_rhs_rhs = bin_rhs_inst->GetRHS();

                assert(bin_rhs == bin_rhs_inst->GetResult());
                assert((bin_inst->IsIBinaryInst() && bin_rhs_inst->IsIBinaryInst()) ||
                       (bin_inst->IsFBinaryInst() && bin_rhs_inst->IsFBinaryInst()));

                auto rhs_bin_type = GetBinType(bin_rhs_lhs.get(), bin_rhs_rhs.get());
                if (rhs_bin_type == LVRC) {
                    auto &&bin_rhs_rhs_constant = std::static_pointer_cast<Constant>(bin_rhs_rhs);

                    ConstType value;
                    int32_t combine = 0;

                    std::visit(
                        [&value, &combine, &iop, op1 = bin_opcode, op2 = bin_rhs_opcode](auto &&lhs, auto &&rhs) {
                            using type_l = std::decay_t<decltype(lhs)>;
                            using type_r = std::decay_t<decltype(rhs)>;
                            assert((std::is_same_v<type_l, type_r>));

                            if (op1 == OP_ADD && (op2 == OP_ADD || op2 == OP_SUB)) {
                                value = (op2 == OP_ADD) ? lhs + rhs : lhs - rhs;
                                combine = 1;
                            } else if (op1 == OP_SUB && (op2 == OP_ADD || op2 == OP_SUB)) {
                                value = (op2 == OP_ADD) ? lhs - rhs : lhs + rhs;
                                combine = 2;
                            } else if (op1 == OP_MUL && (op2 == OP_MUL || op2 == OP_DIV)) {
                                if (op2 == OP_MUL) {
                                    value = lhs * rhs;
                                    combine = 3;
                                } else {
                                    if (iop == false) {
                                        value = lhs / rhs;
                                        combine = 3;
                                    }
                                }
                            } else if (op1 == OP_DIV && (op2 == OP_MUL || op2 == OP_DIV)) {
                                if (iop == false) {
                                    if (op2 == OP_MUL) {
                                        value = lhs / rhs;
                                        combine = 3;
                                    } else {
                                        value = lhs * rhs;
                                        combine = 4;
                                    }
                                }
                            }
                        },
                        bin_lhs_constant->GetValue(), bin_rhs_rhs_constant->GetValue());

                    if (combine) {
                        auto &&constant = ConstantAllocator::FindConstantPtr(value);
                        bin_rhs_lhs->RemoveUser(bin_rhs_inst);
                        bin_rhs_lhs->InsertUser(bin_inst);
                        if (combine == 1 || combine == 3) {
                            bin_inst->SetLHS(bin_rhs_lhs);
                            bin_inst->SetRHS(constant);
                        } else if (combine == 2 || combine == 4) {
                            bin_inst->SetLHS(constant);
                            bin_inst->SetRHS(bin_rhs_lhs);
                        } else {
                            assert(false);
                        }
                    }
                } else if (rhs_bin_type == LCRV) {
                    auto &&bin_rhs_lhs_constant = std::static_pointer_cast<Constant>(bin_rhs_lhs);

                    ConstType value;
                    int32_t combine = 0;

                    std::visit(
                        [&value, &combine, &iop, op1 = bin_opcode, op2 = bin_rhs_opcode](auto &&lhs, auto &&rhs) {
                            using type_l = std::decay_t<decltype(lhs)>;
                            using type_r = std::decay_t<decltype(rhs)>;
                            assert((std::is_same_v<type_l, type_r>));

                            if (op1 == OP_ADD && (op2 == OP_ADD || op2 == OP_SUB)) {
                                value = lhs + rhs;
                                combine = (op2 == OP_ADD) ? 1 : 2;
                            } else if (op1 == OP_SUB && (op2 == OP_ADD || op2 == OP_SUB)) {
                                value = lhs - rhs;
                                combine = (op2 == OP_ADD) ? 2 : 1;
                            } else if (op1 == OP_MUL && (op2 == OP_MUL || op2 == OP_DIV)) {
                                if (op2 == OP_MUL) {
                                    value = lhs * rhs;
                                    combine = 3;
                                } else {
                                    if (iop == false) {
                                        value = lhs * rhs;
                                        combine = 4;
                                    }
                                }
                            } else if (op1 == OP_DIV && (op2 == OP_MUL || op2 == OP_DIV)) {
                                if (iop == false) {
                                    value = lhs / rhs;
                                    combine = (op2 == OP_MUL) ? 4 : 3;
                                }
                            }
                        },
                        bin_lhs_constant->GetValue(), bin_rhs_lhs_constant->GetValue());

                    if (combine) {
                        auto &&constant = ConstantAllocator::FindConstantPtr(value);
                        bin_rhs_rhs->RemoveUser(bin_rhs_inst);
                        bin_rhs_rhs->InsertUser(bin_inst);
                        if (combine == 1 || combine == 3) {
                            bin_inst->SetOpCode((combine == 1) ? OP_ADD : OP_MUL);
                            bin_inst->SetLHS(bin_rhs_rhs);
                            bin_inst->SetRHS(constant);
                        } else if (combine == 2 || combine == 4) {
                            bin_inst->SetOpCode((combine == 2) ? OP_SUB : OP_DIV);
                            bin_inst->SetLHS(constant);
                            bin_inst->SetRHS(bin_rhs_rhs);
                        } else {
                            assert(false);
                        }
                    }
                }
            }
        }
    }
    DCE::EliminateUselessCode(func);
    return;
}
