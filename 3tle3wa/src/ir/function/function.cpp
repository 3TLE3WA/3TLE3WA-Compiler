#include "3tle3wa/ir/function/function.hh"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/utils/logs.hh"

//===-----------------------------------------------------------===//
//                     NormalFunction Implementation
//===-----------------------------------------------------------===//

NormalFunction::NormalFunction(ScalarTypePtr _type, std::string &_name, ParamList &_list, bool _effect)
    : BaseFunction(_type, _name, _list, _effect),
      loops(nullptr),
      branch(nullptr),
      se_type(SEtype{false, false, false}) {}

bool NormalFunction::IsLibFunction() const { return false; }

CfgNodePtr NormalFunction::CreateEntry() {
    entry = CtrlFlowGraphNode::CreatePtr(this, BlkAttr::Entry | BlkAttr::Normal);
    return entry;
}

CfgNodePtr NormalFunction::CreateExit() {
    exit = CtrlFlowGraphNode::CreatePtr(this, BlkAttr::Exit | BlkAttr::Normal);
    return exit;
}

CfgNodePtr NormalFunction::CreateCfgNode(BlkAttr::BlkType blk_type) {
    return CtrlFlowGraphNode::CreatePtr(this, blk_type);
}

CfgNodePtr NormalFunction::GetEntryNode() { return entry; }
CfgNodePtr NormalFunction::GetExitNode() { return exit; }

void NormalFunction::SetEntryNode(CfgNodePtr _entry) { entry = _entry; }
void NormalFunction::SetExitNode(CfgNodePtr _exit) { exit = _exit; }

CfgNodeList NormalFunction::GetSequentialNodes() {
    CfgNodeList list;
    std::unordered_map<CtrlFlowGraphNode *, bool> visit;

    auto &&ChkAllPredVisit = [&visit](CtrlFlowGraphNode *node) -> bool {
        if (node->blk_attr.ChkOneOfBlkType(BlkAttr::Entry)) return true;
        if (node->blk_attr.ChkOneOfBlkType(BlkAttr::Exit)) return false;
        assert(node->GetPredecessors().size() > 0);
        for (const auto &pred : node->GetPredecessors()) {
            // exclude loop back-edge
            // if (pred->blk_attr.body_end) continue;                            // body-end jump to cond-begin
            if (pred->blk_attr.ChkOneOfBlkType(BlkAttr::Continue)) continue;  // continue to cond-begin
            if (!visit[pred.get()]) return false;
        }
        return true;
    };

    std::stack<CfgNodePtr> stack;
    stack.push(entry);
    while (!stack.empty()) {
        auto &&top = stack.top();
        stack.pop();

        if (!visit[top.get()] && ChkAllPredVisit(top.get())) {
            visit[top.get()] = true;
            list.push_back(top);

            auto &&last_inst = top->GetLastInst().get();
            if (last_inst->IsBranchInst()) {
                BranchInst *br_inst = static_cast<BranchInst *>(last_inst);
                auto &&lhs = br_inst->GetTrueTarget();
                auto &&rhs = br_inst->GetFalseTarget();

                stack.push(rhs);
                stack.push(lhs);

            } else if (last_inst->IsJumpInst()) {
                JumpInst *jump_inst = static_cast<JumpInst *>(last_inst);
                auto &&target = jump_inst->GetTarget();

                // no matter Break or GR, it is pushed until all predecessor tag `visit`
                if (ChkAllPredVisit(target.get())) {
                    stack.push(target);
                }
            }
        }
    }
    if (!visit[exit.get()]) list.push_back(exit);

    return list;
}

CfgNodeList NormalFunction::GetReverseSeqNodes() {
    CfgNodeList list = GetSequentialNodes();
    list.reverse();

    return list;
}

void NormalFunction::SetVarIdx(size_t _var_idx) { var_idx = _var_idx; }
size_t NormalFunction::GetVarIdx() { return var_idx; }

