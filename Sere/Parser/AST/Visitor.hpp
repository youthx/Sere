#pragma once
#ifndef PARSER_AST_VISITOR_HPP
#define PARSER_AST_VISITOR_HPP

// =======================
// SereParser AST Visitor
// =======================

#include "./Expr.hpp"
#include "./Midlevel/SymbolTable.hpp"
#include "./Midlevel/Environments.hpp"
#include "../Builtins.hpp"
#include "../../IR/IR.hpp"

#include <typeinfo>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <utility>
#include <string>
#include <llvm/Support/raw_ostream.h>
//
// Attribute Macros
//
#if defined(__GNUC__) || defined(__clang__)
#define SEREPARSER_UNUSED(x) (void)(x)
#else
#define SEREPARSER_UNUSED(x) (void)(x)
#endif

#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(nodiscard)
#define SEREPARSER_NODISCARD [[nodiscard]]
#else
#define SEREPARSER_NODISCARD
#endif
#else
#define SEREPARSER_NODISCARD
#endif

#define SEREPARSER_NOEXCEPT noexcept(false)

#define ASSERT_MUST_RETURN_SERE_OBJECT                                                                                                \
    do                                                                                                                                \
    {                                                                                                                                 \
        if (typeid(R) != typeid(SereObject))                                                                                          \
        {                                                                                                                             \
            throw std::runtime_error("Implementation Error (Visitor): Invalid return type (" + std::string(typeid(R).name()) + ")."); \
        }                                                                                                                             \
    } while (0)

#define SANITIZE_NAME(name) _sanitize_name_impl_(name)

namespace SereParser
{

    inline std::string _sanitize_name_impl_(const std::string &name)
    {
        std::string clean_name;
        for (char c : name)
        {
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_')
            {
                clean_name += c;
            }
            else if (c == '_')
            {
                clean_name += '_';
            }
        }
        if (clean_name.empty())
        {
            clean_name = "var";
        }
        return clean_name;
    }

    class SereObject; // Forward declaration

    //
    // Runtime Table Holder
    //
    class RT
    {
    public:
        static inline SereIR::CodeGenContext ctx = SereIR::CodeGenContext();

        static inline Runtime::SymbolTable global = Runtime::SymbolTable();
        static inline std::shared_ptr<Runtime::TypeEnvironment> global_type_env = std::make_shared<Runtime::TypeEnvironment>();
    };

    inline llvm::Type *typename_to_llvm_type(const std::string &type_name)
    {
        if (type_name == "int")
            return llvm::Type::getInt64Ty(RT::ctx.llvm_ctx);
        if (type_name == "float")
            return llvm::Type::getFloatTy(RT::ctx.llvm_ctx);
        if (type_name == "bool")
            return llvm::Type::getInt1Ty(RT::ctx.llvm_ctx);
        if (type_name == "str")
            return llvm::Type::getInt8PtrTy(RT::ctx.llvm_ctx);
        if (type_name == "none")
            return llvm::Type::getVoidTy(RT::ctx.llvm_ctx);
        throw std::runtime_error("Unknown type for LLVM conversion: " + type_name);
    }

    inline llvm::Type *typekind_to_llvm_type(Runtime::SereTypeKind kind)
    {
        switch (kind)
        {
        case Runtime::SereTypeKind::INT:
            return llvm::Type::getInt64Ty(RT::ctx.llvm_ctx);
        case Runtime::SereTypeKind::FLOAT:
            return llvm::Type::getFloatTy(RT::ctx.llvm_ctx);
        case Runtime::SereTypeKind::BOOL:
            return llvm::Type::getInt1Ty(RT::ctx.llvm_ctx);
        case Runtime::SereTypeKind::STRING:
            return llvm::Type::getInt8PtrTy(RT::ctx.llvm_ctx);
        case Runtime::SereTypeKind::NONE:
            return llvm::Type::getVoidTy(RT::ctx.llvm_ctx);
        default:
            throw std::runtime_error("Unknown SereTypeKind for LLVM conversion.");
        }
    }

    inline Runtime::SereTypeKind parse_type_annotation(const std::string &type_name)
    {
        if (type_name == "int")
            return Runtime::SereTypeKind::INT;
        if (type_name == "float")
            return Runtime::SereTypeKind::FLOAT;
        if (type_name == "bool")
            return Runtime::SereTypeKind::BOOL;
        if (type_name == "str" || type_name == "string")
            return Runtime::SereTypeKind::STRING;
        if (type_name == "none")
            return Runtime::SereTypeKind::NONE;
        return Runtime::SereTypeKind::UNKNOWN; // Default case for unknown types
    }

