#include "3tle3wa/backend/rl/InterferenceGraph.hh"

#include "3tle3wa/backend/asm/AsmAbi.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"

InterferenceGraph::InterferenceGraph(RLPlanner *planner) : nodes_(), planner_(planner), mv_pairs_() {}

void InterferenceGraph::RegisterIGNode(VirtualRegister *ref) {
    auto idx = ref->GetVRIdx();
    nodes_[idx] = std::make_unique<IGNode>(idx, ref);
}

void InterferenceGraph::Connect(size_t first, size_t second) {
    nodes_.at(first)->ConnectTo(second);
    nodes_.at(second)->ConnectTo(first);
}

void InterferenceGraph::Mergable(size_t first, size_t second) {
    if (not nodes_.count(first) or not nodes_.count(second)) {
        return;
    }

    auto l = std::min(first, second);
    auto r = std::max(second, first);

    mv_pairs_.insert(std::make_pair(l, r));
}

InterferenceGraph::IGNode::IGNode(size_t idx, VirtualRegister *ref) : idx_(idx), color_(-1), ref_(ref), another_() {
    if (ref->IsParam() and not ref->IsOnStk()) {
        SetColor(ref->GetRealRegIdx());
    }
}

void InterferenceGraph::IGNode::ConnectTo(size_t another) { another_.insert(another); }

void InterferenceGraph::IGNode::CanMergeWith(IGNode *another) { merges_.insert(another); }

size_t InterferenceGraph::IGNode::GetDegree() const { return another_.size(); }

size_t InterferenceGraph::IGNode::GetColor() const { return color_; }

const std::unordered_set<size_t> &InterferenceGraph::IGNode::GetAnothers() const { return another_; }

VirtualRegister *InterferenceGraph::IGNode::GetRef() const { return ref_; }

void InterferenceGraph::IGNode::SetColor(size_t color) {
    color_ = color;
    ref_->SetRealRegIdx(color);

    // for (auto &&node : merges_) {
    //     if (not node->GetRef()->IsAllocated()) {
    //         node->SetColor(color);
    //     }
    // }
}

size_t InterferenceGraph::IGNode::PreferWhichArg() {
    if (ref_->IsParam()) {
        return ref_->GetRealRegIdx();
    }

    if (ref_->IsRetval()) {
        if (ref_->UseIGPR()) {
            return abi_info.i.arg_bgn;
        }
        if (ref_->UseFGPR()) {
            return abi_info.f.arg_bgn;
        }
    }

    return -1;
}

bool InterferenceGraph::IGNode::PreferCallerSave() { return false; }

bool InterferenceGraph::IGNode::PreferCalleeSave() { return true; }

bool InterferenceGraph::IGNode::CanSpill() { return ref_->CanSpill(); }

std::set<size_t> InterferenceGraph::IGNode::InterferWith(
    std::unordered_map<size_t, std::unique_ptr<IGNode>> &nodes_map) {
    std::set<size_t> intfer;
    for (auto &&another : another_) {
        auto color = nodes_map.at(another)->GetColor();
        if (color != (size_t)-1) {
            intfer.insert(color);
        }
    }
    return intfer;
}

bool InterferenceGraph::IGNode::operator<(const IGNode &other) const {
    if (not ref_->CanSpill() and other.ref_->CanSpill()) {
        return false;
    }

    if (GetDegree() == other.GetDegree()) {
        return ref_->Weight() < other.ref_->Weight();
    }

    return not(GetDegree() < other.GetDegree());
}

bool InterferenceGraph::IGNode::operator>(const IGNode &other) const {
    if (not ref_->CanSpill() and other.ref_->CanSpill()) {
        return true;
    }

    if (GetDegree() == other.GetDegree()) {
        return ref_->Weight() > other.ref_->Weight();
    }

    return not(GetDegree() > other.GetDegree());
}