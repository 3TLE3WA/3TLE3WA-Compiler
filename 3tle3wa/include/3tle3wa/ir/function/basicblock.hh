#pragma once

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <list>
#include <memory>
#include <sstream>

#include "3tle3wa/ir/instruction/instruction.hh"

class BasicBlock;

using InstList = std::list<InstPtr>;
using BlockPtr = std::shared_ptr<BasicBlock>;

class BaseFunction;

struct Loop;

struct BlkAttr {
    typedef size_t BlkType;
    static const BlkType Normal = (1ul << 0x0000);
    static const BlkType Entry = (1ul << 0x0001);

    static const BlkType Break = (1ul << 0x000b);
    static const BlkType Continue = (1ul << 0x000c);

    static const BlkType GoReturn = (1ul << 0x000d);
    static const BlkType InlineGR = (1ul << 0x000e);

    static const BlkType Exit = (1ul << 0x000f);

    static const BlkType LoopTag = (1ul << 0x0010);
    static const BlkType BranchTag = (1ul << 0x00011);

    BlkType blk_type;

    bool before_blk;

    bool cond_begin;
    bool cond_end;

    bool body_begin;
    // bool body_end;

    bool iftrue_begin;
    bool iftrue_end;

    bool iffalse_begin;
    bool iffalse_end;

    bool structure_out;

    Loop *loop;

    BlkAttr(BlkType _type = Normal)
        : blk_type(_type),
          before_blk(false),
          cond_begin(false),
          cond_end(false),
          body_begin(false),
          //   body_end(false),
          iftrue_begin(false),
          iftrue_end(false),
          iffalse_begin(false),
          iffalse_end(false),
          structure_out(false),
          loop(nullptr) {}

    template <typename... BlkTypes>
    void AppBlkTypes(BlkType first, BlkTypes... rest) {
        blk_type |= first;
        for (auto &&follow : std::initializer_list<BlkType>{rest...}) {
            blk_type |= follow;
        }
    }

    template <typename... BlkTypes>
    void ClrBlkTypes(BlkType first, BlkTypes... rest) {
        blk_type &= (~first);
        for (auto &&follow : std::initializer_list<BlkType>{rest...}) {
            blk_type &= (~follow);
        }
    }

    void CombineBlkAttr(const BlkAttr &blk_attr) {
        blk_type |= blk_attr.blk_type;

        before_blk = before_blk | blk_attr.before_blk;
        cond_begin = cond_begin | blk_attr.cond_begin;
        cond_end = cond_end | blk_attr.cond_end;
        body_begin = body_begin | blk_attr.body_begin;
        iftrue_begin = iftrue_begin | blk_attr.iftrue_begin;
        iftrue_end = iftrue_end | blk_attr.iftrue_end;
        iffalse_begin = iffalse_begin | blk_attr.iffalse_begin;
        iffalse_end = iffalse_end | blk_attr.iffalse_end;
        structure_out = structure_out | blk_attr.structure_out;
        loop = nullptr;

        if (ChkAllOfBlkType(GoReturn, Exit)) {
            ClrBlkTypes(GoReturn);
        }
    }

    template <typename... BlkTypes>
    bool ChkOneOfBlkType(BlkType first, BlkTypes... rest) const {
        bool res = static_cast<bool>(blk_type & first);
        for (auto &&follow : std::initializer_list<BlkType>{rest...}) {
            res = res | static_cast<bool>(blk_type & follow);
        }
        return res;
    }

    template <typename... BlkTypes>
    bool ChkAllOfBlkType(BlkType first, BlkTypes... rest) const {
        bool res = static_cast<bool>(blk_type & first);
        for (auto &&follow : std::initializer_list<BlkType>{rest...}) {
            res = res & static_cast<bool>(blk_type & follow);
        }
        return res;
    }

    std::string to_str() {
        std::stringstream ss;
        if (ChkOneOfBlkType(BlkAttr::Entry)) ss << "Entry ";
        if (ChkOneOfBlkType(BlkAttr::Normal)) ss << "Normal ";
        if (ChkOneOfBlkType(BlkAttr::Break)) ss << "Break ";
        if (ChkOneOfBlkType(BlkAttr::Continue)) ss << "Continue ";
        if (ChkOneOfBlkType(BlkAttr::GoReturn)) ss << "GoReturn ";
        if (ChkOneOfBlkType(BlkAttr::InlineGR)) ss << "InlineGR ";
        if (ChkOneOfBlkType(BlkAttr::Exit)) ss << "Exit ";
        ss << "[ " << before_blk << ' ' << cond_begin << ' ' << cond_end << ' ' << body_begin
           << ' ' /*<< body_end << ' '*/
           << iftrue_begin << ' ' << iftrue_end << ' ' << iffalse_begin << ' ' << iffalse_end << ' ' << structure_out
           << " ]";
        return ss.str();
    }
};

class BasicBlock {
   protected:
    size_t idx;
    InstList inst_list;

    BaseFunction *parent;

    static size_t blk_idx;

   public:
    BlkAttr blk_attr;

    BasicBlock(BaseFunction *, BlkAttr::BlkType blk_tpye = BlkAttr::Normal);
    ~BasicBlock() = default;

    size_t GetBlockIdx();

    InstList &GetInstList();

    size_t GetInstCnt() const;

    InstPtr &GetLastInst();
    void RemoveLastInst();

    void InsertInstBack(InstPtr);
    void InsertInstFront(InstPtr);
    void RemoveInst(InstPtr);

    BaseFunction *GetParent();

    static void ResetBlkIdx();
    static size_t GetBlkIdx();
    static void SetBlkIdx(size_t);

    virtual std::string tollvmIR() = 0;
};
