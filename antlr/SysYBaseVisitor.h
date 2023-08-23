
// Generated from SysY.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "SysYVisitor.h"


/**
 * This class provides an empty implementation of SysYVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SysYBaseVisitor : public SysYVisitor {
public:

  virtual std::any visitCompilationUnit(SysYParser::CompilationUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTranslationUnit(SysYParser::TranslationUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExternalDeclaration(SysYParser::ExternalDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDecl(SysYParser::DeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBType(SysYParser::BTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstDef(SysYParser::ConstDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDecl(SysYParser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitVarDef(SysYParser::InitVarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitListInitval(SysYParser::ListInitvalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDef(SysYParser::FuncDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncType(SysYParser::FuncTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(SysYParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItemList(SysYParser::BlockItemListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItem(SysYParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmt(SysYParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignStmt(SysYParser::AssignStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignOp(SysYParser::AssignOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpStmt(SysYParser::ExpStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockStmt(SysYParser::BlockStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStmt(SysYParser::IfStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileLoop(SysYParser::WhileLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContinueStmt(SysYParser::ContinueStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBreakStmt(SysYParser::BreakStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStmt(SysYParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExp(SysYParser::ExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstExp(SysYParser::ConstExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLVal(SysYParser::LValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber1(SysYParser::Number1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber2(SysYParser::Number2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncRParams(SysYParser::FuncRParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncRParam(SysYParser::FuncRParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary1(SysYParser::Unary1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary2(SysYParser::Unary2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary3(SysYParser::Unary3Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryOp(SysYParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMul2(SysYParser::Mul2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMul1(SysYParser::Mul1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulOp(SysYParser::MulOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdd2(SysYParser::Add2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdd1(SysYParser::Add1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddOp(SysYParser::AddOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRel2(SysYParser::Rel2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRel1(SysYParser::Rel1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelOp(SysYParser::RelOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEq1(SysYParser::Eq1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEq2(SysYParser::Eq2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqOp(SysYParser::EqOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLAnd2(SysYParser::LAnd2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLAnd1(SysYParser::LAnd1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLOr1(SysYParser::LOr1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLOr2(SysYParser::LOr2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondExp(SysYParser::CondExpContext *ctx) override {
    return visitChildren(ctx);
  }


};

