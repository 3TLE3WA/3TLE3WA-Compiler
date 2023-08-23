#include "3tle3wa/ir/function/basicblock.hh"

size_t BasicBlock::blk_idx = 1;

BasicBlock::BasicBlock(BaseFunction *_parent, BlkAttr::BlkType blk_type)
    : idx(blk_idx++), inst_list(InstList()), parent(_parent), blk_attr(blk_type) {}

size_t BasicBlock::GetBlockIdx() { return idx; }

InstList &BasicBlock::GetInstList() { return inst_list; }

size_t BasicBlock::GetInstCnt() const { return inst_list.size(); }

InstPtr &BasicBlock::GetLastInst() {
    auto &&last_inst = (*inst_list.rbegin());
    assert(last_inst->IsJumpInst() || last_inst->IsBranchInst() || last_inst->IsReturnInst());
    return last_inst;
}

void BasicBlock::RemoveLastInst() {
    auto &&last_inst = GetLastInst();
    assert(last_inst->IsJumpInst() || last_inst->IsBranchInst());
    inst_list.pop_back();
}

void BasicBlock::InsertInstBack(InstPtr inst) { inst_list.push_back(inst); }
void BasicBlock::InsertInstFront(InstPtr inst) { inst_list.push_front(inst); }
void BasicBlock::RemoveInst(InstPtr inst) { inst_list.remove(inst); }

BaseFunction *BasicBlock::GetParent() { return parent; }

void BasicBlock::ResetBlkIdx() { blk_idx = 1; }
size_t BasicBlock::GetBlkIdx() { return blk_idx; }
void BasicBlock::SetBlkIdx(size_t _blk_idx) { blk_idx = _blk_idx; }
