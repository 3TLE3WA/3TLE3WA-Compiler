
// Generated from SysY.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  SysYLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    Int = 15, Void = 16, Const = 17, Return = 18, If = 19, Else = 20, While = 21, 
    For = 22, Do = 23, Break = 24, Continue = 25, Lparen = 26, Rparen = 27, 
    Lbrkt = 28, Rbrkt = 29, Lbrace = 30, Rbrace = 31, Comma = 32, Semicolon = 33, 
    Minus = 34, Addition = 35, Exclamation = 36, Multiplication = 37, Division = 38, 
    Modulo = 39, LAND = 40, LOR = 41, EQ = 42, NEQ = 43, LT = 44, LE = 45, 
    GT = 46, GE = 47, IntLiteral = 48, FloatLiteral = 49, Identifier = 50, 
    Whitespace = 51, Newline = 52, BlockComment = 53, LineComment = 54
  };

  explicit SysYLexer(antlr4::CharStream *input);

  ~SysYLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