    //
    // ===============================
    // Semantic Analysis: TypeChecker
    // ===============================
    //
    class TypeChecker : public std::enable_shared_from_this<TypeChecker>
    {
    public:
        using Ptr = std::shared_ptr<TypeChecker>;

        explicit TypeChecker()
            : env(RT::global_type_env) {}

        void set_env(const std::shared_ptr<Runtime::TypeEnvironment> &new_env)
        {
            env = new_env;
        }

        void push_scope()
        {
            env = env->create_child_scope();
        }

        void pop_scope()
        {
            if (!env->parent_env)
                throw std::runtime_error("TypeChecker: No parent environment to pop to.");
            env = env->parent_env;
        }

        // Type Checking Entrypoints
        Runtime::SereTypeKind check_literal(const SereObject &obj)
        {
            switch (obj.getType())
            {
            case SereObjectType::INTEGER:
                return Runtime::SereTypeKind::INT;
            case SereObjectType::FLOAT:
                return Runtime::SereTypeKind::FLOAT;
            case SereObjectType::STRING:
                return Runtime::SereTypeKind::STRING;
            case SereObjectType::BOOLEAN:
                return Runtime::SereTypeKind::BOOL;
            case SereObjectType::NONE:
                return Runtime::SereTypeKind::NONE;
            default:
                return Runtime::SereTypeKind::UNKNOWN;
            }
        }

        Runtime::SereTypeKind check_variable(const std::string &name)
        {
            return env->get_(name);
        }

        Runtime::SereTypeKind check_binary(Runtime::SereTypeKind left, Runtime::SereTypeKind right, const std::string &op)
        {
            if (left == right)
                return left;
            if ((left == Runtime::SereTypeKind::INT && right == Runtime::SereTypeKind::FLOAT) ||
                (left == Runtime::SereTypeKind::FLOAT && right == Runtime::SereTypeKind::INT))
                return Runtime::SereTypeKind::FLOAT;
            throw std::runtime_error("Type error: invalid operands for " + op + ": " +
                                     Runtime::to_string(left) + " and " + Runtime::to_string(right));
        }

        Runtime::SereTypeKind check_unary(Runtime::SereTypeKind operand, const std::string &op)
        {
            if (op == "-" && (operand == Runtime::SereTypeKind::INT || operand == Runtime::SereTypeKind::FLOAT))
                return operand;
            if (op == "!" && operand == Runtime::SereTypeKind::BOOL)
                return Runtime::SereTypeKind::BOOL;
            throw std::runtime_error("Type error: invalid operand for " + op + ": " + Runtime::to_string(operand));
        }

        void check_assign(const std::string &name, Runtime::SereTypeKind rhs_type)
        {
            env->set(name, rhs_type);
        }

        void debug_dump_env() const
        {
            std::cout << "[TypeChecker] Current Environment:\n";
            for (const auto &pair : env->table)
            {
                std::cout << "  " << pair.first << " : " << Runtime::to_string(pair.second) << "\n";
            }
        }

        std::shared_ptr<Runtime::TypeEnvironment> env;
    };

    //
    // ===============================
    // ExprVisitor Template
    // ===============================
    //
    template <typename R>
    class ExprVisitor
    {
    public:
        virtual ~ExprVisitor() = default;
        std::shared_ptr<TypeChecker> type_checker;

        explicit ExprVisitor(std::shared_ptr<TypeChecker> checker)
            : type_checker(std::move(checker))
        {
        }

