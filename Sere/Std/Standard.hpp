#ifndef STD_STANDARD_HPP
#define STD_STANDARD_HPP

#include <string>
#include <stdexcept>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

#include "../Parser/AST/Visitor.hpp"
namespace SereLib {

    llvm::Function* declare_printf(llvm::Module& module, llvm::LLVMContext& context);
    llvm::Value* print_init_builtin(llvm::Module& module, llvm::LLVMContext& context);
    void init_core(llvm::Module& module, llvm::LLVMContext& context);

    llvm::Function* declare_printf(llvm::Module& module, llvm::LLVMContext& context) {
        static llvm::Function* printfFunc = nullptr;
        if (printfFunc) return printfFunc; // already declared

        llvm::FunctionType* printfType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),                                  // returns int
            llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0),        // char*
            true                                                              // variadic
        );

        printfFunc = llvm::Function::Create(
            printfType, llvm::Function::ExternalLinkage, "printf", &module
        );

        return printfFunc;
    }

    llvm::Value* print_init_builtin(llvm::Module& module, llvm::LLVMContext& context) {
        llvm::IRBuilder<> builder(context);

        // Function type: void (i8*)
        llvm::FunctionType* print_ty = llvm::FunctionType::get(
            llvm::Type::getVoidTy(context),
            { llvm::Type::getInt8PtrTy(context) }, // single i8* argument
            false
        );

        llvm::Function* print_def = llvm::Function::Create(
            print_ty, llvm::Function::ExternalLinkage, "print", &module
        );

        llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", print_def);
        builder.SetInsertPoint(entry);

        llvm::Function* printf_func = declare_printf(module, context);

        llvm::Argument* arg_str = &*print_def->arg_begin();
        arg_str->setName("_format");

        llvm::Constant* format_str = llvm::ConstantDataArray::getString(context, "%s\n", true);
        llvm::GlobalVariable* format_var = new llvm::GlobalVariable(
            module,
            format_str->getType(),
            true,
            llvm::GlobalValue::PrivateLinkage,
            format_str,
            ".str"
        );

        llvm::Constant* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
        llvm::Constant* indices[] = { zero, zero };
        llvm::Constant* format_str_ptr = llvm::ConstantExpr::getGetElementPtr(
            format_str->getType(), format_var, indices
        );

        builder.CreateCall(printf_func, { format_str_ptr, arg_str });
        builder.CreateRetVoid();

        return print_def;
    }

    void init_core(llvm::Module& module, llvm::LLVMContext& context) {
        SereParser::RT::ctx.set_named_value("print", print_init_builtin(module, context));
    }

}

#endif // STD_STANDARD_HPP
