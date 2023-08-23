#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"
#include "3tle3wa/backend/asm/AsmSchedItem.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLSchedItem.hh"
#include "3tle3wa/backend/scheduler/FIFO.hh"
#include "3tle3wa/backend/scheduler/SchedMachine.hh"
#include "3tle3wa/backend/scheduler/SiFiveU74.hh"
#include "3tle3wa/riscv/asm.hh"
#include "3tle3wa/utils/logs.hh"

void AsmProgress::DoOptimization() {
    for (auto &&abb : ablks_) {
        abb->Peepholes();
    }

    DoASchedule();
}

void AsmBasicBlock::Peepholes() {
    do {
        // for now only focus on inst pair

        auto nxt_it = insts_.begin();
        auto cur_it = nxt_it++;

        while (nxt_it != insts_.end()) {
            auto cur = cur_it->get();
            auto nxt = nxt_it->get();

            auto rv_cur = dynamic_cast<riscv::Instruction *>(cur);
            auto rv_nxt = dynamic_cast<riscv::Instruction *>(nxt);

            if (rv_cur == nullptr or rv_nxt == nullptr) {
                cur_it = nxt_it++;
                continue;
            }

            bool cur_redundant = false;
            bool nxt_redundant = false;

            if (auto c = dynamic_cast<riscv::LD *>(rv_cur); c != nullptr) {
                if (auto n = dynamic_cast<riscv::LD *>(rv_nxt); n != nullptr) {
                    cur_redundant = c->GetRd() == n->GetRd();
                    nxt_redundant = c->GetRd() == n->GetRd() and c->GetRs() == n->GetRs() and c->GetImm() == n->GetImm();
                } else if (auto n = dynamic_cast<riscv::SD *>(rv_nxt); n != nullptr) {
                    cur_redundant = false;
                    nxt_redundant = c->GetRd() == n->GetRs() and c->GetRs() == n->GetRt() and c->GetImm() == n->GetImm();
                }
            } else if (auto c = dynamic_cast<riscv::SD *>(rv_cur); c != nullptr) {
                if (auto n = dynamic_cast<riscv::LD *>(rv_nxt); n != nullptr) {
                    cur_redundant = false;
                    nxt_redundant = c->GetRs() == n->GetRd() and c->GetRt() == n->GetRs() and c->GetImm() == n->GetImm();
                } else if (auto n = dynamic_cast<riscv::SD *>(rv_nxt); n != nullptr) {
                    cur_redundant = c->GetRt() == n->GetRt() and c->GetImm() == n->GetImm();
                    nxt_redundant = false;
                }
            } else if (auto c = dynamic_cast<riscv::LW *>(rv_cur); c != nullptr) {
                if (auto n = dynamic_cast<riscv::LW *>(rv_nxt); n != nullptr) {
                    cur_redundant = c->GetRd() == n->GetRd();
                    nxt_redundant = c->GetRd() == n->GetRd() and c->GetRs() == n->GetRs() and c->GetImm() == n->GetImm();
                } else if (auto n = dynamic_cast<riscv::SW *>(rv_nxt); n != nullptr) {
                    cur_redundant = false;
                    nxt_redundant = c->GetRd() == n->GetRs() and c->GetRs() == n->GetRt() and c->GetImm() == n->GetImm();
                }
            } else if (auto c = dynamic_cast<riscv::SW *>(rv_cur); c != nullptr) {
                if (auto n = dynamic_cast<riscv::LW *>(rv_nxt); n != nullptr) {
                    cur_redundant = false;
                    nxt_redundant = c->GetRs() == n->GetRd() and c->GetRt() == n->GetRs() and c->GetImm() == n->GetImm();
                } else if (auto n = dynamic_cast<riscv::SW *>(rv_nxt); n != nullptr) {
                    cur_redundant = c->GetRt() == n->GetRt() and c->GetImm() == n->GetImm();
                    nxt_redundant = false;
                }
            } else if (auto c = dynamic_cast<riscv::FLW *>(rv_cur); c != nullptr) {
                if (auto n = dynamic_cast<riscv::FLW *>(rv_nxt); n != nullptr) {
                    cur_redundant = c->GetRd() == n->GetRd();
                    nxt_redundant = c->GetRd() == n->GetRd() and c->GetRs() == n->GetRs() and c->GetImm() == n->GetImm();
                } else if (auto n = dynamic_cast<riscv::FSW *>(rv_nxt); n != nullptr) {
                    cur_redundant = false;
                    nxt_redundant = c->GetRd() == n->GetRs() and c->GetRs() == n->GetRt() and c->GetImm() == n->GetImm();
                }
            } else if (auto c = dynamic_cast<riscv::FSW *>(rv_cur); c != nullptr) {
                if (auto n = dynamic_cast<riscv::FLW *>(rv_nxt); n != nullptr) {
                    cur_redundant = false;
                    nxt_redundant = c->GetRs() == n->GetRd() and c->GetRt() == n->GetRs() and c->GetImm() == n->GetImm();
                } else if (auto n = dynamic_cast<riscv::FSW *>(rv_nxt); n != nullptr) {
                    cur_redundant = c->GetRt() == n->GetRt() and c->GetImm() == n->GetImm();
                    nxt_redundant = false;
                }
            }

            if (nxt_redundant) {
                nxt_it = insts_.erase(nxt_it);
                continue;
            }

            if (cur_redundant) {
                cur_it = insts_.erase(cur_it);
                continue;
            }

            cur_it = nxt_it++;
        }
    } while (0);
}

void AsmProgress::DoASchedule() {
    for (auto &&abb : ablks_) {
        abb->ArchSchedule();
    }
}

void AsmBasicBlock::ArchSchedule() {
    do /* FIFO */ {
        SchedMachine mach(std::make_unique<SchedFIFO>());

        for (auto &&inst : insts_) {
            auto item = std::make_unique<AsmSchedItem>();
            item->inst = inst.get();

            mach.Push(std::move(item));
        }

        mach.Sched();

        auto fifo = dynamic_cast<SchedFIFO *>(mach.Policy());

        for (auto &&item : fifo->View()) {
            auto asmitem = dynamic_cast<AsmSchedItem *>(item);
            insts_view_.push_back(asmitem->inst);
        }
    } while (0);

    return;

    do /* schedule */ {
        SchedMachine mach(std::make_unique<SchedSiFiveU74>());

        for (auto &&inst : insts_) {
            auto item = std::make_unique<AsmSchedItem>();
            item->inst = inst.get();
            item->type = inst->GetSchedType();

            item->memop = (item->type == SCHED_TYPE::LOAD) or (item->type == SCHED_TYPE::STORE);

            if (auto res = inst->GetResult(); res != 0) {
                item->writes.push_back(res);
            }

            if (auto opds = inst->GetOperands(); not opds.empty()) {
                for (auto &&opd : opds) {
                    item->reads.push_back(opd);
                }
            }

            mach.Push(std::move(item));
        }

        insts_view_.clear();

        mach.Sched();

        auto sifiveu74 = dynamic_cast<SchedSiFiveU74 *>(mach.Policy());

        for (auto &&item : sifiveu74->View()) {
            auto asmitem = dynamic_cast<AsmSchedItem *>(item);
            insts_view_.push_back(asmitem->inst);
        }
    } while (0);
}