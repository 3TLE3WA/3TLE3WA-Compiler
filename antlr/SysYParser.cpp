
// Generated from SysY.g4 by ANTLR 4.12.0

#include "SysYParser.h"

#include "SysYVisitor.h"

using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SysYParserStaticData final {
    SysYParserStaticData(std::vector<std::string> ruleNames, std::vector<std::string> literalNames,
                         std::vector<std::string> symbolicNames)
        : ruleNames(std::move(ruleNames)),
          literalNames(std::move(literalNames)),
          symbolicNames(std::move(symbolicNames)),
          vocabulary(this->literalNames, this->symbolicNames) {}

    SysYParserStaticData(const SysYParserStaticData &) = delete;
    SysYParserStaticData(SysYParserStaticData &&) = delete;
    SysYParserStaticData &operator=(const SysYParserStaticData &) = delete;
    SysYParserStaticData &operator=(SysYParserStaticData &&) = delete;

    std::vector<antlr4::dfa::DFA> decisionToDFA;
    antlr4::atn::PredictionContextCache sharedContextCache;
    const std::vector<std::string> ruleNames;
    const std::vector<std::string> literalNames;
    const std::vector<std::string> symbolicNames;
    const antlr4::dfa::Vocabulary vocabulary;
    antlr4::atn::SerializedATNView serializedATN;
    std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sysyParserOnceFlag;
SysYParserStaticData *sysyParserStaticData = nullptr;

void sysyParserInitialize() {
    assert(sysyParserStaticData == nullptr);
    auto staticData = std::make_unique<SysYParserStaticData>(
        std::vector<std::string>{"compilationUnit",
                                 "translationUnit",
                                 "externalDeclaration",
                                 "decl",
                                 "constDecl",
                                 "bType",
                                 "constDef",
                                 "constInitVal",
                                 "varDecl",
                                 "varDef",
                                 "initVal",
                                 "funcDef",
                                 "funcType",
                                 "funcFParams",
                                 "funcFParam",
                                 "block",
                                 "blockItemList",
                                 "blockItem",
                                 "stmt",
                                 "assignStmt",
                                 "assignOp",
                                 "expStmt",
                                 "blockStmt",
                                 "seleStmt",
                                 "loopStmt",
                                 "jumpStmt",
                                 "exp",
                                 "constExp",
                                 "lVal",
                                 "primaryExp",
                                 "number",
                                 "funcRParams",
                                 "funcRParam",
                                 "unaryExp",
                                 "unaryOp",
                                 "mulExp",
                                 "mulOp",
                                 "addExp",
                                 "addOp",
                                 "relExp",
                                 "relOp",
                                 "eqExp",
                                 "eqOp",
                                 "lAndExp",
                                 "lOrExp",
                                 "condExp"},
        std::vector<std::string>{"",        "'float'",    "'='",      "'*='", "'/='",   "'%='",    "'+='",  "'-='",
                                 "'<<='",   "'>>='",      "'&='",     "'^='", "'|='",   "'&'",     "'~'",   "'int'",
                                 "'void'",  "'const'",    "'return'", "'if'", "'else'", "'while'", "'for'", "'do'",
                                 "'break'", "'continue'", "'('",      "')'",  "'['",    "']'",     "'{'",   "'}'",
                                 "','",     "';'",        "'-'",      "'+'",  "'!'",    "'*'",     "'/'",   "'%'",
                                 "'&&'",    "'||'",       "'=='",     "'!='", "'<'",    "'<='",    "'>'",   "'>='"},
        std::vector<std::string>{"",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "",
                                 "Int",
                                 "Void",
                                 "Const",
                                 "Return",
                                 "If",
                                 "Else",
                                 "While",
                                 "For",
                                 "Do",
                                 "Break",
                                 "Continue",
                                 "Lparen",
                                 "Rparen",
                                 "Lbrkt",
                                 "Rbrkt",
                                 "Lbrace",
                                 "Rbrace",
                                 "Comma",
                                 "Semicolon",
                                 "Minus",
                                 "Addition",
                                 "Exclamation",
                                 "Multiplication",
                                 "Division",
                                 "Modulo",
                                 "LAND",
                                 "LOR",
                                 "EQ",
                                 "NEQ",
                                 "LT",
                                 "LE",
                                 "GT",
                                 "GE",
                                 "IntLiteral",
                                 "FloatLiteral",
                                 "Identifier",
                                 "Whitespace",
                                 "Newline",
                                 "BlockComment",
                                 "LineComment"});
    static const int32_t serializedATNSegment[] = {
        4,   1,   54,  431, 2,   0,   7,   0,   2,   1,   7,   1,   2,   2,   7,   2,   2,   3,   7,   3,   2,   4,
        7,   4,   2,   5,   7,   5,   2,   6,   7,   6,   2,   7,   7,   7,   2,   8,   7,   8,   2,   9,   7,   9,
        2,   10,  7,   10,  2,   11,  7,   11,  2,   12,  7,   12,  2,   13,  7,   13,  2,   14,  7,   14,  2,   15,
        7,   15,  2,   16,  7,   16,  2,   17,  7,   17,  2,   18,  7,   18,  2,   19,  7,   19,  2,   20,  7,   20,
        2,   21,  7,   21,  2,   22,  7,   22,  2,   23,  7,   23,  2,   24,  7,   24,  2,   25,  7,   25,  2,   26,
        7,   26,  2,   27,  7,   27,  2,   28,  7,   28,  2,   29,  7,   29,  2,   30,  7,   30,  2,   31,  7,   31,
        2,   32,  7,   32,  2,   33,  7,   33,  2,   34,  7,   34,  2,   35,  7,   35,  2,   36,  7,   36,  2,   37,
        7,   37,  2,   38,  7,   38,  2,   39,  7,   39,  2,   40,  7,   40,  2,   41,  7,   41,  2,   42,  7,   42,
        2,   43,  7,   43,  2,   44,  7,   44,  2,   45,  7,   45,  1,   0,   3,   0,   94,  8,   0,   1,   0,   1,
        0,   1,   1,   4,   1,   99,  8,   1,   11,  1,   12,  1,   100, 1,   2,   1,   2,   1,   2,   3,   2,   106,
        8,   2,   1,   3,   1,   3,   3,   3,   110, 8,   3,   1,   4,   1,   4,   1,   4,   1,   4,   1,   4,   5,
        4,   117, 8,   4,   10,  4,   12,  4,   120, 9,   4,   1,   4,   1,   4,   1,   5,   1,   5,   1,   6,   1,
        6,   1,   6,   1,   6,   1,   6,   5,   6,   131, 8,   6,   10,  6,   12,  6,   134, 9,   6,   1,   6,   1,
        6,   1,   6,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   5,   7,   144, 8,   7,   10,  7,   12,  7,
        147, 9,   7,   3,   7,   149, 8,   7,   1,   7,   3,   7,   152, 8,   7,   1,   8,   1,   8,   1,   8,   1,
        8,   5,   8,   158, 8,   8,   10,  8,   12,  8,   161, 9,   8,   1,   8,   1,   8,   1,   9,   1,   9,   1,
        9,   1,   9,   1,   9,   5,   9,   170, 8,   9,   10,  9,   12,  9,   173, 9,   9,   1,   9,   1,   9,   1,
        9,   1,   9,   1,   9,   5,   9,   180, 8,   9,   10,  9,   12,  9,   183, 9,   9,   1,   9,   1,   9,   3,
        9,   187, 8,   9,   1,   10,  1,   10,  1,   10,  1,   10,  1,   10,  5,   10,  194, 8,   10,  10,  10,  12,
        10,  197, 9,   10,  3,   10,  199, 8,   10,  1,   10,  3,   10,  202, 8,   10,  1,   11,  1,   11,  1,   11,
        1,   11,  3,   11,  208, 8,   11,  1,   11,  1,   11,  1,   11,  1,   12,  1,   12,  1,   13,  1,   13,  1,
        13,  5,   13,  218, 8,   13,  10,  13,  12,  13,  221, 9,   13,  1,   14,  1,   14,  1,   14,  1,   14,  1,
        14,  1,   14,  1,   14,  1,   14,  5,   14,  231, 8,   14,  10,  14,  12,  14,  234, 9,   14,  3,   14,  236,
        8,   14,  1,   15,  1,   15,  3,   15,  240, 8,   15,  1,   15,  1,   15,  1,   16,  4,   16,  245, 8,   16,
        11,  16,  12,  16,  246, 1,   17,  1,   17,  3,   17,  251, 8,   17,  1,   18,  1,   18,  1,   18,  1,   18,
        1,   18,  1,   18,  3,   18,  259, 8,   18,  1,   19,  1,   19,  1,   19,  1,   19,  1,   19,  1,   20,  1,
        20,  1,   21,  3,   21,  269, 8,   21,  1,   21,  1,   21,  1,   22,  1,   22,  1,   23,  1,   23,  1,   23,
        1,   23,  1,   23,  1,   23,  1,   23,  3,   23,  282, 8,   23,  1,   24,  1,   24,  1,   24,  1,   24,  1,
        24,  1,   24,  1,   25,  1,   25,  1,   25,  1,   25,  1,   25,  1,   25,  3,   25,  296, 8,   25,  1,   25,
        3,   25,  299, 8,   25,  1,   26,  1,   26,  1,   27,  1,   27,  1,   28,  1,   28,  1,   28,  1,   28,  1,
        28,  5,   28,  310, 8,   28,  10,  28,  12,  28,  313, 9,   28,  1,   29,  1,   29,  1,   29,  1,   29,  1,
        29,  1,   29,  3,   29,  321, 8,   29,  1,   30,  1,   30,  3,   30,  325, 8,   30,  1,   31,  1,   31,  1,
        31,  5,   31,  330, 8,   31,  10,  31,  12,  31,  333, 9,   31,  1,   32,  1,   32,  1,   33,  1,   33,  1,
        33,  1,   33,  3,   33,  341, 8,   33,  1,   33,  1,   33,  1,   33,  1,   33,  3,   33,  347, 8,   33,  1,
        34,  1,   34,  1,   35,  1,   35,  1,   35,  1,   35,  1,   35,  1,   35,  1,   35,  5,   35,  358, 8,   35,
        10,  35,  12,  35,  361, 9,   35,  1,   36,  1,   36,  1,   37,  1,   37,  1,   37,  1,   37,  1,   37,  1,
        37,  1,   37,  5,   37,  372, 8,   37,  10,  37,  12,  37,  375, 9,   37,  1,   38,  1,   38,  1,   39,  1,
        39,  1,   39,  1,   39,  1,   39,  1,   39,  1,   39,  5,   39,  386, 8,   39,  10,  39,  12,  39,  389, 9,
        39,  1,   40,  1,   40,  1,   41,  1,   41,  1,   41,  1,   41,  1,   41,  1,   41,  1,   41,  5,   41,  400,
        8,   41,  10,  41,  12,  41,  403, 9,   41,  1,   42,  1,   42,  1,   43,  1,   43,  1,   43,  1,   43,  1,
        43,  1,   43,  5,   43,  413, 8,   43,  10,  43,  12,  43,  416, 9,   43,  1,   44,  1,   44,  1,   44,  1,
        44,  1,   44,  1,   44,  5,   44,  424, 8,   44,  10,  44,  12,  44,  427, 9,   44,  1,   45,  1,   45,  1,
        45,  0,   6,   70,  74,  78,  82,  86,  88,  46,  0,   2,   4,   6,   8,   10,  12,  14,  16,  18,  20,  22,
        24,  26,  28,  30,  32,  34,  36,  38,  40,  42,  44,  46,  48,  50,  52,  54,  56,  58,  60,  62,  64,  66,
        68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  0,   8,   2,   0,   1,   1,   15,  15,  2,   0,
        1,   1,   15,  16,  1,   0,   2,   12,  2,   0,   13,  14,  34,  37,  1,   0,   37,  39,  1,   0,   34,  35,
        1,   0,   44,  47,  1,   0,   42,  43,  432, 0,   93,  1,   0,   0,   0,   2,   98,  1,   0,   0,   0,   4,
        105, 1,   0,   0,   0,   6,   109, 1,   0,   0,   0,   8,   111, 1,   0,   0,   0,   10,  123, 1,   0,   0,
        0,   12,  125, 1,   0,   0,   0,   14,  151, 1,   0,   0,   0,   16,  153, 1,   0,   0,   0,   18,  186, 1,
        0,   0,   0,   20,  201, 1,   0,   0,   0,   22,  203, 1,   0,   0,   0,   24,  212, 1,   0,   0,   0,   26,
        214, 1,   0,   0,   0,   28,  222, 1,   0,   0,   0,   30,  237, 1,   0,   0,   0,   32,  244, 1,   0,   0,
        0,   34,  250, 1,   0,   0,   0,   36,  258, 1,   0,   0,   0,   38,  260, 1,   0,   0,   0,   40,  265, 1,
        0,   0,   0,   42,  268, 1,   0,   0,   0,   44,  272, 1,   0,   0,   0,   46,  274, 1,   0,   0,   0,   48,
        283, 1,   0,   0,   0,   50,  298, 1,   0,   0,   0,   52,  300, 1,   0,   0,   0,   54,  302, 1,   0,   0,
        0,   56,  304, 1,   0,   0,   0,   58,  320, 1,   0,   0,   0,   60,  324, 1,   0,   0,   0,   62,  326, 1,
        0,   0,   0,   64,  334, 1,   0,   0,   0,   66,  346, 1,   0,   0,   0,   68,  348, 1,   0,   0,   0,   70,
        350, 1,   0,   0,   0,   72,  362, 1,   0,   0,   0,   74,  364, 1,   0,   0,   0,   76,  376, 1,   0,   0,
        0,   78,  378, 1,   0,   0,   0,   80,  390, 1,   0,   0,   0,   82,  392, 1,   0,   0,   0,   84,  404, 1,
        0,   0,   0,   86,  406, 1,   0,   0,   0,   88,  417, 1,   0,   0,   0,   90,  428, 1,   0,   0,   0,   92,
        94,  3,   2,   1,   0,   93,  92,  1,   0,   0,   0,   93,  94,  1,   0,   0,   0,   94,  95,  1,   0,   0,
        0,   95,  96,  5,   0,   0,   1,   96,  1,   1,   0,   0,   0,   97,  99,  3,   4,   2,   0,   98,  97,  1,
        0,   0,   0,   99,  100, 1,   0,   0,   0,   100, 98,  1,   0,   0,   0,   100, 101, 1,   0,   0,   0,   101,
        3,   1,   0,   0,   0,   102, 106, 3,   6,   3,   0,   103, 106, 3,   22,  11,  0,   104, 106, 5,   33,  0,
        0,   105, 102, 1,   0,   0,   0,   105, 103, 1,   0,   0,   0,   105, 104, 1,   0,   0,   0,   106, 5,   1,
        0,   0,   0,   107, 110, 3,   8,   4,   0,   108, 110, 3,   16,  8,   0,   109, 107, 1,   0,   0,   0,   109,
        108, 1,   0,   0,   0,   110, 7,   1,   0,   0,   0,   111, 112, 5,   17,  0,   0,   112, 113, 3,   10,  5,
        0,   113, 118, 3,   12,  6,   0,   114, 115, 5,   32,  0,   0,   115, 117, 3,   12,  6,   0,   116, 114, 1,
        0,   0,   0,   117, 120, 1,   0,   0,   0,   118, 116, 1,   0,   0,   0,   118, 119, 1,   0,   0,   0,   119,
        121, 1,   0,   0,   0,   120, 118, 1,   0,   0,   0,   121, 122, 5,   33,  0,   0,   122, 9,   1,   0,   0,
        0,   123, 124, 7,   0,   0,   0,   124, 11,  1,   0,   0,   0,   125, 132, 5,   50,  0,   0,   126, 127, 5,
        28,  0,   0,   127, 128, 3,   54,  27,  0,   128, 129, 5,   29,  0,   0,   129, 131, 1,   0,   0,   0,   130,
        126, 1,   0,   0,   0,   131, 134, 1,   0,   0,   0,   132, 130, 1,   0,   0,   0,   132, 133, 1,   0,   0,
        0,   133, 135, 1,   0,   0,   0,   134, 132, 1,   0,   0,   0,   135, 136, 5,   2,   0,   0,   136, 137, 3,
        14,  7,   0,   137, 13,  1,   0,   0,   0,   138, 152, 3,   54,  27,  0,   139, 148, 5,   30,  0,   0,   140,
        145, 3,   14,  7,   0,   141, 142, 5,   32,  0,   0,   142, 144, 3,   14,  7,   0,   143, 141, 1,   0,   0,
        0,   144, 147, 1,   0,   0,   0,   145, 143, 1,   0,   0,   0,   145, 146, 1,   0,   0,   0,   146, 149, 1,
        0,   0,   0,   147, 145, 1,   0,   0,   0,   148, 140, 1,   0,   0,   0,   148, 149, 1,   0,   0,   0,   149,
        150, 1,   0,   0,   0,   150, 152, 5,   31,  0,   0,   151, 138, 1,   0,   0,   0,   151, 139, 1,   0,   0,
        0,   152, 15,  1,   0,   0,   0,   153, 154, 3,   10,  5,   0,   154, 159, 3,   18,  9,   0,   155, 156, 5,
        32,  0,   0,   156, 158, 3,   18,  9,   0,   157, 155, 1,   0,   0,   0,   158, 161, 1,   0,   0,   0,   159,
        157, 1,   0,   0,   0,   159, 160, 1,   0,   0,   0,   160, 162, 1,   0,   0,   0,   161, 159, 1,   0,   0,
        0,   162, 163, 5,   33,  0,   0,   163, 17,  1,   0,   0,   0,   164, 171, 5,   50,  0,   0,   165, 166, 5,
        28,  0,   0,   166, 167, 3,   54,  27,  0,   167, 168, 5,   29,  0,   0,   168, 170, 1,   0,   0,   0,   169,
        165, 1,   0,   0,   0,   170, 173, 1,   0,   0,   0,   171, 169, 1,   0,   0,   0,   171, 172, 1,   0,   0,
        0,   172, 187, 1,   0,   0,   0,   173, 171, 1,   0,   0,   0,   174, 181, 5,   50,  0,   0,   175, 176, 5,
        28,  0,   0,   176, 177, 3,   54,  27,  0,   177, 178, 5,   29,  0,   0,   178, 180, 1,   0,   0,   0,   179,
        175, 1,   0,   0,   0,   180, 183, 1,   0,   0,   0,   181, 179, 1,   0,   0,   0,   181, 182, 1,   0,   0,
        0,   182, 184, 1,   0,   0,   0,   183, 181, 1,   0,   0,   0,   184, 185, 5,   2,   0,   0,   185, 187, 3,
        20,  10,  0,   186, 164, 1,   0,   0,   0,   186, 174, 1,   0,   0,   0,   187, 19,  1,   0,   0,   0,   188,
        202, 3,   52,  26,  0,   189, 198, 5,   30,  0,   0,   190, 195, 3,   20,  10,  0,   191, 192, 5,   32,  0,
        0,   192, 194, 3,   20,  10,  0,   193, 191, 1,   0,   0,   0,   194, 197, 1,   0,   0,   0,   195, 193, 1,
        0,   0,   0,   195, 196, 1,   0,   0,   0,   196, 199, 1,   0,   0,   0,   197, 195, 1,   0,   0,   0,   198,
        190, 1,   0,   0,   0,   198, 199, 1,   0,   0,   0,   199, 200, 1,   0,   0,   0,   200, 202, 5,   31,  0,
        0,   201, 188, 1,   0,   0,   0,   201, 189, 1,   0,   0,   0,   202, 21,  1,   0,   0,   0,   203, 204, 3,
        24,  12,  0,   204, 205, 5,   50,  0,   0,   205, 207, 5,   26,  0,   0,   206, 208, 3,   26,  13,  0,   207,
        206, 1,   0,   0,   0,   207, 208, 1,   0,   0,   0,   208, 209, 1,   0,   0,   0,   209, 210, 5,   27,  0,
        0,   210, 211, 3,   30,  15,  0,   211, 23,  1,   0,   0,   0,   212, 213, 7,   1,   0,   0,   213, 25,  1,
        0,   0,   0,   214, 219, 3,   28,  14,  0,   215, 216, 5,   32,  0,   0,   216, 218, 3,   28,  14,  0,   217,
        215, 1,   0,   0,   0,   218, 221, 1,   0,   0,   0,   219, 217, 1,   0,   0,   0,   219, 220, 1,   0,   0,
        0,   220, 27,  1,   0,   0,   0,   221, 219, 1,   0,   0,   0,   222, 223, 3,   10,  5,   0,   223, 235, 5,
        50,  0,   0,   224, 225, 5,   28,  0,   0,   225, 232, 5,   29,  0,   0,   226, 227, 5,   28,  0,   0,   227,
        228, 3,   54,  27,  0,   228, 229, 5,   29,  0,   0,   229, 231, 1,   0,   0,   0,   230, 226, 1,   0,   0,
        0,   231, 234, 1,   0,   0,   0,   232, 230, 1,   0,   0,   0,   232, 233, 1,   0,   0,   0,   233, 236, 1,
        0,   0,   0,   234, 232, 1,   0,   0,   0,   235, 224, 1,   0,   0,   0,   235, 236, 1,   0,   0,   0,   236,
        29,  1,   0,   0,   0,   237, 239, 5,   30,  0,   0,   238, 240, 3,   32,  16,  0,   239, 238, 1,   0,   0,
        0,   239, 240, 1,   0,   0,   0,   240, 241, 1,   0,   0,   0,   241, 242, 5,   31,  0,   0,   242, 31,  1,
        0,   0,   0,   243, 245, 3,   34,  17,  0,   244, 243, 1,   0,   0,   0,   245, 246, 1,   0,   0,   0,   246,
        244, 1,   0,   0,   0,   246, 247, 1,   0,   0,   0,   247, 33,  1,   0,   0,   0,   248, 251, 3,   6,   3,
        0,   249, 251, 3,   36,  18,  0,   250, 248, 1,   0,   0,   0,   250, 249, 1,   0,   0,   0,   251, 35,  1,
        0,   0,   0,   252, 259, 3,   38,  19,  0,   253, 259, 3,   42,  21,  0,   254, 259, 3,   44,  22,  0,   255,
        259, 3,   46,  23,  0,   256, 259, 3,   48,  24,  0,   257, 259, 3,   50,  25,  0,   258, 252, 1,   0,   0,
        0,   258, 253, 1,   0,   0,   0,   258, 254, 1,   0,   0,   0,   258, 255, 1,   0,   0,   0,   258, 256, 1,
        0,   0,   0,   258, 257, 1,   0,   0,   0,   259, 37,  1,   0,   0,   0,   260, 261, 3,   56,  28,  0,   261,
        262, 3,   40,  20,  0,   262, 263, 3,   52,  26,  0,   263, 264, 5,   33,  0,   0,   264, 39,  1,   0,   0,
        0,   265, 266, 7,   2,   0,   0,   266, 41,  1,   0,   0,   0,   267, 269, 3,   52,  26,  0,   268, 267, 1,
        0,   0,   0,   268, 269, 1,   0,   0,   0,   269, 270, 1,   0,   0,   0,   270, 271, 5,   33,  0,   0,   271,
        43,  1,   0,   0,   0,   272, 273, 3,   30,  15,  0,   273, 45,  1,   0,   0,   0,   274, 275, 5,   19,  0,
        0,   275, 276, 5,   26,  0,   0,   276, 277, 3,   90,  45,  0,   277, 278, 5,   27,  0,   0,   278, 281, 3,
        36,  18,  0,   279, 280, 5,   20,  0,   0,   280, 282, 3,   36,  18,  0,   281, 279, 1,   0,   0,   0,   281,
        282, 1,   0,   0,   0,   282, 47,  1,   0,   0,   0,   283, 284, 5,   21,  0,   0,   284, 285, 5,   26,  0,
        0,   285, 286, 3,   90,  45,  0,   286, 287, 5,   27,  0,   0,   287, 288, 3,   36,  18,  0,   288, 49,  1,
        0,   0,   0,   289, 290, 5,   25,  0,   0,   290, 299, 5,   33,  0,   0,   291, 292, 5,   24,  0,   0,   292,
        299, 5,   33,  0,   0,   293, 295, 5,   18,  0,   0,   294, 296, 3,   52,  26,  0,   295, 294, 1,   0,   0,
        0,   295, 296, 1,   0,   0,   0,   296, 297, 1,   0,   0,   0,   297, 299, 5,   33,  0,   0,   298, 289, 1,
        0,   0,   0,   298, 291, 1,   0,   0,   0,   298, 293, 1,   0,   0,   0,   299, 51,  1,   0,   0,   0,   300,
        301, 3,   74,  37,  0,   301, 53,  1,   0,   0,   0,   302, 303, 3,   74,  37,  0,   303, 55,  1,   0,   0,
        0,   304, 311, 5,   50,  0,   0,   305, 306, 5,   28,  0,   0,   306, 307, 3,   52,  26,  0,   307, 308, 5,
        29,  0,   0,   308, 310, 1,   0,   0,   0,   309, 305, 1,   0,   0,   0,   310, 313, 1,   0,   0,   0,   311,
        309, 1,   0,   0,   0,   311, 312, 1,   0,   0,   0,   312, 57,  1,   0,   0,   0,   313, 311, 1,   0,   0,
        0,   314, 315, 5,   26,  0,   0,   315, 316, 3,   52,  26,  0,   316, 317, 5,   27,  0,   0,   317, 321, 1,
        0,   0,   0,   318, 321, 3,   56,  28,  0,   319, 321, 3,   60,  30,  0,   320, 314, 1,   0,   0,   0,   320,
        318, 1,   0,   0,   0,   320, 319, 1,   0,   0,   0,   321, 59,  1,   0,   0,   0,   322, 325, 5,   48,  0,
        0,   323, 325, 5,   49,  0,   0,   324, 322, 1,   0,   0,   0,   324, 323, 1,   0,   0,   0,   325, 61,  1,
        0,   0,   0,   326, 331, 3,   64,  32,  0,   327, 328, 5,   32,  0,   0,   328, 330, 3,   64,  32,  0,   329,
        327, 1,   0,   0,   0,   330, 333, 1,   0,   0,   0,   331, 329, 1,   0,   0,   0,   331, 332, 1,   0,   0,
        0,   332, 63,  1,   0,   0,   0,   333, 331, 1,   0,   0,   0,   334, 335, 3,   52,  26,  0,   335, 65,  1,
        0,   0,   0,   336, 347, 3,   58,  29,  0,   337, 338, 5,   50,  0,   0,   338, 340, 5,   26,  0,   0,   339,
        341, 3,   62,  31,  0,   340, 339, 1,   0,   0,   0,   340, 341, 1,   0,   0,   0,   341, 342, 1,   0,   0,
        0,   342, 347, 5,   27,  0,   0,   343, 344, 3,   68,  34,  0,   344, 345, 3,   66,  33,  0,   345, 347, 1,
        0,   0,   0,   346, 336, 1,   0,   0,   0,   346, 337, 1,   0,   0,   0,   346, 343, 1,   0,   0,   0,   347,
        67,  1,   0,   0,   0,   348, 349, 7,   3,   0,   0,   349, 69,  1,   0,   0,   0,   350, 351, 6,   35,  -1,
        0,   351, 352, 3,   66,  33,  0,   352, 359, 1,   0,   0,   0,   353, 354, 10,  1,   0,   0,   354, 355, 3,
        72,  36,  0,   355, 356, 3,   66,  33,  0,   356, 358, 1,   0,   0,   0,   357, 353, 1,   0,   0,   0,   358,
        361, 1,   0,   0,   0,   359, 357, 1,   0,   0,   0,   359, 360, 1,   0,   0,   0,   360, 71,  1,   0,   0,
        0,   361, 359, 1,   0,   0,   0,   362, 363, 7,   4,   0,   0,   363, 73,  1,   0,   0,   0,   364, 365, 6,
        37,  -1,  0,   365, 366, 3,   70,  35,  0,   366, 373, 1,   0,   0,   0,   367, 368, 10,  1,   0,   0,   368,
        369, 3,   76,  38,  0,   369, 370, 3,   70,  35,  0,   370, 372, 1,   0,   0,   0,   371, 367, 1,   0,   0,
        0,   372, 375, 1,   0,   0,   0,   373, 371, 1,   0,   0,   0,   373, 374, 1,   0,   0,   0,   374, 75,  1,
        0,   0,   0,   375, 373, 1,   0,   0,   0,   376, 377, 7,   5,   0,   0,   377, 77,  1,   0,   0,   0,   378,
        379, 6,   39,  -1,  0,   379, 380, 3,   74,  37,  0,   380, 387, 1,   0,   0,   0,   381, 382, 10,  1,   0,
        0,   382, 383, 3,   80,  40,  0,   383, 384, 3,   74,  37,  0,   384, 386, 1,   0,   0,   0,   385, 381, 1,
        0,   0,   0,   386, 389, 1,   0,   0,   0,   387, 385, 1,   0,   0,   0,   387, 388, 1,   0,   0,   0,   388,
        79,  1,   0,   0,   0,   389, 387, 1,   0,   0,   0,   390, 391, 7,   6,   0,   0,   391, 81,  1,   0,   0,
        0,   392, 393, 6,   41,  -1,  0,   393, 394, 3,   78,  39,  0,   394, 401, 1,   0,   0,   0,   395, 396, 10,
        1,   0,   0,   396, 397, 3,   84,  42,  0,   397, 398, 3,   78,  39,  0,   398, 400, 1,   0,   0,   0,   399,
        395, 1,   0,   0,   0,   400, 403, 1,   0,   0,   0,   401, 399, 1,   0,   0,   0,   401, 402, 1,   0,   0,
        0,   402, 83,  1,   0,   0,   0,   403, 401, 1,   0,   0,   0,   404, 405, 7,   7,   0,   0,   405, 85,  1,
        0,   0,   0,   406, 407, 6,   43,  -1,  0,   407, 408, 3,   82,  41,  0,   408, 414, 1,   0,   0,   0,   409,
        410, 10,  1,   0,   0,   410, 411, 5,   40,  0,   0,   411, 413, 3,   82,  41,  0,   412, 409, 1,   0,   0,
        0,   413, 416, 1,   0,   0,   0,   414, 412, 1,   0,   0,   0,   414, 415, 1,   0,   0,   0,   415, 87,  1,
        0,   0,   0,   416, 414, 1,   0,   0,   0,   417, 418, 6,   44,  -1,  0,   418, 419, 3,   86,  43,  0,   419,
        425, 1,   0,   0,   0,   420, 421, 10,  1,   0,   0,   421, 422, 5,   41,  0,   0,   422, 424, 3,   86,  43,
        0,   423, 420, 1,   0,   0,   0,   424, 427, 1,   0,   0,   0,   425, 423, 1,   0,   0,   0,   425, 426, 1,
        0,   0,   0,   426, 89,  1,   0,   0,   0,   427, 425, 1,   0,   0,   0,   428, 429, 3,   88,  44,  0,   429,
        91,  1,   0,   0,   0,   40,  93,  100, 105, 109, 118, 132, 145, 148, 151, 159, 171, 181, 186, 195, 198, 201,
        207, 219, 232, 235, 239, 246, 250, 258, 268, 281, 295, 298, 311, 320, 324, 331, 340, 346, 359, 373, 387, 401,
        414, 425};
    staticData->serializedATN = antlr4::atn::SerializedATNView(
        serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

    antlr4::atn::ATNDeserializer deserializer;
    staticData->atn = deserializer.deserialize(staticData->serializedATN);

    const size_t count = staticData->atn->getNumberOfDecisions();
    staticData->decisionToDFA.reserve(count);
    for (size_t i = 0; i < count; i++) {
        staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
    }
    sysyParserStaticData = staticData.release();
}

}  // namespace

SysYParser::SysYParser(TokenStream *input) : SysYParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SysYParser::SysYParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
    SysYParser::initialize();
    _interpreter = new atn::ParserATNSimulator(this, *sysyParserStaticData->atn, sysyParserStaticData->decisionToDFA,
                                               sysyParserStaticData->sharedContextCache, options);
}

