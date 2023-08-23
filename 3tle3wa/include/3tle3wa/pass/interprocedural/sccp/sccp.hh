#pragma once

#include <map>
#include <queue>

#include "3tle3wa/ir/IR.hh"

namespace SCCP {

struct Lattice {
    enum State {
        Undefine,  // Top
        Constant,  // Constant
        Variable,  // Bottom
    };
    State state;
    BaseValuePtr value;

    Lattice();
    Lattice(State, BaseValuePtr);

    inline bool IsUndefine() const;
    inline bool IsConstant() const;
    inline bool IsVariable() const;

    inline BaseValuePtr GetValue() const;

    bool operator!=(const Lattice &rhs);
};

static Lattice Undefine(Lattice::Undefine, nullptr);
static Lattice Variable(Lattice::Variable, nullptr);

inline Lattice CreateConstant(BaseValuePtr);

Lattice Meet(const Lattice &, const Lattice &);

static std::queue<CfgNodePtr> CFGWorkList;
static std::queue<InstPtr> SSAWorkList;

static std::map<CfgNodePtr, bool> ExcutedMap;
static std::map<BaseValuePtr, Lattice> LatticeMap;  // Constant may not in LatticeMap

Lattice GetLattice(BaseValuePtr);
void SetLattice(BaseValuePtr, Lattice &);

void EvaluateOnPhi(PhiInstPtr);
void EvaluateOnAssign(InstPtr);

void SCCP(NormalFuncPtr);

}  // namespace SCCP