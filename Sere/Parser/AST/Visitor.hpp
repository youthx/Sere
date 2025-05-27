#pragma once
#ifndef PARSER_AST_VISITOR_HPP
#define PARSER_AST_VISITOR_HPP

#include "./Expr.hpp"
#include <typeinfo>
#include <stdexcept>
#include "./Midlevel/SymbolTable.hpp"
#include "./Midlevel/Environments.hpp"
#include "../Builtins.hpp"
#include <memory>
#include <iostream>

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
    static inline std::shared_ptr<Runtime::TypeEnvironment> global_type_env = std::make_shared<Runtime::TypeEnvironment>();
};

// =====================================
// Semantic Analysis: TypeChecker
// =====================================
class TypeChecker : public std::enable_shared_from_this<TypeChecker> {
public:
    using Ptr = std::shared_ptr<TypeChecker>;

    explicit TypeChecker()
        : env(RT::global_type_env) {} // <-- Always start with global env

    void set_env(const std::shared_ptr<Runtime::TypeEnvironment>& new_env) {
        env = new_env;
    }

    void push_scope() {
        env = env->create_child_scope();
    }
    void pop_scope() {
        if (!env->parent_env)
            throw std::runtime_error("TypeChecker: No parent environment to pop to.");
        env = env->parent_env;
    }

    // ===== Type Checking Entrypoints for Visitor =====
    Runtime::SereTypeKind check_literal(const SereObject& obj) {
        switch (obj.getType()) {
            case SereObjectType::INTEGER: return Runtime::SereTypeKind::INT;
            case SereObjectType::FLOAT:   return Runtime::SereTypeKind::FLOAT;
            case SereObjectType::STRING:  return Runtime::SereTypeKind::STRING;
            case SereObjectType::BOOLEAN: return Runtime::SereTypeKind::BOOL;
            case SereObjectType::NONE:    return Runtime::SereTypeKind::NONE;
            default:                      return Runtime::SereTypeKind::UNKNOWN;
        }
    }

    Runtime::SereTypeKind check_variable(const std::string& name) {
        return env->get_(name);
    }

    Runtime::SereTypeKind check_binary(Runtime::SereTypeKind left, Runtime::SereTypeKind right, const std::string& op) {
        if (left == right) return left;
        if ((left == Runtime::SereTypeKind::INT && right == Runtime::SereTypeKind::FLOAT) ||
            (left == Runtime::SereTypeKind::FLOAT && right == Runtime::SereTypeKind::INT))
            return Runtime::SereTypeKind::FLOAT;
        throw std::runtime_error("Type error: invalid operands for " + op + ": " +
                                 Runtime::to_string(left) + " and " + Runtime::to_string(right));
    }

    Runtime::SereTypeKind check_unary(Runtime::SereTypeKind operand, const std::string& op) {
        if (op == "-" && (operand == Runtime::SereTypeKind::INT || operand == Runtime::SereTypeKind::FLOAT))
            return operand;
        if (op == "!" && operand == Runtime::SereTypeKind::BOOL)
            return Runtime::SereTypeKind::BOOL;
        throw std::runtime_error("Type error: invalid operand for " + op + ": " + Runtime::to_string(operand));
    }

    void check_assign(const std::string& name, Runtime::SereTypeKind rhs_type) {
        env->set(name, rhs_type);
    }

    void debug_dump_env() const {
        std::cout << "[TypeChecker] Current Environment:\n";
        for (const auto& pair : env->table) {
            std::cout << "  " << pair.first << " : " << Runtime::to_string(pair.second) << "\n";
        }
    }

    std::shared_ptr<Runtime::TypeEnvironment> env;
};

// =====================================
// ExprVisitor Template
// =====================================
template <typename R>
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    std::shared_ptr<TypeChecker> type_checker;

    ExprVisitor(std::shared_ptr<TypeChecker> checker)
        : type_checker(std::move(checker)) {}

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

// =====================================
// StatVisitor Template
// =====================================
template <typename R>
class StatVisitor {
public:
    virtual ~StatVisitor() = default;
    std::shared_ptr<ExprVisitor<R>> expr_visitor;
    std::shared_ptr<TypeChecker> type_checker;