        // Visitor interface
        SEREPARSER_NODISCARD virtual R visit_binary(const class BinaryExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_unary(const class UnaryExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_literal(const class LiteralExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_logical(const class LogicalExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_call(const class CallExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_group(const class GroupExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_super(const class SuperExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_self(const class SelfExprAST &expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_variable(const class VariableExprAST &expr) SEREPARSER_NOEXCEPT;

        SEREPARSER_NODISCARD R accept_expression(const class ExprAST &expr)
        {
            return expr.accept(*this);
        }
    };

    //
    // ===============================
    // StatVisitor Template
    // ===============================
    //
    template <typename R>
    class StatVisitor
    {
    public:
        virtual ~StatVisitor() = default;
        std::shared_ptr<ExprVisitor<R>> expr_visitor;
        std::shared_ptr<TypeChecker> type_checker;

        explicit StatVisitor(std::shared_ptr<ExprVisitor<R>> expr_visitor_)
            : expr_visitor(std::move(expr_visitor_)),
              type_checker(std::make_shared<TypeChecker>())
        {
            type_checker->set_env(RT::global_type_env);
        }

        SEREPARSER_NODISCARD virtual R visit_block(const class BlockStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_class(const class ClassStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_expr(const class ExprStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_function(const class FunctionStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_if(const class IfStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_while(const class WhileStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_return(const class ReturnStatAST &stat) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_assign(const class AssignStatAST &stat) SEREPARSER_NOEXCEPT;

        SEREPARSER_NODISCARD R accept_statement(class StatAST &stat)
        {
            return stat.accept(this);
        }
    };

    //
    // ===============================
    // ExprVisitor Implementations
    // ===============================
    //
    template <typename R>
    R ExprVisitor<R>::visit_binary(const BinaryExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;

        if (!expr.left || !expr.right)
            throw std::invalid_argument("BinaryExprAST: left or right is null.");

        auto left_val = expr.left->accept(*this);
        auto right_val = expr.right->accept(*this);

        llvm::Value *left_llvm = left_val.getLLVMValue(&RT::ctx.llvm_ctx);
        llvm::Value *right_llvm = right_val.getLLVMValue(&RT::ctx.llvm_ctx);
        if (!left_llvm || !right_llvm)
            throw std::runtime_error("BinaryExprAST: LLVM values are not valid.");

        // Semantic Type Check (assumed to return some type info, like enum or LLVM type)
        auto left_type = left_llvm->getType();
        auto right_type = right_llvm->getType();

        if (left_type != right_type) {
            throw std::runtime_error("Type mismatch in binary operation.");
        }

        bool isFloat = left_type->isFloatingPointTy();

        switch (expr.op.type)
        {
            case SereLexer::TokenType::TOKEN_PLUS:
                if (isFloat)
                    left_val.setLLVMValue(RT::ctx.builder.CreateFAdd(left_llvm, right_llvm, "add_tmp"));
                else
                    left_val.setLLVMValue(RT::ctx.builder.CreateAdd(left_llvm, right_llvm, "add_tmp"));
                break;

            case SereLexer::TokenType::TOKEN_MINUS:
                if (isFloat)
                    left_val.setLLVMValue(RT::ctx.builder.CreateFSub(left_llvm, right_llvm, "sub_tmp"));
                else
                    left_val.setLLVMValue(RT::ctx.builder.CreateSub(left_llvm, right_llvm, "sub_tmp"));
                break;

            case SereLexer::TokenType::TOKEN_STAR:
                if (isFloat)
                    left_val.setLLVMValue(RT::ctx.builder.CreateFMul(left_llvm, right_llvm, "mul_tmp"));
                else
                    left_val.setLLVMValue(RT::ctx.builder.CreateMul(left_llvm, right_llvm, "mul_tmp"));
                break;

            case SereLexer::TokenType::TOKEN_SLASH:
                if (isFloat)
                    left_val.setLLVMValue(RT::ctx.builder.CreateFDiv(left_llvm, right_llvm, "div_tmp"));
                else
                    // Assuming signed integer division here, use CreateUDiv if unsigned
                    left_val.setLLVMValue(RT::ctx.builder.CreateSDiv(left_llvm, right_llvm, "div_tmp"));
                break;

            default:
                throw std::invalid_argument("BinaryExprAST: Invalid operator.");
        }

        return left_val;
    }


    template <typename R>
    R ExprVisitor<R>::visit_literal(const LiteralExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        type_checker->check_literal(expr.value);

        SereObject value = expr.value; // copy

        switch (value.getType())
        {
        case SereObjectType::INTEGER:
            value.setLLVMValue(
                llvm::ConstantInt::get(RT::ctx.llvm_ctx, llvm::APInt(64, value.getInteger())));
            break;
        case SereObjectType::FLOAT:
            value.setLLVMValue(
                llvm::ConstantFP::get(RT::ctx.llvm_ctx, llvm::APFloat(value.getFloat())));
            break;
        case SereObjectType::STRING: {
            llvm::Constant *str_const = llvm::ConstantDataArray::getString(RT::ctx.llvm_ctx, value.getString(), true);
            
            llvm::GlobalVariable *str_global = new llvm::GlobalVariable(
                *RT::ctx.get_module(),
                str_const->getType(),
                false,
                llvm::GlobalValue::PrivateLinkage,
                str_const,
                ".str"
            );

            llvm::Constant *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(RT::ctx.llvm_ctx), 0);
            llvm::Constant *indices[] = { zero, zero };
            
            llvm::Constant *str_ptr = llvm::ConstantExpr::getGetElementPtr(
                str_const->getType(),
                str_global,
                indices
            );

            value.setLLVMValue(str_ptr); // ✅ now it's a valid i8* pointer
            break;
        }
        case SereObjectType::BOOLEAN:
            value.setLLVMValue(
                llvm::ConstantInt::get(RT::ctx.llvm_ctx, llvm::APInt(1, value.getBoolean())));
            break;
        case SereObjectType::NONE:
            value.setLLVMValue(
                llvm::Constant::getNullValue(llvm::Type::getVoidTy(RT::ctx.llvm_ctx)));
            break;
        case SereObjectType::SYMBOLIC:
            break;
        }
        return value;
    }

    template <typename R>
    R ExprVisitor<R>::visit_group(const GroupExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        if (!expr.expr)
            throw std::invalid_argument("GroupExprAST: expr is null.");
        return expr.expr->accept(*this);
    }

    template <typename R>
    R ExprVisitor<R>::visit_unary(const UnaryExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        if (!expr.operand)
            throw std::invalid_argument("UnaryExprAST: operand is null.");

        R val = expr.operand->accept(*this);
        llvm::Value *operand_llvm = val.getLLVMValue(&RT::ctx.llvm_ctx);

        switch (expr.op.type)
        {
        case SereLexer::TokenType::TOKEN_MINUS:
            val.setLLVMValue(RT::ctx.builder.CreateFNeg(operand_llvm, "neg_tmp"));
        case SereLexer::TokenType::TOKEN_BANG:
            val.setLLVMValue(RT::ctx.builder.CreateNot(operand_llvm, "not_tmp"));
        default:
            throw std::invalid_argument("UnaryExprAST: Invalid operator.");
        }
        return val;
    }

    template <typename R>
    R ExprVisitor<R>::visit_variable(const VariableExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;

        llvm::Value *var_ptr = RT::ctx.get_named_value(SANITIZE_NAME(expr.name.lexeme));
        if (!var_ptr)
        {
            throw std::runtime_error("LLVM variable '" + expr.name.lexeme + "' not found in current scope.");
        }
       

        llvm::Value *loaded = RT::ctx.builder.CreateLoad(var_ptr->getType()->getPointerElementType(), var_ptr, SANITIZE_NAME(expr.name.lexeme));

        SereObject result;
        result.setLLVMValue(loaded);
        return result;
    }
    // --- Not-yet-implemented visit_* methods ---
    template <typename R>
    R ExprVisitor<R>::visit_logical(const LogicalExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_logical not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_call(const CallExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        
        llvm::Function *callee = RT::ctx.module->getFunction(SANITIZE_NAME(expr.callee.lexeme));
        if (!callee) {
            throw std::runtime_error(SANITIZE_NAME(expr.callee.lexeme) + " is not defined in the current scope.");
        }

        llvm::FunctionType *func_type = callee->getFunctionType();
        unsigned expected_count = func_type->getNumParams();
        bool isVarArg = func_type->isVarArg();
        
        if (!isVarArg && expr.arguments.size() != expected_count) {
            throw std::runtime_error("argument count mismatch in function call to " + SANITIZE_NAME(expr.callee.lexeme));
        }

        std::vector<llvm::Value*> argsV;
        for (const auto& argExpr : expr.arguments) {
            auto argVal = argExpr->accept(*this);
            llvm::Value *val = argVal.getLLVMValue(&RT::ctx.llvm_ctx);
            if (!val) {
                throw std::runtime_error("Invalid LLVM value for argument in function call.");
            }
            argsV.push_back(val);
        }

        for (unsigned i = 0; i < expected_count; ++i) {
            llvm::Type *expected_type = func_type->getParamType(i);
            llvm::Value *arg_value = argsV[i];
            if (arg_value->getType() != expected_type) {
                throw std::runtime_error("Argument " + std::to_string(i) + " type mismatch in function call to " + SANITIZE_NAME(expr.callee.lexeme));
            }
        }

        llvm::Value *call_inst = RT::ctx.builder.CreateCall(callee, argsV);
        SereObject result;
        result.setLLVMValue(call_inst);
        return result;
    }

    template <typename R>
    R ExprVisitor<R>::visit_super(const SuperExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_super not implemented.");
    }
    template <typename R>
    R ExprVisitor<R>::visit_self(const SelfExprAST &expr) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_self not implemented.");
    }

    //
    // ===============================
    // StatVisitor Implementations
    // ===============================
    //
    template <typename R>
    R StatVisitor<R>::visit_assign(const AssignStatAST &stat) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;

        const std::string name = SANITIZE_NAME(stat.name.lexeme);
        SereObject value = stat.initializer ? stat.initializer->accept(*expr_visitor) : SereObject();
        llvm::Value *value_llvm = value.getLLVMValue(&RT::ctx.llvm_ctx);
        if (!value_llvm)
        {
            throw std::runtime_error("Assign: invalid LLVM value.");
        }

        llvm::Value *alloc = RT::ctx.get_named_value(name);

        Runtime::SereTypeKind inferred_type = type_checker->check_literal(value);
        llvm::Type *inferred_llvm_type = value.getLLVMValue(&RT::ctx.llvm_ctx)->getType();

        if (!alloc)
        {
            if (!RT::ctx.function)
                throw std::runtime_error("Assign: No function context.");
            llvm::IRBuilder<> tmp_builder(&RT::ctx.function->getEntryBlock(), RT::ctx.function->getEntryBlock().begin());

            // Optional explicit annotation
            if (stat.type_annotation)
            {
                auto annotated_type = SANITIZE_NAME(stat.type_annotation->name.lexeme);
                llvm::Type *annotated_llvm_type = typename_to_llvm_type(annotated_type);
                Runtime::SereTypeKind annotated_sere_type = parse_type_annotation(annotated_type);

                if (annotated_sere_type != inferred_type)
                {
                    throw std::runtime_error("Type annotation mismatch for variable '" + name + "'");
                }

                inferred_type = annotated_sere_type;
                inferred_llvm_type = annotated_llvm_type;
            }

            alloc = tmp_builder.CreateAlloca(inferred_llvm_type, nullptr, name);
            type_checker->check_assign(name, inferred_type);

            RT::ctx.set_named_value(name, alloc);
        }

        if (!alloc->getType()->isPointerTy()) {
            throw std::runtime_error("Alloc for variable '" + name + "' is not a pointer type!");
        }

        // Cast value_llvm if necessary to match alloc type
        llvm::Type *dest_type = alloc->getType()->getPointerElementType();
        if (value_llvm->getType() != dest_type)
        {
            // Try a basic cast — this is simplified for numeric types only
            if (value_llvm->getType()->isIntegerTy() && dest_type->isIntegerTy())
            {
                value_llvm = RT::ctx.builder.CreateTruncOrBitCast(value_llvm, dest_type, name + "_cast");
            }
            else if (value_llvm->getType()->isFloatingPointTy() && dest_type->isFloatingPointTy())
            {
                value_llvm = RT::ctx.builder.CreateFPCast(value_llvm, dest_type, name + "_cast");
            }
            else
            {
            /*
                llvm::errs() << "Assigning to variable '" << name << "'\n";
                llvm::errs() << "Value type: "; value_llvm->getType()->print(llvm::errs());
                llvm::errs() << "\nDestination type: "; dest_type->print(llvm::errs());
                llvm::errs() << "\n";
            */
                throw std::runtime_error("Cannot cast value for assignment to variable '" + name + "'");
            }
        }

        RT::ctx.builder.CreateStore(value_llvm, alloc);
        value.setLLVMValue(value_llvm);
        return value;
    }

    template <typename R>
    R StatVisitor<R>::visit_expr(const ExprStatAST &stat) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        if (!stat.expr)
            throw std::invalid_argument("ExprStatAST: expr is null.");
        return stat.expr->accept(*expr_visitor);
    }

    template <typename R>
    R StatVisitor<R>::visit_block(const BlockStatAST &stat) SEREPARSER_NOEXCEPT
    {
        // Never push/pop scope for the outermost block!
        static bool seen_first_block = false;
        bool should_push = seen_first_block;
        if (!seen_first_block)
            seen_first_block = true;

        if (should_push)
            type_checker->push_scope();
        SereObject last_value;
        for (auto &statement : stat.statements)
        {
            last_value = statement->accept(*this);
        }
        if (should_push)
            type_checker->pop_scope();
        return last_value;
    }

    template <typename R>
    R StatVisitor<R>::visit_class(const ClassStatAST &stat) SEREPARSER_NOEXCEPT
    {
        type_checker->push_scope();
        // TODO: Implement class fields and methods
        type_checker->pop_scope();
        SEREPARSER_UNUSED(stat);
        throw std::runtime_error("visit_class not implemented.");
    }

    template <typename R>
    R StatVisitor<R>::visit_function(const FunctionStatAST &func) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        std::string func_name = func.name.lexeme;
        bool is_main = (func_name == "main");
        if (is_main)
            func_name = "__main__";
        func_name = SANITIZE_NAME(func_name);

        std::vector<llvm::Type *> arg_types;
        for (const auto &param : func.params)
        {
            if (!param->type_annotation)
                throw std::runtime_error("Function parameter '" + param->name.lexeme + "' is missing type annotation.");

            llvm::Type *kind = typename_to_llvm_type(SANITIZE_NAME(param->type_annotation->name.lexeme));
            if (!kind)
            {
                throw std::runtime_error("Function parameter '" + param->name.lexeme + "' has an invalid type.");
            }
            arg_types.push_back(kind);
        }
        llvm::Type *return_type = llvm::Type::getVoidTy(RT::ctx.llvm_ctx);
        if (func.type_annotation)
        {
            return_type = typename_to_llvm_type(SANITIZE_NAME(func.type_annotation->name.lexeme));
            if (!return_type)
            {
                throw std::runtime_error("Function return type '" + func.type_annotation->name.lexeme + "' is invalid.");
            }
        }
        llvm::FunctionType *func_type = llvm::FunctionType::get(
            return_type, arg_types, false // Not varargs
        );

        llvm::Function *llvm_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_name, RT::ctx.module.get());

        if (is_main)
            RT::ctx.entry_point = llvm_func;

        llvm::BasicBlock *entry = llvm::BasicBlock::Create(RT::ctx.llvm_ctx, "entry", llvm_func);
        RT::ctx.builder.SetInsertPoint(entry);
        RT::ctx.function = llvm_func;

        RT::ctx.push_scope();
        type_checker->push_scope();

        for (unsigned idx = 0; idx < llvm_func->arg_size(); ++idx)
        {
            auto &arg = *llvm_func->getArg(idx);
            auto param = func.params[idx];
            const std::string param_name = SANITIZE_NAME(param->name.lexeme);
            arg.setName(param_name);

            llvm::AllocaInst *alloca = RT::ctx.builder.CreateAlloca(arg.getType(), nullptr, param_name);
            RT::ctx.builder.CreateStore(&arg, alloca);
            RT::ctx.set_named_value(param_name, alloca);
        }

        func.body->accept(*this);

        llvm::BasicBlock* current_block = RT::ctx.builder.GetInsertBlock();
        if (!current_block->getTerminator()) {
            if (return_type->isVoidTy()) {
                RT::ctx.builder.CreateRetVoid();
            } else if (return_type->isIntegerTy()) {
                RT::ctx.builder.CreateRet(llvm::ConstantInt::get(return_type, 0));
            } else if (return_type->isFloatingPointTy()) {
                RT::ctx.builder.CreateRet(llvm::ConstantFP::get(return_type, 0.0));
            } else if (return_type->isPointerTy()) {
                RT::ctx.builder.CreateRet(llvm::Constant::getNullValue(return_type));
            } else {
                throw std::runtime_error("Unhandled return type for default return.");
            }
        }
        RT::ctx.pop_scope();
        type_checker->pop_scope();

        SereObject obj;
        obj.setLLVMValue(llvm_func);
        return obj;
    }

    template <typename R>
    R StatVisitor<R>::visit_if(const IfStatAST &stat) SEREPARSER_NOEXCEPT
    {
        SEREPARSER_UNUSED(stat);
        throw std::runtime_error("visit_if not implemented.");
    }
    template <typename R>
    R StatVisitor<R>::visit_while(const WhileStatAST &stat) SEREPARSER_NOEXCEPT
    {
        SEREPARSER_UNUSED(stat);
        throw std::runtime_error("visit_while not implemented.");
    }

    template <typename R>
    R StatVisitor<R>::visit_return(const ReturnStatAST &stat) SEREPARSER_NOEXCEPT
    {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        if (stat.value)
        {
            SereObject return_value = stat.value->accept(*expr_visitor);
            
            llvm::Value *return_llvm = return_value.getLLVMValue(&RT::ctx.llvm_ctx);
            if (!return_llvm)
            {
                throw std::runtime_error("ReturnStatAST: Return value LLVM is not valid.");
            }

            RT::ctx.builder.CreateRet(return_llvm);

            return return_value;
        }
        else
        {
            RT::ctx.builder.CreateRetVoid();
            return SereObject(); // Return void
        }
    }

} // namespace SereParser

#endif // PARSER_AST_VISITOR_HPP