SysYParser::~SysYParser() { delete _interpreter; }

const atn::ATN &SysYParser::getATN() const { return *sysyParserStaticData->atn; }

std::string SysYParser::getGrammarFileName() const { return "SysY.g4"; }

const std::vector<std::string> &SysYParser::getRuleNames() const { return sysyParserStaticData->ruleNames; }

const dfa::Vocabulary &SysYParser::getVocabulary() const { return sysyParserStaticData->vocabulary; }

antlr4::atn::SerializedATNView SysYParser::getSerializedATN() const { return sysyParserStaticData->serializedATN; }

//----------------- CompilationUnitContext ------------------------------------------------------------------

SysYParser::CompilationUnitContext::CompilationUnitContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::CompilationUnitContext::EOF() { return getToken(SysYParser::EOF, 0); }

SysYParser::TranslationUnitContext *SysYParser::CompilationUnitContext::translationUnit() {
    return getRuleContext<SysYParser::TranslationUnitContext>(0);
}

size_t SysYParser::CompilationUnitContext::getRuleIndex() const { return SysYParser::RuleCompilationUnit; }

std::any SysYParser::CompilationUnitContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitCompilationUnit(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::CompilationUnitContext *SysYParser::compilationUnit() {
    CompilationUnitContext *_localctx = _tracker.createInstance<CompilationUnitContext>(_ctx, getState());
    enterRule(_localctx, 0, SysYParser::RuleCompilationUnit);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(93);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 8590163970) != 0)) {
            setState(92);
            translationUnit();
        }
        setState(95);
        match(SysYParser::EOF);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- TranslationUnitContext ------------------------------------------------------------------

