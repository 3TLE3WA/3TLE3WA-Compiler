#include "3tle3wa/ir/function/cfgNode.hh"

#include "3tle3wa/ir/function/basicblock.hh"

CtrlFlowGraphNode::CtrlFlowGraphNode(BaseFunction *_parent, BlkAttr::BlkType blk_type)
    : BasicBlock(_parent, blk_type) {}

bool CtrlFlowGraphNode::GetDirty() { return dirty; }
void CtrlFlowGraphNode::SetDirty(bool _dirty) { dirty = _dirty; }

CfgNodePtr CtrlFlowGraphNode::CreatePtr(BaseFunction *_parent, BlkAttr::BlkType blk_type) {
    return std::make_shared<CtrlFlowGraphNode>(_parent, blk_type);
}

void CtrlFlowGraphNode::AddPredecessor(CfgNodePtr predecessor) { predecessors.insert(predecessor); }
void CtrlFlowGraphNode::AddSuccessor(CfgNodePtr successor) { successors.insert(successor); }
void CtrlFlowGraphNode::RmvPredecessor(CfgNodePtr predecessor) { predecessors.erase(predecessor); }
void CtrlFlowGraphNode::RmvSuccessor(CfgNodePtr successor) { successors.erase(successor); }

Predecessor &CtrlFlowGraphNode::GetPredecessors() { return predecessors; }
Successor &CtrlFlowGraphNode::GetSuccessors() { return successors; }

void CtrlFlowGraphNode::InsertDominator(CfgNodePtr dominator) { dominator_set.insert(dominator); }
DominatorSet &CtrlFlowGraphNode::GetDominatorSet() { return dominator_set; }
void CtrlFlowGraphNode::SetDominatorSet(DominatorSet &_dom_set) {
    dominator_set.clear();
    dominator_set = _dom_set;
}

CfgNodePtr CtrlFlowGraphNode::GetImmediateDominator() { return immediate_dominator; }
void CtrlFlowGraphNode::SetImmediateDominator(CfgNodePtr _idom) { immediate_dominator = _idom; }

DominatorSet &CtrlFlowGraphNode::GetDominateChildren() { return dominate_children; }
void CtrlFlowGraphNode::InsertDominateChild(CfgNodePtr child) { dominate_children.insert(child); }

void CtrlFlowGraphNode::InsertDomFrontier(CfgNodePtr frontier) { dominance_frontier.insert(frontier); }
DominatorSet &CtrlFlowGraphNode::GetDomFrontier() { return dominance_frontier; }

std::string CtrlFlowGraphNode::tollvmIR() {
    std::stringstream ss;

    ss << "Block_" << idx << ": ; " << blk_attr.to_str() << endl;
    ss << "\t; Predecessors: ";
    for (auto &&pred : predecessors) {
        ss << pred->idx << ' ';
    }
    ss << '\n';

    ss << "\t; Successors: ";
    for (auto &&succ : successors) {
        ss << succ->idx << ' ';
    }
    ss << '\n';

    // ss << "\t; DominatorSet: ";
    // for (auto &&node : dominator_set) {
    //     ss << node->idx << ' ';
    // }
    // ss << '\n';

    // ss << "\t; ImmediateDominator: ";
    // if (immediate_dominator != nullptr) {
    //     ss << immediate_dominator->idx;
    // }
    // ss << '\n';

    // ss << "\t; DominateChildren: ";
    // for (auto &&node : dominate_children) {
    //     ss << node->idx << ' ';
    // }
    // ss << '\n';

    // ss << "\t; DominanceFrontier: ";
    // for (auto &&node : dominance_frontier) {
    //     ss << node->idx << ' ';
    // }
    // ss << '\n';

    for (auto &&inst : inst_list) {
        ss << '\t' << inst->tollvmIR() << endl;
    }

    return ss.str();
}

// clear instructions of del-node
// remove del-node from its predcessors and successors
// clear predcessors and successors of del-node
void RemoveNode(CfgNodePtr node) {
    auto &&inst_list = node->GetInstList();
    std::for_each(inst_list.begin(), inst_list.end(), RemoveInst);
    inst_list.clear();
    auto &&predcessor = node->GetPredecessors();
    auto &&successor = node->GetSuccessors();
    std::for_each(predcessor.begin(), predcessor.end(), [&node](const auto &pred) { pred->RmvSuccessor(node); });
    std::for_each(successor.begin(), successor.end(), [&node](const auto &succ) { succ->RmvPredecessor(node); });
    predcessor.clear();
    successor.clear();
    node->GetDominatorSet().clear();
    node->GetDomFrontier().clear();
    node->SetImmediateDominator(nullptr);
}
