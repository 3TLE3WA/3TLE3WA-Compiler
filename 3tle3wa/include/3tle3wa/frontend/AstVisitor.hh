#pragma once

#include <any>
#include <cstdio>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/function/structure/structure.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/pass/interprocedural/dce/dce.hh"
#include "3tle3wa/pass/intraprocedural/inline/inline.hh"
#include "3tle3wa/utils/logs.hh"
#include "SysYBaseVisitor.h"

using std::cout;
using std::endl;

class AstVisitor : public SysYBaseVisitor {
    // private variable for parsing AST and building llvm-IR
   private:
    CompilationUnit &comp_unit;

    bool have_main_func;

    bool in_loop;
    CfgNodePtr out_loop_block;

    VariablePtr ret_addr;
    CfgNodePtr ret_block;

    ATTR_TYPE cur_type;
    ATTR_POSITION cur_position;
    ATTR_POINTER ptr_or_not;

    CfgNodePtr cur_block;

    NormalFuncPtr cur_func;
    BaseFuncPtr callee_func;

    using SymTableList = std::list<SymbolTable *>;
    SymbolTable *cur_table;
    SymTableList table_list;

    using RetInstList = std::list<JumpInstPtr>;
    RetInstList return_list;

    CfgNodePtr target_continue;

    using BreakInstList = std::list<JumpInstPtr>;
    BreakInstList break_list;

    using BranchInstList = std::list<BranchInstPtr>;
    BranchInstList lOr_list;
    BranchInstList lAnd_list;

    using AddrTypeMap = std::unordered_map<BaseValuePtr, ListTypePtr>;
    AddrTypeMap addrTypeTable;

   public:
    AstVisitor(CompilationUnit &);
    ~AstVisitor() = default;

    virtual std::any visitCompilationUnit(SysYParser::CompilationUnitContext *ctx) override;
    // override anltr parsing function
   private:
    virtual std::any visitChildren(antlr4::tree::ParseTree *ctx) override;

    virtual std::any visitTranslationUnit(SysYParser::TranslationUnitContext *ctx) override;

    virtual std::any visitDecl(SysYParser::DeclContext *ctx) override;

    virtual std::any visitBType(SysYParser::BTypeContext *ctx) override;

    virtual std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override;

    virtual std::any visitConstDef(SysYParser::ConstDefContext *ctx) override;

    virtual std::any visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) override;

    virtual std::any visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) override;

    virtual std::any visitVarDecl(SysYParser::VarDeclContext *ctx) override;

    virtual std::any visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) override;

    virtual std::any visitInitVarDef(SysYParser::InitVarDefContext *ctx) override;

    virtual std::any visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) override;

    virtual std::any visitListInitval(SysYParser::ListInitvalContext *ctx) override;

    virtual std::any visitFuncDef(SysYParser::FuncDefContext *ctx) override;

    virtual std::any visitFuncType(SysYParser::FuncTypeContext *ctx) override;

    virtual std::any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override;

    virtual std::any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override;

    virtual std::any visitBlock(SysYParser::BlockContext *ctx) override;

    virtual std::any visitBlockItemList(SysYParser::BlockItemListContext *ctx) override;

    virtual std::any visitBlockItem(SysYParser::BlockItemContext *ctx) override;

    virtual std::any visitStmt(SysYParser::StmtContext *ctx) override;

    virtual std::any visitAssignStmt(SysYParser::AssignStmtContext *ctx) override;

    virtual std::any visitAssignOp(SysYParser::AssignOpContext *ctx) override;

    virtual std::any visitExpStmt(SysYParser::ExpStmtContext *ctx) override;

    virtual std::any visitBlockStmt(SysYParser::BlockStmtContext *ctx) override;

    virtual std::any visitIfStmt(SysYParser::IfStmtContext *ctx) override;

    virtual std::any visitWhileLoop(SysYParser::WhileLoopContext *ctx) override;

    virtual std::any visitContinueStmt(SysYParser::ContinueStmtContext *ctx) override;

    virtual std::any visitBreakStmt(SysYParser::BreakStmtContext *ctx) override;

    virtual std::any visitReturnStmt(SysYParser::ReturnStmtContext *ctx) override;

    virtual std::any visitExp(SysYParser::ExpContext *ctx) override;

    virtual std::any visitLVal(SysYParser::LValContext *ctx) override;

    virtual std::any visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) override;

    virtual std::any visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) override;

    virtual std::any visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) override;

    virtual std::any visitNumber1(SysYParser::Number1Context *ctx) override;

    virtual std::any visitNumber2(SysYParser::Number2Context *ctx) override;

    virtual std::any visitFuncRParams(SysYParser::FuncRParamsContext *ctx) override;

    virtual std::any visitFuncRParam(SysYParser::FuncRParamContext *ctx) override;

    virtual std::any visitUnary1(SysYParser::Unary1Context *ctx) override;

    virtual std::any visitUnary2(SysYParser::Unary2Context *ctx) override;

    virtual std::any visitUnary3(SysYParser::Unary3Context *ctx) override;

    virtual std::any visitUnaryOp(SysYParser::UnaryOpContext *ctx) override;

    virtual std::any visitMul1(SysYParser::Mul1Context *ctx) override;

    virtual std::any visitMul2(SysYParser::Mul2Context *ctx) override;

    virtual std::any visitMulOp(SysYParser::MulOpContext *ctx) override;

    virtual std::any visitAdd1(SysYParser::Add1Context *ctx) override;

    virtual std::any visitAdd2(SysYParser::Add2Context *ctx) override;

    virtual std::any visitAddOp(SysYParser::AddOpContext *ctx) override;

    virtual std::any visitRel1(SysYParser::Rel1Context *ctx) override;

    virtual std::any visitRel2(SysYParser::Rel2Context *ctx) override;

    virtual std::any visitRelOp(SysYParser::RelOpContext *ctx) override;

    virtual std::any visitEq1(SysYParser::Eq1Context *ctx) override;

    virtual std::any visitEq2(SysYParser::Eq2Context *ctx) override;

    virtual std::any visitEqOp(SysYParser::EqOpContext *ctx) override;

    virtual std::any visitLAnd1(SysYParser::LAnd1Context *ctx) override;

    virtual std::any visitLAnd2(SysYParser::LAnd2Context *ctx) override;

    virtual std::any visitLOr1(SysYParser::LOr1Context *ctx) override;

    virtual std::any visitLOr2(SysYParser::LOr2Context *ctx) override;

    virtual std::any visitCondExp(SysYParser::CondExpContext *ctx) override;

    virtual std::any visitConstExp(SysYParser::ConstExpContext *ctx) override;

    // support function to parse
    ArrDims GetArrayDims(std::vector<SysYParser::ConstExpContext *> &);

    SymbolTable *NewLocalTable(SymbolTable *);
    void ClearTableList();

    BaseValuePtr ResolveTable(std::string &name);

    SymbolTable *InitParamList(CfgNodePtr, SymbolTable *, std::vector<std::string>);

    void ParseLocalListInit(SysYParser::ListInitvalContext *, ListTypePtr, BaseValuePtr);
};
