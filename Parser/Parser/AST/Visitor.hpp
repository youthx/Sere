#ifndef PARSER_AST_VISITOR_HPP
#define PARSER_AST_VISITOR_HPP

#include "./Expr.hpp"
#include <typeinfo>
#include <stdexcept>

#define ASSERT_MUST_RETURN_SERE_OBJECT \
    if (typeid(R) != typeid(SereObject)) { \
        throw std::runtime_error("Implementation Error (Visitor): Invalid return type."); \
    }

namespace SereParser {

    // Forward declare SereObject for type checking
    class SereObject;

    template <typename R>
    class ExprVisitor {
    public:
        virtual ~ExprVisitor() = default;

        // Mark functions as noexcept(false) to clarify they may throw
        virtual R visit_binary(const class BinaryExprAST& expr) noexcept(false);
        virtual R visit_unary(const class UnaryExprAST& expr) noexcept(false);
        virtual R visit_literal(const class LiteralExprAST& expr) noexcept(false);
        virtual R visit_logical(const class LogicalExprAST& expr) noexcept(false);
        virtual R visit_assign(const class AssignExprAST& expr) noexcept(false);
        virtual R visit_call(const class CallExprAST& expr) noexcept(false);
        virtual R visit_group(const class GroupExprAST& expr) noexcept(false);
        virtual R visit_super(const class SuperExprAST& expr) noexcept(false);
        virtual R visit_self(const class SelfExprAST& expr) noexcept(false);
        virtual R visit_variable(const class VariableExprAST& expr) noexcept(false);

        R accept_expression(const class ExprAST& expr) {
            #ifndef SEREPARSER_AST_HPP
                #error "SereObject is not defined."
            #endif 
            // (Assuming ExprAST is not a pointer here)
            return expr.accept(*this);
        }
    };

    template <typename R>
    R ExprVisitor<R>::visit_binary(const class BinaryExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        
        if (!expr.left || !expr.right) {
            throw std::invalid_argument("BinaryExprAST: left or right is null.");
        }

        R left_root = expr.left->accept(*this);
        R right_root = expr.right->accept(*this);

        /*
        printf("BinaryExprAST: %s %s %s\n", expr.op.lexeme.c_str(), left_root.to_string().c_str(), right_root.to_string().c_str());
        throw std::runtime_error("visit_binary not implemented any further.");
        */

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
    R ExprVisitor<R>::visit_literal(const class LiteralExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        return expr.value;
    }

    template <typename R>
    R ExprVisitor<R>::visit_group(const class GroupExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        if (!expr.expr) {
            throw std::invalid_argument("GroupExprAST: expr is null.");
        }
        return this->accept_expression(*expr.expr);
    }

    template <typename R>
    R ExprVisitor<R>::visit_unary(const class UnaryExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT

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

    // Optionally, provide default implementations for other visit_* methods
    // that throw, to avoid undefined behavior if not overridden.

    template <typename R>
    R ExprVisitor<R>::visit_logical(const class LogicalExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        throw std::runtime_error("visit_logical not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_assign(const class AssignExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        throw std::runtime_error("visit_assign not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_call(const class CallExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        throw std::runtime_error("visit_call not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_super(const class SuperExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        throw std::runtime_error("visit_super not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_self(const class SelfExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        throw std::runtime_error("visit_self not implemented.");
    }

    template <typename R>
    R ExprVisitor<R>::visit_variable(const class VariableExprAST& expr) noexcept(false) {
        ASSERT_MUST_RETURN_SERE_OBJECT
        throw std::runtime_error("visit_variable not implemented.");
    }

} // namespace SereParser

#endif // PARSER_AST_VISITOR_HPP