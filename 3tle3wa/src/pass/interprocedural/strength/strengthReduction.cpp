#include <utility>

#include "3tle3wa/pass/interprocedural/strength/strengthReduction.h"

void strengthReduction::StrengthReduction(const NormalFuncPtr &func) {
    if (func->loops != nullptr) start(func->loops);
}

void strengthReduction::start(Loop *loop) {
    /*red("strengthReduction::start, depth=" + std::to_string(loop->depth));*/
    for (auto &&sub_loop: loop->sub_structures) {
        start(dynamic_cast<Loop *>(sub_loop));
    }
    if (loop->depth == 0 || !loop->IsSimpleLoop()) {
        /*cout << "depth == 0" << endl;*/
        return;
    }
    auto cond_body = loop->GetCondBodyBlks();
    auto body_begin = cond_body.front();
    if (body_begin == nullptr) {
        return;
    } else {
        auto body_inst_list = body_begin->GetInstList();
        auto first_inst = body_inst_list.front();
        if (!first_inst->IsPhiInst()) {
            // not-phi
            // -------------
        } else {
            auto phi = std::static_pointer_cast<PhiInst>(first_inst);
            auto ref_list = phi->GetRefList();
            auto ir = phi->tollvmIR();
            /*yellow("phi inst: " + ir);*/
            auto oprands = phi->GetOprands();
            auto init = oprands.front();
            oprands.pop_front();
            /*for (auto &&operand: operands) {
                yellow("operand: " + operand->tollvmIR());
            }*/
            std::map<int, InstPtr> idx2inst;
            /*blue("phi: " + phi->tollvmIR());*/
            for (auto &&val: oprands) {
                auto val_parent = val->GetParent();
                if (val_parent == nullptr) {
                    /*red("val_parent == nullptr");*/
                    continue;
                } else if (val_parent->IsTwoOprandInst()) {
                    /*yellow(val->tollvmIR() + " | " + val_parent->tollvmIR());*/
                    auto bin_inst = std::static_pointer_cast<BinaryInstruction>(val_parent);
                    if (bin_inst->GetLHS() != nullptr && bin_inst->GetRHS()->IsConstant()) {
                        if (bin_inst->GetOpCode() == OP_ADD) {
                            auto a_rhs = bin_inst->GetRHS();
                            auto a_rhs_const = std::static_pointer_cast<Constant>(a_rhs);
                            auto a_rhs_val = a_rhs_const->GetValue();
                            if (std::holds_alternative<int32_t>(a_rhs_val)) {
                                auto a_rhs_val_int = std::get<int32_t>(a_rhs_val);
                                /*green("a_rhs_val_int: " + std::to_string(a_rhs_val_int))*/;
                                auto a_lhs = bin_inst->GetLHS();

                                for (auto &&user: a_lhs->GetUserList()) {
                                    if (user->GetOpCode() == OP_MUL) {
                                        mulInst2Add(user, loop, a_rhs_val_int, bin_inst);
                                    } else if (user->GetOpCode() == OP_DIV) {
                                        divInst2sub(user, loop, a_rhs_val_int, bin_inst);
                                    }
                                }
                            } else if (std::holds_alternative<int64_t>(a_rhs_val)) {
                                auto a_rhs_val_int64 = std::get<int64_t>(a_rhs_val);
                                /*green("a_rhs_val_int64: " + std::to_string(a_rhs_val_int64))*/;
                                auto a_lhs = bin_inst->GetLHS();

                                for (auto &&user: a_lhs->GetUserList()) {
                                    if (user->GetOpCode() == OP_MUL) {
                                        mulInst2Add(user, loop, a_rhs_val_int64, bin_inst);
                                    } else if (user->GetOpCode() == OP_DIV) {
                                        divInst2sub(user, loop, a_rhs_val_int64, bin_inst);
                                    }
                                }
                            }
                        } else if (bin_inst->GetOpCode() == OP_SUB) {
                            auto s_rhs = bin_inst->GetRHS();
                            auto s_rhs_const = std::static_pointer_cast<Constant>(s_rhs);
                            auto s_rhs_val = s_rhs_const->GetValue();
                            if (std::holds_alternative<int32_t>(s_rhs_val)) {
                                auto s_rhs_val_int = std::get<int32_t>(s_rhs_val);
                                /*green("s_rhs_val_int: " + std::to_string(s_rhs_val_int))*/;
                                auto s_lhs = bin_inst->GetLHS();

                                for (auto &&user: s_lhs->GetUserList()) {
                                    if (user->GetOpCode() == OP_MUL) {
                                        mulInst2Add(user, loop, s_rhs_val_int, bin_inst);
                                    } else if (user->GetOpCode() == OP_DIV) {
                                        divInst2sub(user, loop, s_rhs_val_int, bin_inst);
                                    }
                                }
                            } else if (std::holds_alternative<int64_t>(s_rhs_val)) {
                                auto s_rhs_val_int64 = std::get<int64_t>(s_rhs_val);
                                /*green("s_rhs_val_int64: " + std::to_string(s_rhs_val_int64))*/;
                                auto s_lhs = bin_inst->GetLHS();

                                for (auto &&user: s_lhs->GetUserList()) {
                                    if (user->GetOpCode() == OP_MUL) {
                                        mulInst2Add(user, loop, s_rhs_val_int64, bin_inst);
                                    } else if (user->GetOpCode() == OP_DIV) {
                                        divInst2sub(user, loop, s_rhs_val_int64, bin_inst);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void strengthReduction::divInst2sub(InstPtr divInst, Loop *loop, int64_t step, InstPtr originInst) {
    auto binaryInst = std::static_pointer_cast<BinaryInstruction>(divInst);
    if (binaryInst == nullptr) {
        /*red("divInst2sub: binaryInst is nullptr");*/
        return;
    } else if (!binaryInst->GetRHS()->IsConstant()) {
        /*red("divInst2sub: rhs is not constant");*/
        return;
    } else {
        auto rhs = binaryInst->GetRHS();
        auto rhs_const = std::static_pointer_cast<Constant>(rhs);
        auto rhs_val = rhs_const->GetValue();
        if (std::holds_alternative<int32_t>(rhs_val)) {
            auto rhs_val_int = std::get<int32_t>(rhs_val);
            auto lhs = binaryInst->GetLHS();
            /*for (auto &&user: lhs->GetUserList()) {
                cout << "user: " + user->tollvmIR() << endl;
            }*/
            auto res = binaryInst->GetResult();
            auto node = binaryInst->GetParent();
            if (!isNotUsedInCurLoop(res, loop, node)){
                return;
            }
            /*auto inst_map = detectInstInBlk(lhs, binaryInst->GetParent(), loop);*/
            auto &&inst = binaryInst;
            inst->SetOpCode(OP_SUB);
            auto new_rhs = Constant::CreatePtr(rhs_val_int / step);
            inst->SetRHS(new_rhs);
        }
    }
}

void strengthReduction::mulInst2Add(InstPtr mulInst, Loop *loop, int64_t step, InstPtr originInst) {
    auto binaryInst = std::static_pointer_cast<BinaryInstruction>(mulInst);
    if (binaryInst == nullptr || !binaryInst->GetRHS()->IsConstant()) {
        /*red("mulInst2Add: binaryInst is nullptr");*/
        return;
    } else {
        auto rhs = binaryInst->GetRHS();
        auto rhs_const = std::static_pointer_cast<Constant>(rhs);
        auto rhs_val = rhs_const->GetValue();
        if (std::holds_alternative<int32_t>(rhs_val)) {
            auto rhs_val_int = std::get<int32_t>(rhs_val);
            auto lhs = binaryInst->GetLHS();
            auto res = binaryInst->GetResult();
            /*for (auto &&user: res->GetUserList()) {
                cout << "res user: " + user->tollvmIR() << endl;
            }*/
            auto node = binaryInst->GetParent();
            if (!isNotUsedInCurLoop(res, loop, node)){
                return;
            }
            /*auto inst_map = detectInstInBlk(lhs, binaryInst->GetParent(), loop);*/
            auto &&inst = binaryInst;
            inst->SetOpCode(OP_ADD);
            auto new_rhs = Constant::CreatePtr(rhs_val_int * step);
            inst->SetRHS(new_rhs);
            auto lhs_val = std::static_pointer_cast<Variable>(lhs);
            insertMulValInit(loop->body_begin, lhs_val, step, binaryInst->GetResult(), originInst);
        }
    }
}

bool strengthReduction::isNotUsedInCurLoop(VariablePtr var, Loop *pLoop, const CfgNodePtr& blk) {
    bool flag = true;
    for (auto &&loop: pLoop->sub_structures) {
        auto sub_loop = dynamic_cast<Loop *>(loop);
        flag &= isNotUsedInCurLoop(var, sub_loop, blk);
    }
    for (auto &&node: pLoop->GetEntireStructure()) {
        for (auto &&user_node: var->GetUserList()) {
            if (user_node->GetParent() == node && node != blk) {
                return false;
            } else if (node == blk) {
                return false;
            }
        }
    }
    return flag;
}

void strengthReduction::insertMulValInit(const CfgNodePtr &blk, const VariablePtr &vp, int64_t val, VariablePtr res,
                                         InstPtr originInst) {
    auto res_var = originInst->GetResult();
    int idx = 0;
//    cout << blk->tollvmIR() << endl;
    for (auto &&user: res_var->GetUserList()) {
//        cout << user->GetParent()->tollvmIR() << endl;
//        cout << idx++ << "user: " << user->tollvmIR() << endl;
        auto phi_user = std::static_pointer_cast<PhiInst>(user);
        if (phi_user == nullptr) {
            continue;
        }
        auto int_val = phi_user->GetRefList().front().first;
        if (int_val->IsConstant()) {
            auto int_cst = std::static_pointer_cast<Constant>(int_val);
            if (std::holds_alternative<int32_t>(int_cst->GetValue())) {
                auto int_val_int = std::get<int32_t>(int_cst->GetValue());
                auto &&new_cst = Constant::CreatePtr(int32_t(val * int_val_int));
                phi_user->GetRefList().front().first = new_cst;
            }
        }

//        cout << user->GetParent()->tollvmIR() << endl;
    }
}

std::map<InstPtr, int> strengthReduction::detectInstInBlk(const BaseValuePtr &lhs, const BlockPtr &blk, Loop *loop) {
    std::map<InstPtr, int> inst_map;
    for (auto &&inst: blk->GetInstList()) {
        if (inst->IsTwoOprandInst() && (inst->GetOpCode() == OP_MUL || inst->GetOpCode() == OP_DIV)) {
            auto binaryInst = std::static_pointer_cast<BinaryInstruction>(inst);
            if (binaryInst->GetLHS() == lhs && binaryInst->GetRHS()->IsConstant()) {
                /*yellow("detected Inst In Blk: " + inst->tollvmIR());*/
                if (isNotChanged(lhs, loop, inst))
                    inst_map.insert({inst, inst->GetInstIdx()});
            } else if (binaryInst->GetRHS() == lhs && binaryInst->GetLHS()->IsConstant()) {
                /*yellow("detected Inst In Blk: " + inst->tollvmIR());*/
                if (isNotChanged(lhs, loop, inst))
                    inst_map.insert({inst, (inst->GetInstIdx()) * (-1)});
            }
        }
    }
    return inst_map;
}


bool strengthReduction::isNotChanged(const BaseValuePtr &valPtr, Loop *pLoop, InstPtr &oInst) {
    if (pLoop->depth == 0) return true;
    bool flg = true;
    for (auto &&sloop: pLoop->sub_structures) {
        flg &= isNotChanged(valPtr, dynamic_cast<Loop *>(sloop), oInst);
    }
    if (!flg) return false;
    // check if valPtr is changed in loop
    for (auto &&blk: pLoop->GetCondBodyBlks()) {
        for (auto &&inst: blk->GetInstList()) {
            /*if (inst->IsTwoOprandInst()) {
                auto binaryInst = std::static_pointer_cast<BinaryInstruction>(inst);
                if ((binaryInst->GetRHS() == valPtr || binaryInst->GetLHS() == valPtr) && inst != oInst) {
                    red(" <cond, 2> changed in loop, exit: " + inst->tollvmIR());
                    return false;
                }
            } else if (inst->IsOneOprandInst()) {
                auto unaryInst = std::static_pointer_cast<UnaryInstruction>(inst);
                if (unaryInst->GetOprand() == valPtr && inst != oInst) {
                    red(" <cond, 1> changed in loop, exit: " + inst->tollvmIR());
                    return false;
                }
            }*/
            if (inst->GetResult() == valPtr && inst != oInst && !inst->IsPhiInst()) {
                /*red(" <cond> changed in loop, exit: " + inst->tollvmIR());*/
                return false;
            }
        }
    }
    for (auto &&blk: pLoop->GetLoopBodyBlks()) {
        for (auto &&inst: blk->GetInstList()) {
            /*if (inst->IsTwoOprandInst()) {
                auto binaryInst = std::static_pointer_cast<BinaryInstruction>(inst);
                if ((binaryInst->GetRHS() == valPtr || binaryInst->GetLHS() == valPtr) && inst != oInst) {
                    red(" <body, 2> changed in loop, exit: " + inst->tollvmIR());
                    return false;
                }
            } else if (inst->IsOneOprandInst()) {
                auto unaryInst = std::static_pointer_cast<UnaryInstruction>(inst);
                if (unaryInst->GetOprand() == valPtr && inst != oInst) {
                    red(" <body, 1> changed in loop, exit: " + inst->tollvmIR());
                    return false;
                }
            }*/
            if (inst->GetResult() == valPtr && inst != oInst && !inst->IsPhiInst()) {
                /*red(" <body> changed in loop, exit: " + inst->tollvmIR());*/
                return false;
            }
        }
    }

    return flg;
}

/*
void strengthReduction::red2(const std::string &s) {
//    cout << "\033[1;31m" << s << "\033[0m" << endl;
}

void strengthReduction::red(const std::string &s) {
//    cout << "\033[1;31m" << s << "\033[0m" << endl;
}

void strengthReduction::blue(const std::string &s) {
//    cout << "\033[1;34m" << s << "\033[0m" << endl;
}

void strengthReduction::green(const std::string &s) {
    cout << "\033[1;32m" << s << "\033[0m" << endl;
}

void strengthReduction::yellow(const std::string &s) {
    cout << "\033[1;33m" << s << "\033[0m" << endl;
}*/
