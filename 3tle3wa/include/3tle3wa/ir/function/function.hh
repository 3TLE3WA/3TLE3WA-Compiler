#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "3tle3wa/ir/function/basefunc.hh"
#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/branch.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/function/structure/structure.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/type/listType.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"

class NormalFunction;
using NormalFuncPtr = std::shared_ptr<NormalFunction>;
using BlockList = std::list<CfgNodePtr>;

class NormalFunction final : public BaseFunction {
   private:
    CfgNodePtr entry;
    CfgNodePtr exit;

    size_t var_idx;
    size_t blk_idx;

   public:
    Loop *loops;
    Branch_ *branch;
    struct SEtype {
        bool call_se_func;
        bool mod_param_arr;
        bool mod_glb_value;
    } se_type;

    NormalFunction(ScalarTypePtr, std::string &, ParamList &, bool);
    ~NormalFunction() = default;

    bool IsLibFunction() const;

    CfgNodePtr CreateEntry();
    CfgNodePtr CreateExit();
    CfgNodePtr CreateCfgNode(BlkAttr::BlkType blk_type = BlkAttr::Normal);

    CfgNodePtr GetEntryNode();
    CfgNodePtr GetExitNode();

    void SetEntryNode(CfgNodePtr);
    void SetExitNode(CfgNodePtr);

    CfgNodeList GetSequentialNodes();
    CfgNodeList GetReverseSeqNodes();

    void SetVarIdx(size_t);
    size_t GetVarIdx();

    void SetBlkIdx(size_t);
    size_t GetBlkIdx();

    static NormalFuncPtr CreatePtr(ScalarTypePtr, std::string &, ParamList &, bool);

    std::string tollvmIR();
};

class LibraryFunction;
using LibFuncPtr = std::shared_ptr<LibraryFunction>;

class LibraryFunction : public BaseFunction {
   public:
    LibraryFunction(ScalarTypePtr, std::string &, ParamList &, bool);
    ~LibraryFunction() = default;

    bool IsLibFunction() const;

    virtual bool IsSYSYLibFunction() const = 0;

    std::string tollvmIR();
};

class SYSYLibFunction;
using SYSYLibFuncPtr = std::shared_ptr<SYSYLibFunction>;

class SYSYLibFunction final : public LibraryFunction {
   public:
    SYSYLibFunction(ScalarTypePtr, std::string &, ParamList &, bool);
    ~SYSYLibFunction() = default;

    bool IsSYSYLibFunction() const;

    static SYSYLibFuncPtr CreatePtr(ScalarTypePtr, std::string, ParamList &, bool);
};

class LLVMLibFunction;
using LLVMLibFuncPtr = std::shared_ptr<LLVMLibFunction>;

class LLVMLibFunction final : public LibraryFunction {
   public:
    std::string proto_name;
    size_t proto_arg_nums;

    LLVMLibFunction(std::string &, size_t, ScalarTypePtr, std::string &, ParamList &, bool);
    ~LLVMLibFunction() = default;

    bool IsSYSYLibFunction() const;

    std::string &GetProtoName();
    size_t GetProtoArgNums() const;

    static LLVMLibFuncPtr CreatePtr(std::string, size_t, ScalarTypePtr, std::string, ParamList &, bool);
};

std::ostream &operator<<(std::ostream &, BaseFuncPtr);
