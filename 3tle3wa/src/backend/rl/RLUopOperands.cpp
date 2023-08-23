#include "3tle3wa/backend/rl/RLUop.hh"

using VecVR = std::vector<VirtualRegister *>;

const VecVR UopRet::GetOperands() const {
    if (retval_ != nullptr) {
        return VecVR{retval_};
    }
    return VecVR{};
}

VirtualRegister *UopRet::GetResult() const { return nullptr; }

const VecVR UopCall::GetOperands() const { return params_; }

VirtualRegister *UopCall::GetResult() const { return retval_; }

const VecVR UopLui::GetOperands() const { return VecVR{}; }

VirtualRegister *UopLui::GetResult() const { return dst_; }

const VecVR UopLi::GetOperands() const { return VecVR{}; }

VirtualRegister *UopLi::GetResult() const { return dst_; }

const VecVR UopMv::GetOperands() const { return VecVR{src_}; }

VirtualRegister *UopMv::GetResult() const { return dst_; }

const VecVR UopCvtS2W::GetOperands() const { return VecVR{src_}; }

VirtualRegister *UopCvtS2W::GetResult() const { return dst_; }

const VecVR UopCvtW2S::GetOperands() const { return VecVR{src_}; }

VirtualRegister *UopCvtW2S::GetResult() const { return dst_; }

const VecVR UopBranch::GetOperands() const { return VecVR{cond_}; }

VirtualRegister *UopBranch::GetResult() const { return nullptr; }

const VecVR UopICmpBranch::GetOperands() const {
    VecVR vec;
    if (lhs_ != nullptr) {
        vec.push_back(lhs_);
    }
    if (rhs_ != nullptr) {
        vec.push_back(rhs_);
    }
    return vec;
}

VirtualRegister *UopICmpBranch::GetResult() const { return nullptr; }

const VecVR UopJump::GetOperands() const { return VecVR{}; }

VirtualRegister *UopJump::GetResult() const { return nullptr; }

const VecVR UopLla::GetOperands() const { return VecVR{}; }

VirtualRegister *UopLla::GetResult() const { return dst_; }

const VecVR UopLoad::GetOperands() const {
    if (base_ != nullptr) {
        return VecVR{base_};
    }
    return VecVR{};
}

VirtualRegister *UopLoad::GetResult() const { return dst_; }

const VecVR UopStore::GetOperands() const {
    VecVR vec;
    if (base_ != nullptr) {
        vec.push_back(base_);
    }
    if (src_ != nullptr) {
        vec.push_back(src_);
    }
    return vec;
}

VirtualRegister *UopStore::GetResult() const { return nullptr; }

const VecVR UopFLoad::GetOperands() const {
    if (base_ != nullptr) {
        return VecVR{base_};
    }
    return VecVR{};
}

VirtualRegister *UopFLoad::GetResult() const { return dst_; }

const VecVR UopFStore::GetOperands() const {
    VecVR vec;
    if (base_ != nullptr) {
        vec.push_back(base_);
    }
    if (src_ != nullptr) {
        vec.push_back(src_);
    }
    return vec;
}

VirtualRegister *UopFStore::GetResult() const { return nullptr; }

const VecVR UopFLoadLB::GetOperands() const {
    if (helper_ != nullptr) {
        return VecVR{helper_};
    }
    return VecVR{};
}

VirtualRegister *UopFLoadLB::GetResult() const { return dst_; }

const VecVR UopFCmp::GetOperands() const { return VecVR{lhs_, rhs_}; }

VirtualRegister *UopFCmp::GetResult() const { return dst_; }

const VecVR UopIBin::GetOperands() const {
    VecVR vec;
    if (lhs_ != nullptr) {
        vec.push_back(lhs_);
    }
    if (rhs_ != nullptr) {
        vec.push_back(rhs_);
    }
    return vec;
}

VirtualRegister *UopIBin::GetResult() const { return dst_; }

const VecVR UopIBin64::GetOperands() const {
    VecVR vec;
    if (lhs_ != nullptr) {
        vec.push_back(lhs_);
    }
    if (rhs_ != nullptr) {
        vec.push_back(rhs_);
    }
    return vec;
}

VirtualRegister *UopIBin64::GetResult() const { return dst_; }

const VecVR UopIBinImm::GetOperands() const {
    if (lhs_ != nullptr) {
        return VecVR{lhs_};
    }
    return VecVR{};
}

VirtualRegister *UopIBinImm::GetResult() const { return dst_; }

const VecVR UopIBinImm64::GetOperands() const {
    if (lhs_ != nullptr) {
        return VecVR{lhs_};
    }
    return VecVR{};
}

VirtualRegister *UopIBinImm64::GetResult() const { return dst_; }

const VecVR UopFBin::GetOperands() const {
    VecVR vec;
    if (lhs_ != nullptr) {
        vec.push_back(lhs_);
    }
    if (rhs_ != nullptr) {
        vec.push_back(rhs_);
    }
    return vec;
}

VirtualRegister *UopFBin::GetResult() const { return dst_; }

const VecVR UopFTri::GetOperands() const {
    VecVR vec;
    vec.push_back(lhs_);
    vec.push_back(rhs_);
    vec.push_back(ahs_);
    return vec;
}

VirtualRegister *UopFTri::GetResult() const { return dst_; }

const VecVR UopNop::GetOperands() const { return VecVR{}; }

VirtualRegister *UopNop::GetResult() const { return nullptr; }