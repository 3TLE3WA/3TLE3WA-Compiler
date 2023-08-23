#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmGen.hh"
#include "3tle3wa/backend/asm/AsmGlobalValue.hh"
#include "3tle3wa/backend/asm/AsmLocalConstant.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLGen.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/frontend/AstVisitor.hh"
#include "3tle3wa/pass/Pass.hh"
#include "3tle3wa/utils/logs.hh"
#include "SysYLexer.h"
#include "SysYParser.h"

using namespace antlr4;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    bool pusage = false;

    const char *input = nullptr;
    const char *output = nullptr;
    const char *irfile = nullptr;
    const char *dbgfile = nullptr;

    CRVC_UNUSE char optlvl = 0;

    for (int ch; (ch = getopt(argc, argv, "SO:l:o:d:h")) != -1;) {
        switch (ch) {
            case 'l':
                Log("llvm ir filename: %s", optarg);
                irfile = optarg;
                break;
            case 'o':
                Log("rv asm filename: %s", optarg);
                output = optarg;
                break;
            case 'h':
                pusage = true;
                break;
            case 'd':
                Log("debug filename: %s", optarg);
                dbgfile = optarg;
                break;
            case 'O':
                Log("optimize level: %c", *optarg);
                optlvl = *optarg;
                break;
            case 'S':
                Log("output asm (current useless)");
                break;
            default:
                break;
        }
    }

    if (pusage) {
        std::cout << "Usage: compiler [options] <file>" << std::endl;
        std::cout << "only receive one file as input" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -d <file>   output dbg file" << std::endl;
        std::cout << "  -l <file>   output llvm ir" << std::endl;
        std::cout << "  -o <file>   output rv asm" << std::endl;
        std::cout << "  -h          this help" << std::endl;
        return 0;
    }

    if (optind < argc) {
        input = argv[optind];
    } else {
        panic("no input file");
    }

    std::ifstream src(input);

    Assert(src.is_open(), "cannot open input file %s", input);

    ANTLRInputStream source(src);
    SysYLexer lexer(&source);
    CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);
    parser.setErrorHandler(std::make_shared<BailErrorStrategy>());

    SysYParser::CompilationUnitContext *root = parser.compilationUnit();

    CompilationUnit comp_unit;

    Log("start parsing");

    std::unique_ptr<AstVisitor> visitor = std::make_unique<AstVisitor>(comp_unit);
    visitor->visitCompilationUnit(root);
    visitor = nullptr;

    Log("start llvm ir optimizing");

    Optimization optimizer(comp_unit);
    optimizer.DoOptimization();

    comp_unit.InsertConstantToGlbTable();

    if (irfile) {
        comp_unit.generatellvmIR(irfile);
    }

    if (output != nullptr or dbgfile != nullptr) {
        RLGen rlgen;
        rlgen.Register(comp_unit);

        Log("generating asm");

        rlgen.SerialGenerate();

        if (dbgfile != nullptr) {
            Log("output backend ir file");

            std::fstream fs(dbgfile, std::ios::out);
            fs << rlgen.CString();
        }

        if (output != nullptr) {
            Log("output asm file");

            std::fstream fs(output, std::ios::out);
            fs << rlgen.AG()->CString();
        }
    }

    Log("out main");
    return 0;
}