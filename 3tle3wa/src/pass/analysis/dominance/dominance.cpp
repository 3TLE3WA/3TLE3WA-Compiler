#include "3tle3wa/pass/analysis/dominance/dominance.hh"

void Dominance::ComputeDominanceInfo(CfgNodePtr entry, CfgNodeList allNodes) {
    // dominator-set
    auto ComputeDominatorSet = [&entry, &allNodes] {
        auto &&IntersectPredecessorsDom = [&](CfgNodePtr node) {
            DominatorSet dom_set = node->GetDominatorSet();
            auto &&predcessors = node->GetPredecessors();
            std::for_each(predcessors.begin(), predcessors.end(), [&dom_set](const auto &pred) {
                DominatorSet temp = DominatorSet();  // To Collect Intersection
                DominatorSet dom_set_pred = pred->GetDominatorSet();
                std::set_intersection(dom_set.begin(), dom_set.end(), dom_set_pred.begin(), dom_set_pred.end(),
                                      std::inserter(temp, temp.begin()));
                dom_set = temp;
            });
            dom_set.insert(node);
            return dom_set;
        };

        bool changed = true;
        while (changed) {
            changed = false;
            for (auto &&node : allNodes) {
                if (node == entry) continue;
                DominatorSet temp = IntersectPredecessorsDom(node);
                if (temp != node->GetDominatorSet()) {
                    node->SetDominatorSet(temp);
                    changed = true;
                }
            }
        }
    };
    ComputeDominatorSet();

    // immediate-dominator
    auto ComputeImmediateDominator = [](const auto &node) {
        DominatorSet dominator_set = node->GetDominatorSet();
        for (auto &&dominator : dominator_set) {
            if (dominator == node) continue;
            DominatorSet dom_set_of_dom = dominator->GetDominatorSet();
            dom_set_of_dom.insert(node);
            if (dom_set_of_dom == dominator_set) return dominator;
        }
        assert(false);
    };

    for (auto &&node : allNodes) {
        if (node == entry) continue;
        auto idom = ComputeImmediateDominator(node);
        node->SetImmediateDominator(idom);
        idom->InsertDominateChild(node);
    }
}

void Dominance::ComputeDominanceFrontier(CfgNodeList allNodes) {
    std::for_each(allNodes.begin(), allNodes.end(), [](const auto &node) { node->GetDomFrontier().clear(); });
    std::for_each(allNodes.begin(), allNodes.end(), [](const auto &node) {
        if (node->GetPredecessors().size() > 1) {
            for (auto &&pred : node->GetPredecessors()) {
                CfgNodePtr runner = pred;
                while (runner != node->GetImmediateDominator()) {
                    runner->InsertDomFrontier(node);
                    runner = runner->GetImmediateDominator();
                }
            }
        }
    });
}

void Dominance::DominanceAnalysis(NormalFuncPtr func) {
    auto entry = func->GetEntryNode();
    auto allNodes = func->GetSequentialNodes();

    auto Initialization = [&entry, &allNodes]() {
        DominatorSet allNodeSet = DominatorSet(allNodes.begin(), allNodes.end());
        entry->InsertDominator(entry);

        for (auto &&node : allNodes) {
            node->SetImmediateDominator(nullptr);
            node->GetDominateChildren().clear();
            node->GetDomFrontier().clear();
            if (node == entry) continue;
            node->SetDominatorSet(allNodeSet);
        }
    };

    Initialization();
    ComputeDominanceInfo(entry, allNodes);
    ComputeDominanceFrontier(allNodes);
}
