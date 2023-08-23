#include "3tle3wa/pass/analysis/structure/structure.hh"

#include <cassert>
#include <stack>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/structure/branch.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/function/structure/structure.hh"

Loop *StructureAnalysis::FindInNode2Loop(CtrlFlowGraphNode *node) {
    if (Node2Loop.find(node) == Node2Loop.end()) {
        assert(false);
    }
    return Node2Loop[node];
}

void StructureAnalysis::RmvLoopInNode2Loop(Loop *del_loop) {
    for (auto &&iter = Node2Loop.begin(); iter != Node2Loop.end(); ++iter) {
        auto &&[node, loop] = (*iter);
        if (del_loop == loop) {
            iter = Node2Loop.erase(iter);
            return;
        }
    }
    assert(false);
}

void StructureAnalysis::LoopAnalysis(NormalFuncPtr &func) {
    if (func->loops == nullptr) {
        delete func->loops;
    }
    Node2Loop.clear();

    std::stack<Loop *> stack;
    depth_t depth = 0;
    func->loops = new Loop(nullptr, depth++);
    stack.push(func->loops);

    auto &&seq_nodes = func->GetSequentialNodes();
    for (auto &&node : seq_nodes) {
        // cout << "Node_" << node->GetBlockIdx() << endl;
        const auto &blk_attr = node->blk_attr;

        if (blk_attr.structure_out) {
            stack.top()->loop_exit = node;
            stack.pop();
            depth -= 1;
        }

        if (blk_attr.cond_begin) {
            stack.top()->cond_begin = node;
            Node2Loop[node.get()] = stack.top();
        }
        if (blk_attr.cond_end) stack.top()->cond_end = node;
        if (blk_attr.body_begin) stack.top()->body_begin = node;

        if (blk_attr.ChkOneOfBlkType(BlkAttr::GoReturn, BlkAttr::InlineGR)) {
            node->blk_attr.loop = stack.top();  // record this GR belongs to which loop
        }

        if (blk_attr.before_blk) {
            Loop *loop = new Loop(stack.top(), depth++);
            loop->before_blk = node;

            stack.top()->sub_structures.push_back(loop);
            stack.push(loop);
        }
    }
}

void StructureAnalysis::BranchAnalysis(NormalFuncPtr &) {}