void NormalFunction::SetBlkIdx(size_t _blk_idx) { blk_idx = _blk_idx; }
size_t NormalFunction::GetBlkIdx() { return blk_idx; }

NormalFuncPtr NormalFunction::CreatePtr(ScalarTypePtr _type, std::string &_name, ParamList &_list, bool _effect) {
    return std::make_shared<NormalFunction>(_type, _name, _list, _effect);
}

std::string NormalFunction::tollvmIR() {
    std::stringstream ss;

    ss << "define " << ret_type->tollvmIR() << " @" << func_name << "(";
    size_t param_size = param_list.size();
    if (param_size > 0) {
        size_t idx = 0;
        ss << param_list[idx]->GetBaseType()->tollvmIR() << " " << param_list[idx]->tollvmIR();
        for (idx = 1; idx < param_size; ++idx) {
            ss << ", " << param_list[idx]->GetBaseType()->tollvmIR() << " " << param_list[idx]->tollvmIR();
        }
    }

    ss << ") {" << endl;

    for (auto &&node : GetSequentialNodes()) {
        ss << node->tollvmIR() << endl;
    }

    ss << "}" << endl;

    return ss.str();
}

//===-----------------------------------------------------------===//
//                     LibraryFunction Implementation
//===-----------------------------------------------------------===//

LibraryFunction::LibraryFunction(ScalarTypePtr _type, std::string &_name, ParamList &_list, bool _effect)
    : BaseFunction(_type, _name, _list, _effect) {
    assert(recursive == false);
}

bool LibraryFunction::IsLibFunction() const { return true; }

std::string LibraryFunction::tollvmIR() {
    std::stringstream ss;

    ss << "declare " << ret_type->tollvmIR() << " @" << func_name << "(";
    size_t param_size = param_list.size();
    if (param_size > 0) {
        size_t idx = 0;
        ss << param_list[idx]->GetBaseType()->tollvmIR();
        for (idx = 1; idx < param_size; ++idx) {
            ss << ", " << param_list[idx]->GetBaseType()->tollvmIR();
        }
    }
    ss << ")";

    return ss.str();
}

//===-----------------------------------------------------------===//
//                     SYSYLibFunction Implementation
//===-----------------------------------------------------------===//

SYSYLibFunction::SYSYLibFunction(ScalarTypePtr _type, std::string &_name, ParamList &_list, bool _effect)
    : LibraryFunction(_type, _name, _list, _effect) {}

bool SYSYLibFunction::IsSYSYLibFunction() const { return true; }

SYSYLibFuncPtr SYSYLibFunction::CreatePtr(ScalarTypePtr _type, std::string _name, ParamList &_list, bool _effect) {
    return std::make_shared<SYSYLibFunction>(_type, _name, _list, _effect);
}

//===-----------------------------------------------------------===//
//                     LLVMLibFunction Implementation
//===-----------------------------------------------------------===//

LLVMLibFunction::LLVMLibFunction(std::string &_prote_name, size_t _proto_arg_nums, ScalarTypePtr _type,
                                 std::string &_name, ParamList &_list, bool _effect)
    : LibraryFunction(_type, _name, _list, _effect), proto_name(_prote_name), proto_arg_nums(_proto_arg_nums) {}

bool LLVMLibFunction::IsSYSYLibFunction() const { return false; }

std::string &LLVMLibFunction::GetProtoName() { return proto_name; }
size_t LLVMLibFunction::GetProtoArgNums() const { return proto_arg_nums; }

LLVMLibFuncPtr LLVMLibFunction::CreatePtr(std::string _prote_name, size_t _proto_arg_nums, ScalarTypePtr _type,
                                          std::string _name, ParamList &_list, bool _effect) {
    return std::make_shared<LLVMLibFunction>(_prote_name, _proto_arg_nums, _type, _name, _list, _effect);
}

std::ostream &operator<<(std::ostream &os, BaseFuncPtr func) {
    os << func->tollvmIR();
    return os;
}