SysYParser::TranslationUnitContext::TranslationUnitContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<SysYParser::ExternalDeclarationContext *> SysYParser::TranslationUnitContext::externalDeclaration() {
    return getRuleContexts<SysYParser::ExternalDeclarationContext>();
}

SysYParser::ExternalDeclarationContext *SysYParser::TranslationUnitContext::externalDeclaration(size_t i) {
    return getRuleContext<SysYParser::ExternalDeclarationContext>(i);
}

size_t SysYParser::TranslationUnitContext::getRuleIndex() const { return SysYParser::RuleTranslationUnit; }

std::any SysYParser::TranslationUnitContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitTranslationUnit(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::TranslationUnitContext *SysYParser::translationUnit() {
    TranslationUnitContext *_localctx = _tracker.createInstance<TranslationUnitContext>(_ctx, getState());
    enterRule(_localctx, 2, SysYParser::RuleTranslationUnit);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(98);
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
            setState(97);
            externalDeclaration();
            setState(100);
            _errHandler->sync(this);
            _la = _input->LA(1);
        } while ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 8590163970) != 0));

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExternalDeclarationContext ------------------------------------------------------------------

SysYParser::ExternalDeclarationContext::ExternalDeclarationContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::DeclContext *SysYParser::ExternalDeclarationContext::decl() {
    return getRuleContext<SysYParser::DeclContext>(0);
}

SysYParser::FuncDefContext *SysYParser::ExternalDeclarationContext::funcDef() {
    return getRuleContext<SysYParser::FuncDefContext>(0);
}

tree::TerminalNode *SysYParser::ExternalDeclarationContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

size_t SysYParser::ExternalDeclarationContext::getRuleIndex() const { return SysYParser::RuleExternalDeclaration; }

