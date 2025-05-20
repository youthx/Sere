#pragma once
#ifndef PARSER_AST_VISITOR_HPP
#define PARSER_AST_VISITOR_HPP

#include "./Expr.hpp"
#include <typeinfo>
#include <stdexcept>

// Macro for marking unused variables (for compatibility)
#if defined(__GNUC__) || defined(__clang__)
    #define SEREPARSER_UNUSED(x) (void)(x)
#else
    #define SEREPARSER_UNUSED(x) (void)(x)
#endif

// Macro for nodiscard attribute (C++17+)
#if defined(__has_cpp_attribute)
    #if __has_cpp_attribute(nodiscard)
        #define SEREPARSER_NODISCARD [[nodiscard]]
    #else
        #define SEREPARSER_NODISCARD
    #endif
#else
    #define SEREPARSER_NODISCARD
#endif

// Macro for noexcept specification
#define SEREPARSER_NOEXCEPT noexcept(false)

// Macro for runtime return type check
#define ASSERT_MUST_RETURN_SERE_OBJECT \
    do { \
        if (typeid(R) != typeid(SereObject)) { \
            throw std::runtime_error("Implementation Error (Visitor): Invalid return type (" + std::string(typeid(R).name()) + ")."); \
        } \
    } while(0)

namespace SereParser {

    // Forward declare SereObject for type checking
    class SereObject;

    template <typename R>
    class ExprVisitor {
    public:
        virtual ~ExprVisitor() = default;

        // Visitor interface
        SEREPARSER_NODISCARD virtual R visit_binary(const class BinaryExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_unary(const class UnaryExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_literal(const class LiteralExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_logical(const class LogicalExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_assign(const class AssignExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_call(const class CallExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_group(const class GroupExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_super(const class SuperExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_self(const class SelfExprAST& expr) SEREPARSER_NOEXCEPT;
        SEREPARSER_NODISCARD virtual R visit_variable(const class VariableExprAST& expr) SEREPARSER_NOEXCEPT;

        // Accepts an expression and dispatches to the correct visit_* method
        SEREPARSER_NODISCARD R accept_expression(const class ExprAST& expr) {
            #ifndef SEREPARSER_AST_HPP
                #error "SereObject is not defined."
            #endif
            return expr.accept(*this);
        }
    };

    // Implementation of visitor methods

    template <typename R>
    R ExprVisitor<R>::visit_binary(const class BinaryExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;

        if (!expr.left || !expr.right) {
            throw std::invalid_argument("BinaryExprAST: left or right is null.");
        }

        R left_root = expr.left->accept(*this);
        R right_root = expr.right->accept(*this);

        switch (expr.op.type) {
            case SereLexer::TokenType::TOKEN_PLUS:
                left_root.perform_add(right_root);
                break;
            case SereLexer::TokenType::TOKEN_MINUS:
                left_root.perform_subtract(right_root);
                break;
            case SereLexer::TokenType::TOKEN_STAR:
                left_root.perform_multiply(right_root);
                break;
            case SereLexer::TokenType::TOKEN_SLASH:
                left_root.perform_divide(right_root);
                break;
            default:
                throw std::invalid_argument("BinaryExprAST: Invalid operator.");
        }

        return left_root;
    }

    template <typename R>
    R ExprVisitor<R>::visit_literal(const class LiteralExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        return expr.value;
    }

    template <typename R>
    R ExprVisitor<R>::visit_group(const class GroupExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        if (!expr.expr) {
            throw std::invalid_argument("GroupExprAST: expr is null.");
        }
        return this->accept_expression(*expr.expr);
    }

    template <typename R>
    R ExprVisitor<R>::visit_unary(const class UnaryExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;

        if (!expr.operand) {
            throw std::invalid_argument("UnaryExprAST: operand is null.");
        }

        R right_root = expr.operand->accept(*this);

        switch (expr.op.type) {
            case SereLexer::TokenType::TOKEN_MINUS:
                right_root.perform_negate();
                break;
            case SereLexer::TokenType::TOKEN_BANG:
                right_root.perform_not();
                break;
            default:
                throw std::invalid_argument("UnaryExprAST: Invalid operator.");
        }

        return right_root;
    }

    // Default implementations for not-yet-implemented visit_* methods
    template <typename R>
    R ExprVisitor<R>::visit_logical(const class LogicalExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_logical not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_assign(const class AssignExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_assign not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_call(const class CallExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_call not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_super(const class SuperExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_super not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_self(const class SelfExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_self not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_variable(const class VariableExprAST& expr) SEREPARSER_NOEXCEPT {
        ASSERT_MUST_RETURN_SERE_OBJECT;
        SEREPARSER_UNUSED(expr);
        throw std::runtime_error("visit_variable not implemented.");
    }

} // namespace SereParser

#endif // PARSER_AST_VISITOR_HPP