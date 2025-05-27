#pragma once
#ifndef PARSER_AST_VISITOR_HPP
#define PARSER_AST_VISITOR_HPP

#include "./Expr.hpp"
#include <typeinfo>
#include <stdexcept>
#include "./Midlevel/SymbolTable.hpp"

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

#define ASSERT_MUST_RETURN_SERE_OBJECT \
    do { \
        if (typeid(R) != typeid(SereObject)) { \
            throw std::runtime_error("Implementation Error (Visitor): Invalid return type (" + std::string(typeid(R).name()) + ")."); \
        } \
    } while(0)

namespace SereParser {

class SereObject; // Forward declare

class RT {
public:
    static inline Runtime::SymbolTable global = Runtime::SymbolTable();
};

template <typename R>
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    // Visitor interface
    SEREPARSER_NODISCARD virtual R visit_binary(const class BinaryExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_unary(const class UnaryExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_literal(const class LiteralExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_logical(const class LogicalExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_call(const class CallExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_group(const class GroupExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_super(const class SuperExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_self(const class SelfExprAST& expr) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_variable(const class VariableExprAST& expr) SEREPARSER_NOEXCEPT;

    SEREPARSER_NODISCARD R accept_expression(const class ExprAST& expr) {
        return expr.accept(*this);
    }
};

template <typename R>
class StatVisitor {
public:
    virtual ~StatVisitor() = default;
    const std::shared_ptr<ExprVisitor<R>> expr_visitor;

    StatVisitor(std::shared_ptr<ExprVisitor<R>>& _expr_visitor) : expr_visitor(_expr_visitor) {}

    SEREPARSER_NODISCARD virtual R visit_block(const class BlockStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_class(const class ClassStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_expr(const class ExprStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_function(const class FunctionStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_if(const class IfStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_while(const class WhileStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_return(const class ReturnStatAST& stat) SEREPARSER_NOEXCEPT;
    SEREPARSER_NODISCARD virtual R visit_assign(const class AssignStatAST& stat) SEREPARSER_NOEXCEPT;

    SEREPARSER_NODISCARD R accept_statement(const class StatAST& stat) {
        return stat.accept(this);
    }
};

// ================= ExprVisitor Implementations =================

template <typename R>
R ExprVisitor<R>::visit_binary(const BinaryExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;

    if (!expr.left || !expr.right)
        throw std::invalid_argument("BinaryExprAST: left or right is null.");

    R left_val = expr.left->accept(*this);
    R right_val = expr.right->accept(*this);

    switch (expr.op.type) {
        case SereLexer::TokenType::TOKEN_PLUS:    left_val.perform_add(right_val); break;
        case SereLexer::TokenType::TOKEN_MINUS:   left_val.perform_subtract(right_val); break;
        case SereLexer::TokenType::TOKEN_STAR:    left_val.perform_multiply(right_val); break;
        case SereLexer::TokenType::TOKEN_SLASH:   left_val.perform_divide(right_val); break;
        default: throw std::invalid_argument("BinaryExprAST: Invalid operator.");
    }
    return left_val;
}

template <typename R>
R ExprVisitor<R>::visit_literal(const LiteralExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    return expr.value;
}

template <typename R>
R ExprVisitor<R>::visit_group(const GroupExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    if (!expr.expr) throw std::invalid_argument("GroupExprAST: expr is null.");
    return expr.expr->accept(*this);
}

template <typename R>
R ExprVisitor<R>::visit_unary(const UnaryExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    if (!expr.operand) throw std::invalid_argument("UnaryExprAST: operand is null.");
    R val = expr.operand->accept(*this);
    switch (expr.op.type) {
        case SereLexer::TokenType::TOKEN_MINUS: val.perform_negate(); break;
        case SereLexer::TokenType::TOKEN_BANG:  val.perform_not(); break;
        default: throw std::invalid_argument("UnaryExprAST: Invalid operator.");
    }
    return val;
}

template <typename R>
R ExprVisitor<R>::visit_variable(const VariableExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    auto symbol = RT::global.lookup(expr.name.lexeme);
    if (!symbol) throw std::runtime_error("Variable doesn't exist: " + expr.name.lexeme);
    return symbol->value;
}

// Default implementations for not-yet-implemented visit_* methods
template <typename R>
R ExprVisitor<R>::visit_logical(const LogicalExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    SEREPARSER_UNUSED(expr);
    throw std::runtime_error("visit_logical not implemented.");
}

template <typename R>
R ExprVisitor<R>::visit_call(const CallExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    SEREPARSER_UNUSED(expr);
    throw std::runtime_error("visit_call not implemented.");
}

template <typename R>
R ExprVisitor<R>::visit_super(const SuperExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    SEREPARSER_UNUSED(expr);
    throw std::runtime_error("visit_super not implemented.");
}

template <typename R>
R ExprVisitor<R>::visit_self(const SelfExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    SEREPARSER_UNUSED(expr);
    throw std::runtime_error("visit_self not implemented.");
}

// ================= StatVisitor Implementations =================

template <typename R>
R StatVisitor<R>::visit_assign(const AssignStatAST& stat) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    const std::string& name = stat.name.lexeme;
    SereObject value = stat.initializer ? stat.initializer->accept(*expr_visitor) : SereObject();
    // Insert or update
    auto existing = RT::global.lookup(name);
    if (existing) {
        RT::global.update(name, Runtime::SymbolEntry(value, true));
    } else {
        RT::global.insert(name, Runtime::SymbolEntry(value, true));
    }
    return value;
}

template <typename R>
R StatVisitor<R>::visit_expr(const ExprStatAST& stat) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    if (!stat.expr) throw std::invalid_argument("ExprStatAST: expr is null.");
    return stat.expr->accept(*expr_visitor);
}

template <typename R>
R StatVisitor<R>::visit_block(const BlockStatAST& stat) SEREPARSER_NOEXCEPT {
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_block not implemented.");
}

template <typename R>
R StatVisitor<R>::visit_class(const ClassStatAST& stat) SEREPARSER_NOEXCEPT {
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_class not implemented.");
}

template <typename R>
R StatVisitor<R>::visit_function(const FunctionStatAST& stat) SEREPARSER_NOEXCEPT {
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_function not implemented.");
}
template <typename R>
R StatVisitor<R>::visit_if(const IfStatAST& stat) SEREPARSER_NOEXCEPT {
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_if not implemented.");
}
template <typename R>
R StatVisitor<R>::visit_while(const WhileStatAST& stat) SEREPARSER_NOEXCEPT {
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_while not implemented.");
}
template <typename R>
R StatVisitor<R>::visit_return(const ReturnStatAST& stat) SEREPARSER_NOEXCEPT {
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_return not implemented.");
}

} // namespace SereParser

#endif // PARSER_AST_VISITOR_HPP