std::any SysYParser::ExternalDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitExternalDeclaration(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::ExternalDeclarationContext *SysYParser::externalDeclaration() {
    ExternalDeclarationContext *_localctx = _tracker.createInstance<ExternalDeclarationContext>(_ctx, getState());
    enterRule(_localctx, 4, SysYParser::RuleExternalDeclaration);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(105);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(102);
                decl();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(103);
                funcDef();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(104);
                match(SysYParser::Semicolon);
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

SysYParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::ConstDeclContext *SysYParser::DeclContext::constDecl() {
    return getRuleContext<SysYParser::ConstDeclContext>(0);
}

SysYParser::VarDeclContext *SysYParser::DeclContext::varDecl() { return getRuleContext<SysYParser::VarDeclContext>(0); }

size_t SysYParser::DeclContext::getRuleIndex() const { return SysYParser::RuleDecl; }

std::any SysYParser::DeclContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitDecl(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::DeclContext *SysYParser::decl() {
    DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
    enterRule(_localctx, 6, SysYParser::RuleDecl);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(109);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::Const: {
                enterOuterAlt(_localctx, 1);
                setState(107);
                constDecl();
                break;
            }

            case SysYParser::T__0:
            case SysYParser::Int: {
                enterOuterAlt(_localctx, 2);
                setState(108);
                varDecl();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstDeclContext ------------------------------------------------------------------

SysYParser::ConstDeclContext::ConstDeclContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::ConstDeclContext::Const() { return getToken(SysYParser::Const, 0); }

SysYParser::BTypeContext *SysYParser::ConstDeclContext::bType() { return getRuleContext<SysYParser::BTypeContext>(0); }

std::vector<SysYParser::ConstDefContext *> SysYParser::ConstDeclContext::constDef() {
    return getRuleContexts<SysYParser::ConstDefContext>();
}

SysYParser::ConstDefContext *SysYParser::ConstDeclContext::constDef(size_t i) {
    return getRuleContext<SysYParser::ConstDefContext>(i);
}

tree::TerminalNode *SysYParser::ConstDeclContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

std::vector<tree::TerminalNode *> SysYParser::ConstDeclContext::Comma() { return getTokens(SysYParser::Comma); }

tree::TerminalNode *SysYParser::ConstDeclContext::Comma(size_t i) { return getToken(SysYParser::Comma, i); }

size_t SysYParser::ConstDeclContext::getRuleIndex() const { return SysYParser::RuleConstDecl; }

std::any SysYParser::ConstDeclContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitConstDecl(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::ConstDeclContext *SysYParser::constDecl() {
    ConstDeclContext *_localctx = _tracker.createInstance<ConstDeclContext>(_ctx, getState());
    enterRule(_localctx, 8, SysYParser::RuleConstDecl);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(111);
        match(SysYParser::Const);
        setState(112);
        bType();
        setState(113);
        constDef();
        setState(118);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::Comma) {
            setState(114);
            match(SysYParser::Comma);
            setState(115);
            constDef();
            setState(120);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(121);
        match(SysYParser::Semicolon);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BTypeContext ------------------------------------------------------------------

SysYParser::BTypeContext::BTypeContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::BTypeContext::Int() { return getToken(SysYParser::Int, 0); }

size_t SysYParser::BTypeContext::getRuleIndex() const { return SysYParser::RuleBType; }

std::any SysYParser::BTypeContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitBType(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::BTypeContext *SysYParser::bType() {
    BTypeContext *_localctx = _tracker.createInstance<BTypeContext>(_ctx, getState());
    enterRule(_localctx, 10, SysYParser::RuleBType);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(123);
        _la = _input->LA(1);
        if (!(_la == SysYParser::T__0

              || _la == SysYParser::Int)) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstDefContext ------------------------------------------------------------------

SysYParser::ConstDefContext::ConstDefContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::ConstDefContext::Identifier() { return getToken(SysYParser::Identifier, 0); }

SysYParser::ConstInitValContext *SysYParser::ConstDefContext::constInitVal() {
    return getRuleContext<SysYParser::ConstInitValContext>(0);
}

std::vector<tree::TerminalNode *> SysYParser::ConstDefContext::Lbrkt() { return getTokens(SysYParser::Lbrkt); }

tree::TerminalNode *SysYParser::ConstDefContext::Lbrkt(size_t i) { return getToken(SysYParser::Lbrkt, i); }

std::vector<SysYParser::ConstExpContext *> SysYParser::ConstDefContext::constExp() {
    return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext *SysYParser::ConstDefContext::constExp(size_t i) {
    return getRuleContext<SysYParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::ConstDefContext::Rbrkt() { return getTokens(SysYParser::Rbrkt); }

tree::TerminalNode *SysYParser::ConstDefContext::Rbrkt(size_t i) { return getToken(SysYParser::Rbrkt, i); }

size_t SysYParser::ConstDefContext::getRuleIndex() const { return SysYParser::RuleConstDef; }

std::any SysYParser::ConstDefContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitConstDef(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::ConstDefContext *SysYParser::constDef() {
    ConstDefContext *_localctx = _tracker.createInstance<ConstDefContext>(_ctx, getState());
    enterRule(_localctx, 12, SysYParser::RuleConstDef);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(125);
        match(SysYParser::Identifier);
        setState(132);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::Lbrkt) {
            setState(126);
            match(SysYParser::Lbrkt);
            setState(127);
            constExp();
            setState(128);
            match(SysYParser::Rbrkt);
            setState(134);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(135);
        match(SysYParser::T__1);
        setState(136);
        constInitVal();

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstInitValContext ------------------------------------------------------------------

SysYParser::ConstInitValContext::ConstInitValContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::ConstInitValContext::getRuleIndex() const { return SysYParser::RuleConstInitVal; }

void SysYParser::ConstInitValContext::copyFrom(ConstInitValContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- ListConstInitValContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::ListConstInitValContext::Lbrace() { return getToken(SysYParser::Lbrace, 0); }

tree::TerminalNode *SysYParser::ListConstInitValContext::Rbrace() { return getToken(SysYParser::Rbrace, 0); }

std::vector<SysYParser::ConstInitValContext *> SysYParser::ListConstInitValContext::constInitVal() {
    return getRuleContexts<SysYParser::ConstInitValContext>();
}

SysYParser::ConstInitValContext *SysYParser::ListConstInitValContext::constInitVal(size_t i) {
    return getRuleContext<SysYParser::ConstInitValContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::ListConstInitValContext::Comma() { return getTokens(SysYParser::Comma); }

tree::TerminalNode *SysYParser::ListConstInitValContext::Comma(size_t i) { return getToken(SysYParser::Comma, i); }

SysYParser::ListConstInitValContext::ListConstInitValContext(ConstInitValContext *ctx) { copyFrom(ctx); }

std::any SysYParser::ListConstInitValContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitListConstInitVal(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ScalarConstInitValContext ------------------------------------------------------------------

SysYParser::ConstExpContext *SysYParser::ScalarConstInitValContext::constExp() {
    return getRuleContext<SysYParser::ConstExpContext>(0);
}

SysYParser::ScalarConstInitValContext::ScalarConstInitValContext(ConstInitValContext *ctx) { copyFrom(ctx); }

std::any SysYParser::ScalarConstInitValContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitScalarConstInitVal(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::ConstInitValContext *SysYParser::constInitVal() {
    ConstInitValContext *_localctx = _tracker.createInstance<ConstInitValContext>(_ctx, getState());
    enterRule(_localctx, 14, SysYParser::RuleConstInitVal);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(151);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::T__12:
            case SysYParser::T__13:
            case SysYParser::Lparen:
            case SysYParser::Minus:
            case SysYParser::Addition:
            case SysYParser::Exclamation:
            case SysYParser::Multiplication:
            case SysYParser::IntLiteral:
            case SysYParser::FloatLiteral:
            case SysYParser::Identifier: {
                _localctx = _tracker.createInstance<SysYParser::ScalarConstInitValContext>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(138);
                constExp();
                break;
            }

            case SysYParser::Lbrace: {
                _localctx = _tracker.createInstance<SysYParser::ListConstInitValContext>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(139);
                match(SysYParser::Lbrace);
                setState(148);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970583675887616) != 0)) {
                    setState(140);
                    constInitVal();
                    setState(145);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                    while (_la == SysYParser::Comma) {
                        setState(141);
                        match(SysYParser::Comma);
                        setState(142);
                        constInitVal();
                        setState(147);
                        _errHandler->sync(this);
                        _la = _input->LA(1);
                    }
                }
                setState(150);
                match(SysYParser::Rbrace);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

SysYParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::BTypeContext *SysYParser::VarDeclContext::bType() { return getRuleContext<SysYParser::BTypeContext>(0); }

std::vector<SysYParser::VarDefContext *> SysYParser::VarDeclContext::varDef() {
    return getRuleContexts<SysYParser::VarDefContext>();
}

SysYParser::VarDefContext *SysYParser::VarDeclContext::varDef(size_t i) {
    return getRuleContext<SysYParser::VarDefContext>(i);
}

tree::TerminalNode *SysYParser::VarDeclContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

std::vector<tree::TerminalNode *> SysYParser::VarDeclContext::Comma() { return getTokens(SysYParser::Comma); }

tree::TerminalNode *SysYParser::VarDeclContext::Comma(size_t i) { return getToken(SysYParser::Comma, i); }

size_t SysYParser::VarDeclContext::getRuleIndex() const { return SysYParser::RuleVarDecl; }

std::any SysYParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitVarDecl(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::VarDeclContext *SysYParser::varDecl() {
    VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
    enterRule(_localctx, 16, SysYParser::RuleVarDecl);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(153);
        bType();
        setState(154);
        varDef();
        setState(159);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::Comma) {
            setState(155);
            match(SysYParser::Comma);
            setState(156);
            varDef();
            setState(161);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }
        setState(162);
        match(SysYParser::Semicolon);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

SysYParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::VarDefContext::getRuleIndex() const { return SysYParser::RuleVarDef; }

void SysYParser::VarDefContext::copyFrom(VarDefContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- UninitVarDefContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::UninitVarDefContext::Identifier() { return getToken(SysYParser::Identifier, 0); }

std::vector<tree::TerminalNode *> SysYParser::UninitVarDefContext::Lbrkt() { return getTokens(SysYParser::Lbrkt); }

tree::TerminalNode *SysYParser::UninitVarDefContext::Lbrkt(size_t i) { return getToken(SysYParser::Lbrkt, i); }

std::vector<SysYParser::ConstExpContext *> SysYParser::UninitVarDefContext::constExp() {
    return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext *SysYParser::UninitVarDefContext::constExp(size_t i) {
    return getRuleContext<SysYParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::UninitVarDefContext::Rbrkt() { return getTokens(SysYParser::Rbrkt); }

tree::TerminalNode *SysYParser::UninitVarDefContext::Rbrkt(size_t i) { return getToken(SysYParser::Rbrkt, i); }

SysYParser::UninitVarDefContext::UninitVarDefContext(VarDefContext *ctx) { copyFrom(ctx); }

std::any SysYParser::UninitVarDefContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitUninitVarDef(this);
    else
        return visitor->visitChildren(this);
}
//----------------- InitVarDefContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::InitVarDefContext::Identifier() { return getToken(SysYParser::Identifier, 0); }

SysYParser::InitValContext *SysYParser::InitVarDefContext::initVal() {
    return getRuleContext<SysYParser::InitValContext>(0);
}

std::vector<tree::TerminalNode *> SysYParser::InitVarDefContext::Lbrkt() { return getTokens(SysYParser::Lbrkt); }

tree::TerminalNode *SysYParser::InitVarDefContext::Lbrkt(size_t i) { return getToken(SysYParser::Lbrkt, i); }

std::vector<SysYParser::ConstExpContext *> SysYParser::InitVarDefContext::constExp() {
    return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext *SysYParser::InitVarDefContext::constExp(size_t i) {
    return getRuleContext<SysYParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::InitVarDefContext::Rbrkt() { return getTokens(SysYParser::Rbrkt); }

tree::TerminalNode *SysYParser::InitVarDefContext::Rbrkt(size_t i) { return getToken(SysYParser::Rbrkt, i); }

SysYParser::InitVarDefContext::InitVarDefContext(VarDefContext *ctx) { copyFrom(ctx); }

std::any SysYParser::InitVarDefContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitInitVarDef(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::VarDefContext *SysYParser::varDef() {
    VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
    enterRule(_localctx, 18, SysYParser::RuleVarDef);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(186);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
            case 1: {
                _localctx = _tracker.createInstance<SysYParser::UninitVarDefContext>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(164);
                match(SysYParser::Identifier);
                setState(171);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == SysYParser::Lbrkt) {
                    setState(165);
                    match(SysYParser::Lbrkt);
                    setState(166);
                    constExp();
                    setState(167);
                    match(SysYParser::Rbrkt);
                    setState(173);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                break;
            }

            case 2: {
                _localctx = _tracker.createInstance<SysYParser::InitVarDefContext>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(174);
                match(SysYParser::Identifier);
                setState(181);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == SysYParser::Lbrkt) {
                    setState(175);
                    match(SysYParser::Lbrkt);
                    setState(176);
                    constExp();
                    setState(177);
                    match(SysYParser::Rbrkt);
                    setState(183);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                }
                setState(184);
                match(SysYParser::T__1);
                setState(185);
                initVal();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- InitValContext ------------------------------------------------------------------

SysYParser::InitValContext::InitValContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::InitValContext::getRuleIndex() const { return SysYParser::RuleInitVal; }

void SysYParser::InitValContext::copyFrom(InitValContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- ScalarInitValContext ------------------------------------------------------------------

SysYParser::ExpContext *SysYParser::ScalarInitValContext::exp() { return getRuleContext<SysYParser::ExpContext>(0); }

SysYParser::ScalarInitValContext::ScalarInitValContext(InitValContext *ctx) { copyFrom(ctx); }

std::any SysYParser::ScalarInitValContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitScalarInitVal(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ListInitvalContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::ListInitvalContext::Lbrace() { return getToken(SysYParser::Lbrace, 0); }

tree::TerminalNode *SysYParser::ListInitvalContext::Rbrace() { return getToken(SysYParser::Rbrace, 0); }

std::vector<SysYParser::InitValContext *> SysYParser::ListInitvalContext::initVal() {
    return getRuleContexts<SysYParser::InitValContext>();
}

SysYParser::InitValContext *SysYParser::ListInitvalContext::initVal(size_t i) {
    return getRuleContext<SysYParser::InitValContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::ListInitvalContext::Comma() { return getTokens(SysYParser::Comma); }

tree::TerminalNode *SysYParser::ListInitvalContext::Comma(size_t i) { return getToken(SysYParser::Comma, i); }

SysYParser::ListInitvalContext::ListInitvalContext(InitValContext *ctx) { copyFrom(ctx); }

std::any SysYParser::ListInitvalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitListInitval(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::InitValContext *SysYParser::initVal() {
    InitValContext *_localctx = _tracker.createInstance<InitValContext>(_ctx, getState());
    enterRule(_localctx, 20, SysYParser::RuleInitVal);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(201);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::T__12:
            case SysYParser::T__13:
            case SysYParser::Lparen:
            case SysYParser::Minus:
            case SysYParser::Addition:
            case SysYParser::Exclamation:
            case SysYParser::Multiplication:
            case SysYParser::IntLiteral:
            case SysYParser::FloatLiteral:
            case SysYParser::Identifier: {
                _localctx = _tracker.createInstance<SysYParser::ScalarInitValContext>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(188);
                exp();
                break;
            }

            case SysYParser::Lbrace: {
                _localctx = _tracker.createInstance<SysYParser::ListInitvalContext>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(189);
                match(SysYParser::Lbrace);
                setState(198);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970583675887616) != 0)) {
                    setState(190);
                    initVal();
                    setState(195);
                    _errHandler->sync(this);
                    _la = _input->LA(1);
                    while (_la == SysYParser::Comma) {
                        setState(191);
                        match(SysYParser::Comma);
                        setState(192);
                        initVal();
                        setState(197);
                        _errHandler->sync(this);
                        _la = _input->LA(1);
                    }
                }
                setState(200);
                match(SysYParser::Rbrace);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

SysYParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::FuncTypeContext *SysYParser::FuncDefContext::funcType() {
    return getRuleContext<SysYParser::FuncTypeContext>(0);
}

tree::TerminalNode *SysYParser::FuncDefContext::Identifier() { return getToken(SysYParser::Identifier, 0); }

tree::TerminalNode *SysYParser::FuncDefContext::Lparen() { return getToken(SysYParser::Lparen, 0); }

tree::TerminalNode *SysYParser::FuncDefContext::Rparen() { return getToken(SysYParser::Rparen, 0); }

SysYParser::BlockContext *SysYParser::FuncDefContext::block() { return getRuleContext<SysYParser::BlockContext>(0); }

SysYParser::FuncFParamsContext *SysYParser::FuncDefContext::funcFParams() {
    return getRuleContext<SysYParser::FuncFParamsContext>(0);
}

size_t SysYParser::FuncDefContext::getRuleIndex() const { return SysYParser::RuleFuncDef; }

std::any SysYParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitFuncDef(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::FuncDefContext *SysYParser::funcDef() {
    FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
    enterRule(_localctx, 22, SysYParser::RuleFuncDef);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(203);
        funcType();
        setState(204);
        match(SysYParser::Identifier);
        setState(205);
        match(SysYParser::Lparen);
        setState(207);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SysYParser::T__0

            || _la == SysYParser::Int) {
            setState(206);
            funcFParams();
        }
        setState(209);
        match(SysYParser::Rparen);
        setState(210);
        block();

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FuncTypeContext ------------------------------------------------------------------

SysYParser::FuncTypeContext::FuncTypeContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::FuncTypeContext::Void() { return getToken(SysYParser::Void, 0); }

tree::TerminalNode *SysYParser::FuncTypeContext::Int() { return getToken(SysYParser::Int, 0); }

size_t SysYParser::FuncTypeContext::getRuleIndex() const { return SysYParser::RuleFuncType; }

std::any SysYParser::FuncTypeContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitFuncType(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::FuncTypeContext *SysYParser::funcType() {
    FuncTypeContext *_localctx = _tracker.createInstance<FuncTypeContext>(_ctx, getState());
    enterRule(_localctx, 24, SysYParser::RuleFuncType);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(212);
        _la = _input->LA(1);
        if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 98306) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FuncFParamsContext ------------------------------------------------------------------

SysYParser::FuncFParamsContext::FuncFParamsContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<SysYParser::FuncFParamContext *> SysYParser::FuncFParamsContext::funcFParam() {
    return getRuleContexts<SysYParser::FuncFParamContext>();
}

SysYParser::FuncFParamContext *SysYParser::FuncFParamsContext::funcFParam(size_t i) {
    return getRuleContext<SysYParser::FuncFParamContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::FuncFParamsContext::Comma() { return getTokens(SysYParser::Comma); }

tree::TerminalNode *SysYParser::FuncFParamsContext::Comma(size_t i) { return getToken(SysYParser::Comma, i); }

size_t SysYParser::FuncFParamsContext::getRuleIndex() const { return SysYParser::RuleFuncFParams; }

std::any SysYParser::FuncFParamsContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitFuncFParams(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::FuncFParamsContext *SysYParser::funcFParams() {
    FuncFParamsContext *_localctx = _tracker.createInstance<FuncFParamsContext>(_ctx, getState());
    enterRule(_localctx, 26, SysYParser::RuleFuncFParams);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(214);
        funcFParam();
        setState(219);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::Comma) {
            setState(215);
            match(SysYParser::Comma);
            setState(216);
            funcFParam();
            setState(221);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FuncFParamContext ------------------------------------------------------------------

SysYParser::FuncFParamContext::FuncFParamContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::BTypeContext *SysYParser::FuncFParamContext::bType() { return getRuleContext<SysYParser::BTypeContext>(0); }

tree::TerminalNode *SysYParser::FuncFParamContext::Identifier() { return getToken(SysYParser::Identifier, 0); }

std::vector<tree::TerminalNode *> SysYParser::FuncFParamContext::Lbrkt() { return getTokens(SysYParser::Lbrkt); }

tree::TerminalNode *SysYParser::FuncFParamContext::Lbrkt(size_t i) { return getToken(SysYParser::Lbrkt, i); }

std::vector<tree::TerminalNode *> SysYParser::FuncFParamContext::Rbrkt() { return getTokens(SysYParser::Rbrkt); }

tree::TerminalNode *SysYParser::FuncFParamContext::Rbrkt(size_t i) { return getToken(SysYParser::Rbrkt, i); }

std::vector<SysYParser::ConstExpContext *> SysYParser::FuncFParamContext::constExp() {
    return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext *SysYParser::FuncFParamContext::constExp(size_t i) {
    return getRuleContext<SysYParser::ConstExpContext>(i);
}

size_t SysYParser::FuncFParamContext::getRuleIndex() const { return SysYParser::RuleFuncFParam; }

std::any SysYParser::FuncFParamContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitFuncFParam(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::FuncFParamContext *SysYParser::funcFParam() {
    FuncFParamContext *_localctx = _tracker.createInstance<FuncFParamContext>(_ctx, getState());
    enterRule(_localctx, 28, SysYParser::RuleFuncFParam);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(222);
        bType();
        setState(223);
        match(SysYParser::Identifier);
        setState(235);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SysYParser::Lbrkt) {
            setState(224);
            match(SysYParser::Lbrkt);
            setState(225);
            match(SysYParser::Rbrkt);
            setState(232);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == SysYParser::Lbrkt) {
                setState(226);
                match(SysYParser::Lbrkt);
                setState(227);
                constExp();
                setState(228);
                match(SysYParser::Rbrkt);
                setState(234);
                _errHandler->sync(this);
                _la = _input->LA(1);
            }
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

SysYParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::BlockContext::Lbrace() { return getToken(SysYParser::Lbrace, 0); }

tree::TerminalNode *SysYParser::BlockContext::Rbrace() { return getToken(SysYParser::Rbrace, 0); }

SysYParser::BlockItemListContext *SysYParser::BlockContext::blockItemList() {
    return getRuleContext<SysYParser::BlockItemListContext>(0);
}

size_t SysYParser::BlockContext::getRuleIndex() const { return SysYParser::RuleBlock; }

std::any SysYParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitBlock(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::BlockContext *SysYParser::block() {
    BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
    enterRule(_localctx, 30, SysYParser::RuleBlock);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(237);
        match(SysYParser::Lbrace);
        setState(239);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970592319201282) != 0)) {
            setState(238);
            blockItemList();
        }
        setState(241);
        match(SysYParser::Rbrace);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BlockItemListContext ------------------------------------------------------------------

SysYParser::BlockItemListContext::BlockItemListContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<SysYParser::BlockItemContext *> SysYParser::BlockItemListContext::blockItem() {
    return getRuleContexts<SysYParser::BlockItemContext>();
}

SysYParser::BlockItemContext *SysYParser::BlockItemListContext::blockItem(size_t i) {
    return getRuleContext<SysYParser::BlockItemContext>(i);
}

size_t SysYParser::BlockItemListContext::getRuleIndex() const { return SysYParser::RuleBlockItemList; }

std::any SysYParser::BlockItemListContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitBlockItemList(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::BlockItemListContext *SysYParser::blockItemList() {
    BlockItemListContext *_localctx = _tracker.createInstance<BlockItemListContext>(_ctx, getState());
    enterRule(_localctx, 32, SysYParser::RuleBlockItemList);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(244);
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
            setState(243);
            blockItem();
            setState(246);
            _errHandler->sync(this);
            _la = _input->LA(1);
        } while ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970592319201282) != 0));

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

SysYParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::DeclContext *SysYParser::BlockItemContext::decl() { return getRuleContext<SysYParser::DeclContext>(0); }

SysYParser::StmtContext *SysYParser::BlockItemContext::stmt() { return getRuleContext<SysYParser::StmtContext>(0); }

size_t SysYParser::BlockItemContext::getRuleIndex() const { return SysYParser::RuleBlockItem; }

std::any SysYParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitBlockItem(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::BlockItemContext *SysYParser::blockItem() {
    BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
    enterRule(_localctx, 34, SysYParser::RuleBlockItem);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(250);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::T__0:
            case SysYParser::Int:
            case SysYParser::Const: {
                enterOuterAlt(_localctx, 1);
                setState(248);
                decl();
                break;
            }

            case SysYParser::T__12:
            case SysYParser::T__13:
            case SysYParser::Return:
            case SysYParser::If:
            case SysYParser::While:
            case SysYParser::Break:
            case SysYParser::Continue:
            case SysYParser::Lparen:
            case SysYParser::Lbrace:
            case SysYParser::Semicolon:
            case SysYParser::Minus:
            case SysYParser::Addition:
            case SysYParser::Exclamation:
            case SysYParser::Multiplication:
            case SysYParser::IntLiteral:
            case SysYParser::FloatLiteral:
            case SysYParser::Identifier: {
                enterOuterAlt(_localctx, 2);
                setState(249);
                stmt();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

SysYParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::AssignStmtContext *SysYParser::StmtContext::assignStmt() {
    return getRuleContext<SysYParser::AssignStmtContext>(0);
}

SysYParser::ExpStmtContext *SysYParser::StmtContext::expStmt() { return getRuleContext<SysYParser::ExpStmtContext>(0); }

SysYParser::BlockStmtContext *SysYParser::StmtContext::blockStmt() {
    return getRuleContext<SysYParser::BlockStmtContext>(0);
}

SysYParser::SeleStmtContext *SysYParser::StmtContext::seleStmt() {
    return getRuleContext<SysYParser::SeleStmtContext>(0);
}

SysYParser::LoopStmtContext *SysYParser::StmtContext::loopStmt() {
    return getRuleContext<SysYParser::LoopStmtContext>(0);
}

SysYParser::JumpStmtContext *SysYParser::StmtContext::jumpStmt() {
    return getRuleContext<SysYParser::JumpStmtContext>(0);
}

size_t SysYParser::StmtContext::getRuleIndex() const { return SysYParser::RuleStmt; }

std::any SysYParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitStmt(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::StmtContext *SysYParser::stmt() {
    StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
    enterRule(_localctx, 36, SysYParser::RuleStmt);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(258);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
            case 1: {
                enterOuterAlt(_localctx, 1);
                setState(252);
                assignStmt();
                break;
            }

            case 2: {
                enterOuterAlt(_localctx, 2);
                setState(253);
                expStmt();
                break;
            }

            case 3: {
                enterOuterAlt(_localctx, 3);
                setState(254);
                blockStmt();
                break;
            }

            case 4: {
                enterOuterAlt(_localctx, 4);
                setState(255);
                seleStmt();
                break;
            }

            case 5: {
                enterOuterAlt(_localctx, 5);
                setState(256);
                loopStmt();
                break;
            }

            case 6: {
                enterOuterAlt(_localctx, 6);
                setState(257);
                jumpStmt();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AssignStmtContext ------------------------------------------------------------------

SysYParser::AssignStmtContext::AssignStmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::LValContext *SysYParser::AssignStmtContext::lVal() { return getRuleContext<SysYParser::LValContext>(0); }

SysYParser::AssignOpContext *SysYParser::AssignStmtContext::assignOp() {
    return getRuleContext<SysYParser::AssignOpContext>(0);
}

SysYParser::ExpContext *SysYParser::AssignStmtContext::exp() { return getRuleContext<SysYParser::ExpContext>(0); }

tree::TerminalNode *SysYParser::AssignStmtContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

size_t SysYParser::AssignStmtContext::getRuleIndex() const { return SysYParser::RuleAssignStmt; }

std::any SysYParser::AssignStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitAssignStmt(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::AssignStmtContext *SysYParser::assignStmt() {
    AssignStmtContext *_localctx = _tracker.createInstance<AssignStmtContext>(_ctx, getState());
    enterRule(_localctx, 38, SysYParser::RuleAssignStmt);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(260);
        lVal();
        setState(261);
        assignOp();
        setState(262);
        exp();
        setState(263);
        match(SysYParser::Semicolon);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AssignOpContext ------------------------------------------------------------------

SysYParser::AssignOpContext::AssignOpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::AssignOpContext::getRuleIndex() const { return SysYParser::RuleAssignOp; }

std::any SysYParser::AssignOpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitAssignOp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::AssignOpContext *SysYParser::assignOp() {
    AssignOpContext *_localctx = _tracker.createInstance<AssignOpContext>(_ctx, getState());
    enterRule(_localctx, 40, SysYParser::RuleAssignOp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(265);
        _la = _input->LA(1);
        if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 8188) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExpStmtContext ------------------------------------------------------------------

SysYParser::ExpStmtContext::ExpStmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::ExpStmtContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

SysYParser::ExpContext *SysYParser::ExpStmtContext::exp() { return getRuleContext<SysYParser::ExpContext>(0); }

size_t SysYParser::ExpStmtContext::getRuleIndex() const { return SysYParser::RuleExpStmt; }

std::any SysYParser::ExpStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitExpStmt(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::ExpStmtContext *SysYParser::expStmt() {
    ExpStmtContext *_localctx = _tracker.createInstance<ExpStmtContext>(_ctx, getState());
    enterRule(_localctx, 42, SysYParser::RuleExpStmt);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(268);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970582602145792) != 0)) {
            setState(267);
            exp();
        }
        setState(270);
        match(SysYParser::Semicolon);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- BlockStmtContext ------------------------------------------------------------------

SysYParser::BlockStmtContext::BlockStmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::BlockContext *SysYParser::BlockStmtContext::block() { return getRuleContext<SysYParser::BlockContext>(0); }

size_t SysYParser::BlockStmtContext::getRuleIndex() const { return SysYParser::RuleBlockStmt; }

std::any SysYParser::BlockStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitBlockStmt(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::BlockStmtContext *SysYParser::blockStmt() {
    BlockStmtContext *_localctx = _tracker.createInstance<BlockStmtContext>(_ctx, getState());
    enterRule(_localctx, 44, SysYParser::RuleBlockStmt);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(272);
        block();

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- SeleStmtContext ------------------------------------------------------------------

SysYParser::SeleStmtContext::SeleStmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::SeleStmtContext::getRuleIndex() const { return SysYParser::RuleSeleStmt; }

void SysYParser::SeleStmtContext::copyFrom(SeleStmtContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- IfStmtContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::IfStmtContext::If() { return getToken(SysYParser::If, 0); }

tree::TerminalNode *SysYParser::IfStmtContext::Lparen() { return getToken(SysYParser::Lparen, 0); }

SysYParser::CondExpContext *SysYParser::IfStmtContext::condExp() {
    return getRuleContext<SysYParser::CondExpContext>(0);
}

tree::TerminalNode *SysYParser::IfStmtContext::Rparen() { return getToken(SysYParser::Rparen, 0); }

std::vector<SysYParser::StmtContext *> SysYParser::IfStmtContext::stmt() {
    return getRuleContexts<SysYParser::StmtContext>();
}

SysYParser::StmtContext *SysYParser::IfStmtContext::stmt(size_t i) {
    return getRuleContext<SysYParser::StmtContext>(i);
}

tree::TerminalNode *SysYParser::IfStmtContext::Else() { return getToken(SysYParser::Else, 0); }

SysYParser::IfStmtContext::IfStmtContext(SeleStmtContext *ctx) { copyFrom(ctx); }

std::any SysYParser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitIfStmt(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::SeleStmtContext *SysYParser::seleStmt() {
    SeleStmtContext *_localctx = _tracker.createInstance<SeleStmtContext>(_ctx, getState());
    enterRule(_localctx, 46, SysYParser::RuleSeleStmt);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        _localctx = _tracker.createInstance<SysYParser::IfStmtContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(274);
        match(SysYParser::If);
        setState(275);
        match(SysYParser::Lparen);
        setState(276);
        condExp();
        setState(277);
        match(SysYParser::Rparen);
        setState(278);
        stmt();
        setState(281);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
            case 1: {
                setState(279);
                match(SysYParser::Else);
                setState(280);
                stmt();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LoopStmtContext ------------------------------------------------------------------

SysYParser::LoopStmtContext::LoopStmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::LoopStmtContext::getRuleIndex() const { return SysYParser::RuleLoopStmt; }

void SysYParser::LoopStmtContext::copyFrom(LoopStmtContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- WhileLoopContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::WhileLoopContext::While() { return getToken(SysYParser::While, 0); }

tree::TerminalNode *SysYParser::WhileLoopContext::Lparen() { return getToken(SysYParser::Lparen, 0); }

SysYParser::CondExpContext *SysYParser::WhileLoopContext::condExp() {
    return getRuleContext<SysYParser::CondExpContext>(0);
}

tree::TerminalNode *SysYParser::WhileLoopContext::Rparen() { return getToken(SysYParser::Rparen, 0); }

SysYParser::StmtContext *SysYParser::WhileLoopContext::stmt() { return getRuleContext<SysYParser::StmtContext>(0); }

SysYParser::WhileLoopContext::WhileLoopContext(LoopStmtContext *ctx) { copyFrom(ctx); }

std::any SysYParser::WhileLoopContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitWhileLoop(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::LoopStmtContext *SysYParser::loopStmt() {
    LoopStmtContext *_localctx = _tracker.createInstance<LoopStmtContext>(_ctx, getState());
    enterRule(_localctx, 48, SysYParser::RuleLoopStmt);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        _localctx = _tracker.createInstance<SysYParser::WhileLoopContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(283);
        match(SysYParser::While);
        setState(284);
        match(SysYParser::Lparen);
        setState(285);
        condExp();
        setState(286);
        match(SysYParser::Rparen);
        setState(287);
        stmt();

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- JumpStmtContext ------------------------------------------------------------------

SysYParser::JumpStmtContext::JumpStmtContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::JumpStmtContext::getRuleIndex() const { return SysYParser::RuleJumpStmt; }

void SysYParser::JumpStmtContext::copyFrom(JumpStmtContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- BreakStmtContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::BreakStmtContext::Break() { return getToken(SysYParser::Break, 0); }

tree::TerminalNode *SysYParser::BreakStmtContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

SysYParser::BreakStmtContext::BreakStmtContext(JumpStmtContext *ctx) { copyFrom(ctx); }

std::any SysYParser::BreakStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitBreakStmt(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ReturnStmtContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::ReturnStmtContext::Return() { return getToken(SysYParser::Return, 0); }

tree::TerminalNode *SysYParser::ReturnStmtContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

SysYParser::ExpContext *SysYParser::ReturnStmtContext::exp() { return getRuleContext<SysYParser::ExpContext>(0); }

SysYParser::ReturnStmtContext::ReturnStmtContext(JumpStmtContext *ctx) { copyFrom(ctx); }

std::any SysYParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitReturnStmt(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ContinueStmtContext ------------------------------------------------------------------

tree::TerminalNode *SysYParser::ContinueStmtContext::Continue() { return getToken(SysYParser::Continue, 0); }

tree::TerminalNode *SysYParser::ContinueStmtContext::Semicolon() { return getToken(SysYParser::Semicolon, 0); }

SysYParser::ContinueStmtContext::ContinueStmtContext(JumpStmtContext *ctx) { copyFrom(ctx); }

std::any SysYParser::ContinueStmtContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitContinueStmt(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::JumpStmtContext *SysYParser::jumpStmt() {
    JumpStmtContext *_localctx = _tracker.createInstance<JumpStmtContext>(_ctx, getState());
    enterRule(_localctx, 50, SysYParser::RuleJumpStmt);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(298);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::Continue: {
                _localctx = _tracker.createInstance<SysYParser::ContinueStmtContext>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(289);
                match(SysYParser::Continue);
                setState(290);
                match(SysYParser::Semicolon);
                break;
            }

            case SysYParser::Break: {
                _localctx = _tracker.createInstance<SysYParser::BreakStmtContext>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(291);
                match(SysYParser::Break);
                setState(292);
                match(SysYParser::Semicolon);
                break;
            }

            case SysYParser::Return: {
                _localctx = _tracker.createInstance<SysYParser::ReturnStmtContext>(_localctx);
                enterOuterAlt(_localctx, 3);
                setState(293);
                match(SysYParser::Return);
                setState(295);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970582602145792) != 0)) {
                    setState(294);
                    exp();
                }
                setState(297);
                match(SysYParser::Semicolon);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExpContext ------------------------------------------------------------------

SysYParser::ExpContext::ExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::AddExpContext *SysYParser::ExpContext::addExp() { return getRuleContext<SysYParser::AddExpContext>(0); }

size_t SysYParser::ExpContext::getRuleIndex() const { return SysYParser::RuleExp; }

std::any SysYParser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitExp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::ExpContext *SysYParser::exp() {
    ExpContext *_localctx = _tracker.createInstance<ExpContext>(_ctx, getState());
    enterRule(_localctx, 52, SysYParser::RuleExp);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(300);
        addExp(0);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ConstExpContext ------------------------------------------------------------------

SysYParser::ConstExpContext::ConstExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::AddExpContext *SysYParser::ConstExpContext::addExp() {
    return getRuleContext<SysYParser::AddExpContext>(0);
}

size_t SysYParser::ConstExpContext::getRuleIndex() const { return SysYParser::RuleConstExp; }

std::any SysYParser::ConstExpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitConstExp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::ConstExpContext *SysYParser::constExp() {
    ConstExpContext *_localctx = _tracker.createInstance<ConstExpContext>(_ctx, getState());
    enterRule(_localctx, 54, SysYParser::RuleConstExp);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(302);
        addExp(0);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

SysYParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::LValContext::Identifier() { return getToken(SysYParser::Identifier, 0); }

std::vector<tree::TerminalNode *> SysYParser::LValContext::Lbrkt() { return getTokens(SysYParser::Lbrkt); }

tree::TerminalNode *SysYParser::LValContext::Lbrkt(size_t i) { return getToken(SysYParser::Lbrkt, i); }

std::vector<SysYParser::ExpContext *> SysYParser::LValContext::exp() {
    return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext *SysYParser::LValContext::exp(size_t i) { return getRuleContext<SysYParser::ExpContext>(i); }

std::vector<tree::TerminalNode *> SysYParser::LValContext::Rbrkt() { return getTokens(SysYParser::Rbrkt); }

tree::TerminalNode *SysYParser::LValContext::Rbrkt(size_t i) { return getToken(SysYParser::Rbrkt, i); }

size_t SysYParser::LValContext::getRuleIndex() const { return SysYParser::RuleLVal; }

std::any SysYParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitLVal(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::LValContext *SysYParser::lVal() {
    LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
    enterRule(_localctx, 56, SysYParser::RuleLVal);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        setState(304);
        match(SysYParser::Identifier);
        setState(311);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(305);
                match(SysYParser::Lbrkt);
                setState(306);
                exp();
                setState(307);
                match(SysYParser::Rbrkt);
            }
            setState(313);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

SysYParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::PrimaryExpContext::getRuleIndex() const { return SysYParser::RulePrimaryExp; }

void SysYParser::PrimaryExpContext::copyFrom(PrimaryExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- PrimaryExp2Context ------------------------------------------------------------------

SysYParser::LValContext *SysYParser::PrimaryExp2Context::lVal() { return getRuleContext<SysYParser::LValContext>(0); }

SysYParser::PrimaryExp2Context::PrimaryExp2Context(PrimaryExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::PrimaryExp2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitPrimaryExp2(this);
    else
        return visitor->visitChildren(this);
}
//----------------- PrimaryExp1Context ------------------------------------------------------------------

tree::TerminalNode *SysYParser::PrimaryExp1Context::Lparen() { return getToken(SysYParser::Lparen, 0); }

SysYParser::ExpContext *SysYParser::PrimaryExp1Context::exp() { return getRuleContext<SysYParser::ExpContext>(0); }

tree::TerminalNode *SysYParser::PrimaryExp1Context::Rparen() { return getToken(SysYParser::Rparen, 0); }

SysYParser::PrimaryExp1Context::PrimaryExp1Context(PrimaryExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::PrimaryExp1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitPrimaryExp1(this);
    else
        return visitor->visitChildren(this);
}
//----------------- PrimaryExp3Context ------------------------------------------------------------------

SysYParser::NumberContext *SysYParser::PrimaryExp3Context::number() {
    return getRuleContext<SysYParser::NumberContext>(0);
}

SysYParser::PrimaryExp3Context::PrimaryExp3Context(PrimaryExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::PrimaryExp3Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitPrimaryExp3(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::PrimaryExpContext *SysYParser::primaryExp() {
    PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
    enterRule(_localctx, 58, SysYParser::RulePrimaryExp);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(320);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::Lparen: {
                _localctx = _tracker.createInstance<SysYParser::PrimaryExp1Context>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(314);
                match(SysYParser::Lparen);
                setState(315);
                exp();
                setState(316);
                match(SysYParser::Rparen);
                break;
            }

            case SysYParser::Identifier: {
                _localctx = _tracker.createInstance<SysYParser::PrimaryExp2Context>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(318);
                lVal();
                break;
            }

            case SysYParser::IntLiteral:
            case SysYParser::FloatLiteral: {
                _localctx = _tracker.createInstance<SysYParser::PrimaryExp3Context>(_localctx);
                enterOuterAlt(_localctx, 3);
                setState(319);
                number();
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

SysYParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::NumberContext::getRuleIndex() const { return SysYParser::RuleNumber; }

void SysYParser::NumberContext::copyFrom(NumberContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- Number1Context ------------------------------------------------------------------

tree::TerminalNode *SysYParser::Number1Context::IntLiteral() { return getToken(SysYParser::IntLiteral, 0); }

SysYParser::Number1Context::Number1Context(NumberContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Number1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitNumber1(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Number2Context ------------------------------------------------------------------

tree::TerminalNode *SysYParser::Number2Context::FloatLiteral() { return getToken(SysYParser::FloatLiteral, 0); }

SysYParser::Number2Context::Number2Context(NumberContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Number2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitNumber2(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::NumberContext *SysYParser::number() {
    NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
    enterRule(_localctx, 60, SysYParser::RuleNumber);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(324);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
            case SysYParser::IntLiteral: {
                _localctx = _tracker.createInstance<SysYParser::Number1Context>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(322);
                match(SysYParser::IntLiteral);
                break;
            }

            case SysYParser::FloatLiteral: {
                _localctx = _tracker.createInstance<SysYParser::Number2Context>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(323);
                match(SysYParser::FloatLiteral);
                break;
            }

            default:
                throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FuncRParamsContext ------------------------------------------------------------------

SysYParser::FuncRParamsContext::FuncRParamsContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<SysYParser::FuncRParamContext *> SysYParser::FuncRParamsContext::funcRParam() {
    return getRuleContexts<SysYParser::FuncRParamContext>();
}

SysYParser::FuncRParamContext *SysYParser::FuncRParamsContext::funcRParam(size_t i) {
    return getRuleContext<SysYParser::FuncRParamContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::FuncRParamsContext::Comma() { return getTokens(SysYParser::Comma); }

tree::TerminalNode *SysYParser::FuncRParamsContext::Comma(size_t i) { return getToken(SysYParser::Comma, i); }

size_t SysYParser::FuncRParamsContext::getRuleIndex() const { return SysYParser::RuleFuncRParams; }

std::any SysYParser::FuncRParamsContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitFuncRParams(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::FuncRParamsContext *SysYParser::funcRParams() {
    FuncRParamsContext *_localctx = _tracker.createInstance<FuncRParamsContext>(_ctx, getState());
    enterRule(_localctx, 62, SysYParser::RuleFuncRParams);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(326);
        funcRParam();
        setState(331);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::Comma) {
            setState(327);
            match(SysYParser::Comma);
            setState(328);
            funcRParam();
            setState(333);
            _errHandler->sync(this);
            _la = _input->LA(1);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- FuncRParamContext ------------------------------------------------------------------

SysYParser::FuncRParamContext::FuncRParamContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::ExpContext *SysYParser::FuncRParamContext::exp() { return getRuleContext<SysYParser::ExpContext>(0); }

size_t SysYParser::FuncRParamContext::getRuleIndex() const { return SysYParser::RuleFuncRParam; }

std::any SysYParser::FuncRParamContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitFuncRParam(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::FuncRParamContext *SysYParser::funcRParam() {
    FuncRParamContext *_localctx = _tracker.createInstance<FuncRParamContext>(_ctx, getState());
    enterRule(_localctx, 64, SysYParser::RuleFuncRParam);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(334);
        exp();

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

SysYParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::UnaryExpContext::getRuleIndex() const { return SysYParser::RuleUnaryExp; }

void SysYParser::UnaryExpContext::copyFrom(UnaryExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- Unary1Context ------------------------------------------------------------------

SysYParser::PrimaryExpContext *SysYParser::Unary1Context::primaryExp() {
    return getRuleContext<SysYParser::PrimaryExpContext>(0);
}

SysYParser::Unary1Context::Unary1Context(UnaryExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Unary1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitUnary1(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Unary2Context ------------------------------------------------------------------

tree::TerminalNode *SysYParser::Unary2Context::Identifier() { return getToken(SysYParser::Identifier, 0); }

tree::TerminalNode *SysYParser::Unary2Context::Lparen() { return getToken(SysYParser::Lparen, 0); }

tree::TerminalNode *SysYParser::Unary2Context::Rparen() { return getToken(SysYParser::Rparen, 0); }

SysYParser::FuncRParamsContext *SysYParser::Unary2Context::funcRParams() {
    return getRuleContext<SysYParser::FuncRParamsContext>(0);
}

SysYParser::Unary2Context::Unary2Context(UnaryExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Unary2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitUnary2(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Unary3Context ------------------------------------------------------------------

SysYParser::UnaryOpContext *SysYParser::Unary3Context::unaryOp() {
    return getRuleContext<SysYParser::UnaryOpContext>(0);
}

SysYParser::UnaryExpContext *SysYParser::Unary3Context::unaryExp() {
    return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::Unary3Context::Unary3Context(UnaryExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Unary3Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitUnary3(this);
    else
        return visitor->visitChildren(this);
}
SysYParser::UnaryExpContext *SysYParser::unaryExp() {
    UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
    enterRule(_localctx, 66, SysYParser::RuleUnaryExp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(346);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
            case 1: {
                _localctx = _tracker.createInstance<SysYParser::Unary1Context>(_localctx);
                enterOuterAlt(_localctx, 1);
                setState(336);
                primaryExp();
                break;
            }

            case 2: {
                _localctx = _tracker.createInstance<SysYParser::Unary2Context>(_localctx);
                enterOuterAlt(_localctx, 2);
                setState(337);
                match(SysYParser::Identifier);
                setState(338);
                match(SysYParser::Lparen);
                setState(340);
                _errHandler->sync(this);

                _la = _input->LA(1);
                if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 1970582602145792) != 0)) {
                    setState(339);
                    funcRParams();
                }
                setState(342);
                match(SysYParser::Rparen);
                break;
            }

            case 3: {
                _localctx = _tracker.createInstance<SysYParser::Unary3Context>(_localctx);
                enterOuterAlt(_localctx, 3);
                setState(343);
                unaryOp();
                setState(344);
                unaryExp();
                break;
            }

            default:
                break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- UnaryOpContext ------------------------------------------------------------------

SysYParser::UnaryOpContext::UnaryOpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::UnaryOpContext::Multiplication() { return getToken(SysYParser::Multiplication, 0); }

tree::TerminalNode *SysYParser::UnaryOpContext::Addition() { return getToken(SysYParser::Addition, 0); }

tree::TerminalNode *SysYParser::UnaryOpContext::Minus() { return getToken(SysYParser::Minus, 0); }

tree::TerminalNode *SysYParser::UnaryOpContext::Exclamation() { return getToken(SysYParser::Exclamation, 0); }

size_t SysYParser::UnaryOpContext::getRuleIndex() const { return SysYParser::RuleUnaryOp; }

std::any SysYParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitUnaryOp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::UnaryOpContext *SysYParser::unaryOp() {
    UnaryOpContext *_localctx = _tracker.createInstance<UnaryOpContext>(_ctx, getState());
    enterRule(_localctx, 68, SysYParser::RuleUnaryOp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(348);
        _la = _input->LA(1);
        if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 257698062336) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

SysYParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::MulExpContext::getRuleIndex() const { return SysYParser::RuleMulExp; }

void SysYParser::MulExpContext::copyFrom(MulExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- Mul2Context ------------------------------------------------------------------

SysYParser::MulExpContext *SysYParser::Mul2Context::mulExp() { return getRuleContext<SysYParser::MulExpContext>(0); }

SysYParser::MulOpContext *SysYParser::Mul2Context::mulOp() { return getRuleContext<SysYParser::MulOpContext>(0); }

SysYParser::UnaryExpContext *SysYParser::Mul2Context::unaryExp() {
    return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::Mul2Context::Mul2Context(MulExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Mul2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitMul2(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Mul1Context ------------------------------------------------------------------

SysYParser::UnaryExpContext *SysYParser::Mul1Context::unaryExp() {
    return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::Mul1Context::Mul1Context(MulExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Mul1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitMul1(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::MulExpContext *SysYParser::mulExp() { return mulExp(0); }

SysYParser::MulExpContext *SysYParser::mulExp(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    SysYParser::MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, parentState);
    SysYParser::MulExpContext *previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 70;
    enterRecursionRule(_localctx, 70, SysYParser::RuleMulExp, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<Mul1Context>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(351);
        unaryExp();
        _ctx->stop = _input->LT(-1);
        setState(359);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty()) triggerExitRuleEvent();
                previousContext = _localctx;
                auto newContext = _tracker.createInstance<Mul2Context>(
                    _tracker.createInstance<MulExpContext>(parentContext, parentState));
                _localctx = newContext;
                pushNewRecursionContext(newContext, startState, RuleMulExp);
                setState(353);

                if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(354);
                mulOp();
                setState(355);
                unaryExp();
            }
            setState(361);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- MulOpContext ------------------------------------------------------------------

SysYParser::MulOpContext::MulOpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::MulOpContext::Multiplication() { return getToken(SysYParser::Multiplication, 0); }

tree::TerminalNode *SysYParser::MulOpContext::Division() { return getToken(SysYParser::Division, 0); }

tree::TerminalNode *SysYParser::MulOpContext::Modulo() { return getToken(SysYParser::Modulo, 0); }

size_t SysYParser::MulOpContext::getRuleIndex() const { return SysYParser::RuleMulOp; }

std::any SysYParser::MulOpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitMulOp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::MulOpContext *SysYParser::mulOp() {
    MulOpContext *_localctx = _tracker.createInstance<MulOpContext>(_ctx, getState());
    enterRule(_localctx, 72, SysYParser::RuleMulOp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(362);
        _la = _input->LA(1);
        if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 962072674304) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

SysYParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::AddExpContext::getRuleIndex() const { return SysYParser::RuleAddExp; }

void SysYParser::AddExpContext::copyFrom(AddExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- Add2Context ------------------------------------------------------------------

SysYParser::AddExpContext *SysYParser::Add2Context::addExp() { return getRuleContext<SysYParser::AddExpContext>(0); }

SysYParser::AddOpContext *SysYParser::Add2Context::addOp() { return getRuleContext<SysYParser::AddOpContext>(0); }

SysYParser::MulExpContext *SysYParser::Add2Context::mulExp() { return getRuleContext<SysYParser::MulExpContext>(0); }

SysYParser::Add2Context::Add2Context(AddExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Add2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitAdd2(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Add1Context ------------------------------------------------------------------

SysYParser::MulExpContext *SysYParser::Add1Context::mulExp() { return getRuleContext<SysYParser::MulExpContext>(0); }

SysYParser::Add1Context::Add1Context(AddExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Add1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitAdd1(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::AddExpContext *SysYParser::addExp() { return addExp(0); }

SysYParser::AddExpContext *SysYParser::addExp(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    SysYParser::AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, parentState);
    SysYParser::AddExpContext *previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 74;
    enterRecursionRule(_localctx, 74, SysYParser::RuleAddExp, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<Add1Context>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(365);
        mulExp(0);
        _ctx->stop = _input->LT(-1);
        setState(373);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty()) triggerExitRuleEvent();
                previousContext = _localctx;
                auto newContext = _tracker.createInstance<Add2Context>(
                    _tracker.createInstance<AddExpContext>(parentContext, parentState));
                _localctx = newContext;
                pushNewRecursionContext(newContext, startState, RuleAddExp);
                setState(367);

                if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(368);
                addOp();
                setState(369);
                mulExp(0);
            }
            setState(375);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- AddOpContext ------------------------------------------------------------------

SysYParser::AddOpContext::AddOpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::AddOpContext::Addition() { return getToken(SysYParser::Addition, 0); }

tree::TerminalNode *SysYParser::AddOpContext::Minus() { return getToken(SysYParser::Minus, 0); }

size_t SysYParser::AddOpContext::getRuleIndex() const { return SysYParser::RuleAddOp; }

std::any SysYParser::AddOpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitAddOp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::AddOpContext *SysYParser::addOp() {
    AddOpContext *_localctx = _tracker.createInstance<AddOpContext>(_ctx, getState());
    enterRule(_localctx, 76, SysYParser::RuleAddOp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(376);
        _la = _input->LA(1);
        if (!(_la == SysYParser::Minus

              || _la == SysYParser::Addition)) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- RelExpContext ------------------------------------------------------------------

SysYParser::RelExpContext::RelExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::RelExpContext::getRuleIndex() const { return SysYParser::RuleRelExp; }

void SysYParser::RelExpContext::copyFrom(RelExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- Rel2Context ------------------------------------------------------------------

SysYParser::RelExpContext *SysYParser::Rel2Context::relExp() { return getRuleContext<SysYParser::RelExpContext>(0); }

SysYParser::RelOpContext *SysYParser::Rel2Context::relOp() { return getRuleContext<SysYParser::RelOpContext>(0); }

SysYParser::AddExpContext *SysYParser::Rel2Context::addExp() { return getRuleContext<SysYParser::AddExpContext>(0); }

SysYParser::Rel2Context::Rel2Context(RelExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Rel2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitRel2(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Rel1Context ------------------------------------------------------------------

SysYParser::AddExpContext *SysYParser::Rel1Context::addExp() { return getRuleContext<SysYParser::AddExpContext>(0); }

SysYParser::Rel1Context::Rel1Context(RelExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Rel1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitRel1(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::RelExpContext *SysYParser::relExp() { return relExp(0); }

SysYParser::RelExpContext *SysYParser::relExp(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    SysYParser::RelExpContext *_localctx = _tracker.createInstance<RelExpContext>(_ctx, parentState);
    SysYParser::RelExpContext *previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 78;
    enterRecursionRule(_localctx, 78, SysYParser::RuleRelExp, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<Rel1Context>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(379);
        addExp(0);
        _ctx->stop = _input->LT(-1);
        setState(387);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty()) triggerExitRuleEvent();
                previousContext = _localctx;
                auto newContext = _tracker.createInstance<Rel2Context>(
                    _tracker.createInstance<RelExpContext>(parentContext, parentState));
                _localctx = newContext;
                pushNewRecursionContext(newContext, startState, RuleRelExp);
                setState(381);

                if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(382);
                relOp();
                setState(383);
                addExp(0);
            }
            setState(389);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- RelOpContext ------------------------------------------------------------------

SysYParser::RelOpContext::RelOpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::RelOpContext::LT() { return getToken(SysYParser::LT, 0); }

tree::TerminalNode *SysYParser::RelOpContext::GT() { return getToken(SysYParser::GT, 0); }

tree::TerminalNode *SysYParser::RelOpContext::LE() { return getToken(SysYParser::LE, 0); }

tree::TerminalNode *SysYParser::RelOpContext::GE() { return getToken(SysYParser::GE, 0); }

size_t SysYParser::RelOpContext::getRuleIndex() const { return SysYParser::RuleRelOp; }

std::any SysYParser::RelOpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitRelOp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::RelOpContext *SysYParser::relOp() {
    RelOpContext *_localctx = _tracker.createInstance<RelOpContext>(_ctx, getState());
    enterRule(_localctx, 80, SysYParser::RuleRelOp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(390);
        _la = _input->LA(1);
        if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 263882790666240) != 0))) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- EqExpContext ------------------------------------------------------------------

SysYParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::EqExpContext::getRuleIndex() const { return SysYParser::RuleEqExp; }

void SysYParser::EqExpContext::copyFrom(EqExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- Eq1Context ------------------------------------------------------------------

SysYParser::RelExpContext *SysYParser::Eq1Context::relExp() { return getRuleContext<SysYParser::RelExpContext>(0); }

SysYParser::Eq1Context::Eq1Context(EqExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Eq1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitEq1(this);
    else
        return visitor->visitChildren(this);
}
//----------------- Eq2Context ------------------------------------------------------------------

SysYParser::EqExpContext *SysYParser::Eq2Context::eqExp() { return getRuleContext<SysYParser::EqExpContext>(0); }

SysYParser::EqOpContext *SysYParser::Eq2Context::eqOp() { return getRuleContext<SysYParser::EqOpContext>(0); }

SysYParser::RelExpContext *SysYParser::Eq2Context::relExp() { return getRuleContext<SysYParser::RelExpContext>(0); }

SysYParser::Eq2Context::Eq2Context(EqExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::Eq2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitEq2(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::EqExpContext *SysYParser::eqExp() { return eqExp(0); }

SysYParser::EqExpContext *SysYParser::eqExp(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    SysYParser::EqExpContext *_localctx = _tracker.createInstance<EqExpContext>(_ctx, parentState);
    SysYParser::EqExpContext *previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 82;
    enterRecursionRule(_localctx, 82, SysYParser::RuleEqExp, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<Eq1Context>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(393);
        relExp(0);
        _ctx->stop = _input->LT(-1);
        setState(401);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty()) triggerExitRuleEvent();
                previousContext = _localctx;
                auto newContext = _tracker.createInstance<Eq2Context>(
                    _tracker.createInstance<EqExpContext>(parentContext, parentState));
                _localctx = newContext;
                pushNewRecursionContext(newContext, startState, RuleEqExp);
                setState(395);

                if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(396);
                eqOp();
                setState(397);
                relExp(0);
            }
            setState(403);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- EqOpContext ------------------------------------------------------------------

SysYParser::EqOpContext::EqOpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *SysYParser::EqOpContext::EQ() { return getToken(SysYParser::EQ, 0); }

tree::TerminalNode *SysYParser::EqOpContext::NEQ() { return getToken(SysYParser::NEQ, 0); }

size_t SysYParser::EqOpContext::getRuleIndex() const { return SysYParser::RuleEqOp; }

std::any SysYParser::EqOpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitEqOp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::EqOpContext *SysYParser::eqOp() {
    EqOpContext *_localctx = _tracker.createInstance<EqOpContext>(_ctx, getState());
    enterRule(_localctx, 84, SysYParser::RuleEqOp);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(404);
        _la = _input->LA(1);
        if (!(_la == SysYParser::EQ

              || _la == SysYParser::NEQ)) {
            _errHandler->recoverInline(this);
        } else {
            _errHandler->reportMatch(this);
            consume();
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- LAndExpContext ------------------------------------------------------------------

SysYParser::LAndExpContext::LAndExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::LAndExpContext::getRuleIndex() const { return SysYParser::RuleLAndExp; }

void SysYParser::LAndExpContext::copyFrom(LAndExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- LAnd2Context ------------------------------------------------------------------

SysYParser::LAndExpContext *SysYParser::LAnd2Context::lAndExp() {
    return getRuleContext<SysYParser::LAndExpContext>(0);
}

tree::TerminalNode *SysYParser::LAnd2Context::LAND() { return getToken(SysYParser::LAND, 0); }

SysYParser::EqExpContext *SysYParser::LAnd2Context::eqExp() { return getRuleContext<SysYParser::EqExpContext>(0); }

SysYParser::LAnd2Context::LAnd2Context(LAndExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::LAnd2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitLAnd2(this);
    else
        return visitor->visitChildren(this);
}
//----------------- LAnd1Context ------------------------------------------------------------------

SysYParser::EqExpContext *SysYParser::LAnd1Context::eqExp() { return getRuleContext<SysYParser::EqExpContext>(0); }

SysYParser::LAnd1Context::LAnd1Context(LAndExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::LAnd1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitLAnd1(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::LAndExpContext *SysYParser::lAndExp() { return lAndExp(0); }

SysYParser::LAndExpContext *SysYParser::lAndExp(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    SysYParser::LAndExpContext *_localctx = _tracker.createInstance<LAndExpContext>(_ctx, parentState);
    SysYParser::LAndExpContext *previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 86;
    enterRecursionRule(_localctx, 86, SysYParser::RuleLAndExp, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<LAnd1Context>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(407);
        eqExp(0);
        _ctx->stop = _input->LT(-1);
        setState(414);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty()) triggerExitRuleEvent();
                previousContext = _localctx;
                auto newContext = _tracker.createInstance<LAnd2Context>(
                    _tracker.createInstance<LAndExpContext>(parentContext, parentState));
                _localctx = newContext;
                pushNewRecursionContext(newContext, startState, RuleLAndExp);
                setState(409);

                if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(410);
                match(SysYParser::LAND);
                setState(411);
                eqExp(0);
            }
            setState(416);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- LOrExpContext ------------------------------------------------------------------

SysYParser::LOrExpContext::LOrExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t SysYParser::LOrExpContext::getRuleIndex() const { return SysYParser::RuleLOrExp; }

void SysYParser::LOrExpContext::copyFrom(LOrExpContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- LOr1Context ------------------------------------------------------------------

SysYParser::LAndExpContext *SysYParser::LOr1Context::lAndExp() { return getRuleContext<SysYParser::LAndExpContext>(0); }

SysYParser::LOr1Context::LOr1Context(LOrExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::LOr1Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitLOr1(this);
    else
        return visitor->visitChildren(this);
}
//----------------- LOr2Context ------------------------------------------------------------------

SysYParser::LOrExpContext *SysYParser::LOr2Context::lOrExp() { return getRuleContext<SysYParser::LOrExpContext>(0); }

tree::TerminalNode *SysYParser::LOr2Context::LOR() { return getToken(SysYParser::LOR, 0); }

SysYParser::LAndExpContext *SysYParser::LOr2Context::lAndExp() { return getRuleContext<SysYParser::LAndExpContext>(0); }

SysYParser::LOr2Context::LOr2Context(LOrExpContext *ctx) { copyFrom(ctx); }

std::any SysYParser::LOr2Context::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitLOr2(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::LOrExpContext *SysYParser::lOrExp() { return lOrExp(0); }

SysYParser::LOrExpContext *SysYParser::lOrExp(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    SysYParser::LOrExpContext *_localctx = _tracker.createInstance<LOrExpContext>(_ctx, parentState);
    SysYParser::LOrExpContext *previousContext = _localctx;
    (void)previousContext;  // Silence compiler, in case the context is not used by generated code.
    size_t startState = 88;
    enterRecursionRule(_localctx, 88, SysYParser::RuleLOrExp, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<LOr1Context>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(418);
        lAndExp(0);
        _ctx->stop = _input->LT(-1);
        setState(425);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty()) triggerExitRuleEvent();
                previousContext = _localctx;
                auto newContext = _tracker.createInstance<LOr2Context>(
                    _tracker.createInstance<LOrExpContext>(parentContext, parentState));
                _localctx = newContext;
                pushNewRecursionContext(newContext, startState, RuleLOrExp);
                setState(420);

                if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
                setState(421);
                match(SysYParser::LOR);
                setState(422);
                lAndExp(0);
            }
            setState(427);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- CondExpContext ------------------------------------------------------------------

SysYParser::CondExpContext::CondExpContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

SysYParser::LOrExpContext *SysYParser::CondExpContext::lOrExp() { return getRuleContext<SysYParser::LOrExpContext>(0); }

size_t SysYParser::CondExpContext::getRuleIndex() const { return SysYParser::RuleCondExp; }

std::any SysYParser::CondExpContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<SysYVisitor *>(visitor))
        return parserVisitor->visitCondExp(this);
    else
        return visitor->visitChildren(this);
}

SysYParser::CondExpContext *SysYParser::condExp() {
    CondExpContext *_localctx = _tracker.createInstance<CondExpContext>(_ctx, getState());
    enterRule(_localctx, 90, SysYParser::RuleCondExp);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(428);
        lOrExp(0);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

bool SysYParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
    switch (ruleIndex) {
        case 35:
            return mulExpSempred(antlrcpp::downCast<MulExpContext *>(context), predicateIndex);
        case 37:
            return addExpSempred(antlrcpp::downCast<AddExpContext *>(context), predicateIndex);
        case 39:
            return relExpSempred(antlrcpp::downCast<RelExpContext *>(context), predicateIndex);
        case 41:
            return eqExpSempred(antlrcpp::downCast<EqExpContext *>(context), predicateIndex);
        case 43:
            return lAndExpSempred(antlrcpp::downCast<LAndExpContext *>(context), predicateIndex);
        case 44:
            return lOrExpSempred(antlrcpp::downCast<LOrExpContext *>(context), predicateIndex);

        default:
            break;
    }
    return true;
}

bool SysYParser::mulExpSempred(MulExpContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
        case 0:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

bool SysYParser::addExpSempred(AddExpContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
        case 1:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

bool SysYParser::relExpSempred(RelExpContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
        case 2:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

bool SysYParser::eqExpSempred(EqExpContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
        case 3:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

bool SysYParser::lAndExpSempred(LAndExpContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
        case 4:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

bool SysYParser::lOrExpSempred(LOrExpContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
        case 5:
            return precpred(_ctx, 1);

        default:
            break;
    }
    return true;
}

void SysYParser::initialize() { ::antlr4::internal::call_once(sysyParserOnceFlag, sysyParserInitialize); }