    StatVisitor(std::shared_ptr<ExprVisitor<R>> expr_visitor_)
        : expr_visitor(std::move(expr_visitor_)),
          type_checker(std::make_shared<TypeChecker>())
    {
        // Always use the real global env!
        type_checker->set_env(RT::global_type_env);
    }

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

// =====================================
// ExprVisitor Implementations
// =====================================
template <typename R>
R ExprVisitor<R>::visit_binary(const BinaryExprAST& expr) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;

    if (!expr.left || !expr.right)
        throw std::invalid_argument("BinaryExprAST: left or right is null.");

    R left_val = expr.left->accept(*this);
    R right_val = expr.right->accept(*this);

    // Semantic Type Check
    auto left_type = type_checker->check_literal(left_val);
    auto right_type = type_checker->check_literal(right_val);
    std::string op_str = expr.op.lexeme;
    auto result_type = type_checker->check_binary(left_type, right_type, op_str);

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
    type_checker->check_literal(expr.value);
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
    auto operand_type = type_checker->check_literal(val);
    std::string op_str = expr.op.lexeme;
    type_checker->check_unary(operand_type, op_str);

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
    auto type = type_checker->check_variable(expr.name.lexeme);
    if (type == Runtime::SereTypeKind::UNKNOWN) {
        throw std::runtime_error("Variable '" + expr.name.lexeme + "' is not defined in the current scope.");
    }
    auto symbol = RT::global.lookup(expr.name.lexeme);
    if (!symbol) throw std::runtime_error("Variable doesn't exist: " + expr.name.lexeme);
    return symbol->value;
}

// --- Not-yet-implemented visit_* methods ---
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

// =====================================
// StatVisitor Implementations
// =====================================
template <typename R>
R StatVisitor<R>::visit_assign(const AssignStatAST& stat) SEREPARSER_NOEXCEPT {
    ASSERT_MUST_RETURN_SERE_OBJECT;
    const std::string& name = stat.name.lexeme;
    SereObject value = stat.initializer ? stat.initializer->accept(*expr_visitor) : SereObject();

    Runtime::SereTypeKind declared_type = Runtime::SereTypeKind::UNKNOWN;
    if (!stat.type.lexeme.empty() && stat.type.lexeme != name) {
        auto builtin_type = BuiltIn::get_builtin_type(stat.type.lexeme);
        if (!builtin_type) {
            throw std::runtime_error("Unknown type: " + stat.type.lexeme);
        }
        declared_type = builtin_type->parent;
    }
    auto value_type = type_checker->check_literal(value);
    if (declared_type == Runtime::SereTypeKind::UNKNOWN)
        declared_type = value_type;

    if (value_type != declared_type) {
        throw std::runtime_error("TypeError: type mismatch.");
    }

    // Register the variable and its type in the environment
    type_checker->check_assign(name, value_type);

    // Insert or update runtime value
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
    // FIX: Never push/pop scope for the outermost block!
    static bool seen_first_block = false;
    bool should_push = seen_first_block; // Only push if NOT first block
    if (!seen_first_block) seen_first_block = true;

    if (should_push) type_checker->push_scope();
    SereObject last_value;
    for (auto& statement : stat.statements) {
        last_value = statement->accept(*this);
    }
    if (should_push) type_checker->pop_scope();
    return last_value;
}

template <typename R>
R StatVisitor<R>::visit_class(const ClassStatAST& stat) SEREPARSER_NOEXCEPT {
    type_checker->push_scope();
    // for (auto& field : stat.fields) ...
    // for (auto& method : stat.methods) ...
    type_checker->pop_scope();
    SEREPARSER_UNUSED(stat);
    throw std::runtime_error("visit_class not implemented.");
}

template <typename R>
R StatVisitor<R>::visit_function(const FunctionStatAST& stat) SEREPARSER_NOEXCEPT {
    type_checker->push_scope();
    // ...handle params, body, return type...
    type_checker->pop_scope();
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