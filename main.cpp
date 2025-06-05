#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string> 
#include <cstring>
#include <assert.h>

#include "errors.hpp"
#include "./Sere/Scanner/Token.hpp"
#include "./Sere/Scanner/Scanner.hpp"
#include "./Sere/Parser/Parser.hpp"
#include "./Sere/Parser/AST/Visitor.hpp"
#include "./Sere/Parser/AST/AST.hpp"
#include "./Sere/Parser/AST/Midlevel/Environments.hpp"
#include "./Sere/IR/CodeGenContext.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>

#include "./Sere/Std/Registry.hpp"

int compile_optimization_passes(llvm::Module *module, llvm::LLVMContext &context)
{
    if (module == nullptr) throw std::invalid_argument("Module is null");
    llvm::SMDiagnostic error;

    // verify the pre-optimized module
    if (llvm::verifyModule(*module, &llvm::errs())) {
        llvm::errs() << "Module verification failed before optimization.\n";
        return -1;
    }
    

    llvm::legacy::PassManager passManager;

    /* ========= OPTIMIZATION PIPELINE ========= //

        IN -> mem2reg -> simply -> reassociate -> GVN -> CFGS -> OPTIMIZED OUTPUT

    */ 
    passManager.add(llvm::createPromoteMemoryToRegisterPass()); // mem2reg
    passManager.add(llvm::createInstructionCombiningPass()); // combine redundant instructions
    passManager.add(llvm::createReassociatePass()); // reorder expressions
    passManager.add(llvm::createGVNPass()); // Eliminate common subexpressions
    passManager.add(llvm::createCFGSimplificationPass()); // Control flow graph cleanup

    passManager.run(*module);
    
    // verify the optimized module
    if (llvm::verifyModule(*module, &llvm::errs())) {
        llvm::errs() << "Module verification failed after attempting optimization.\n";
        return -1;
    }

    module->print(llvm::outs(), nullptr);
    return 0;
}



std::vector<unsigned char> sere_read_file(const char *filepath)
{
    if (filepath == nullptr)
    {
        throw std::invalid_argument("Filepath is null");
    }

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw std::runtime_error("File not found or could not be opened");
    }

    std::streamsize size = file.tellg();
    if (size < 0)
    {
        throw std::runtime_error("Failed to determine file size");
    }
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        throw std::runtime_error("File read error");
    }

    return buffer;
}

int main(int argc, char *argv[])
{


    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: \n\t" << argv[0] << " <input_file>" << std::endl;
            return 64;
        }

        const char *filepath = argv[1];
        if (filepath == nullptr || filepath[0] == '\0')
        {
            std::cerr << "Invalid file path provided" << std::endl;
            return 65;
        }

        std::vector<unsigned char> buffer = sere_read_file(filepath);
        SereLexer::Scanner scanner(reinterpret_cast<const char *>(buffer.data()));
        SereLexer::TokenList tokens = scanner.tokenize();
        SereParser::Parser parser(tokens);
        auto stats = parser.parse();
        if (!stats.empty())
        {
            
        
            SereLib::load_registry();
            SereLib::include_lib("core");
            
            auto type_checker = std::make_shared<SereParser::TypeChecker>();
            auto expr_visitor = std::make_shared<SereParser::ExprVisitor<SereParser::SereObject>>(type_checker);
            auto visitor = std::make_shared<SereParser::StatVisitor<SereParser::SereObject>>(expr_visitor);
            for (auto stat : stats) {
                SereParser::SereObject result = stat.get()->accept(*visitor);
            }

            auto module = SereParser::RT::ctx.get_module();
            llvm::LLVMContext &context = module->getContext();
            

            compile_optimization_passes(module, context);
        }
        else
        {
            std::cerr << "Failed to parse expression." << std::endl;
            return 66;
        }
        
        /*
        for (const auto &token : tokens.getTokens())
        {
            std::cout << "Token: " << token->type << ", Lexeme: " << token->lexeme << std::endl;
        }

        */
        // Uncomment and implement the following lines if needed
        // SereLexer::Lexer lexer(buffer);
        // SereToken::TokenList tokens = lexer.tokenize();

        // for (const auto& token : tokens) {
        //     std::cout << "Token: " << token.type << ", Value: " << token.value << std::endl;
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred" << std::endl;
        return 2;
    }

    return 0;
}