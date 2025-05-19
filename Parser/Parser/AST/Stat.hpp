#ifndef PARSER_AST_STAT_HPP
#define PARSER_AST_STAT_HPP

#include <vector>
#include <memory>
#include "./Expr.hpp"
#include "./AST.hpp"

namespace SereParser {

    // Forward declaration of StatVisitor
    template <typename R>
    class StatVisitor;

    // Base class for all statements
    class StatAST {
    public:
        virtual ~StatAST() = default;

        // Accept method for the visitor
        virtual SereObject accept(StatVisitor<SereObject>& visitor) const = 0;
    };

    // Visitor interface
    template <typename R>
    class StatVisitor {
    public:
        virtual ~StatVisitor() = default;

        virtual R visit_block(const class BlockStatAST& stat) = 0;
        virtual R visit_class(const class ClassStatAST& stat) = 0;
        virtual R visit_expr(const class ExprStatAST& stat) = 0;
        virtual R visit_function(const class FunctionStatAST& stat) = 0;
        virtual R visit_if(const class IfStatAST& stat) = 0;
        virtual R visit_while(const class WhileStatAST& stat) = 0;
        virtual R visit_variable(const class VariableStatAST& stat) = 0;
        virtual R visit_return(const class ReturnStatAST& stat) = 0;
    };

    // Block statement
    class BlockStatAST : public StatAST {
    public:
        const std::vector<std::shared_ptr<StatAST>> statements;

        BlockStatAST(std::vector<std::shared_ptr<StatAST>> statements)
            : statements(std::move(statements)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_block(*this);
        }
    };

    // Class statement
    class ClassStatAST : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const std::shared_ptr<VariableExprAST> superclass;
        const std::vector<std::shared_ptr<FunctionStatAST>> methods;

        ClassStatAST(SereLexer::TokenBase* name, std::shared_ptr<VariableExprAST> superclass, std::vector<std::shared_ptr<FunctionStatAST>> methods)
            : name(*name), superclass(std::move(superclass)), methods(std::move(methods)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_class(*this);
        }
    };

    // Expression statement
    class ExprStatAST : public StatAST {
    public:
        const std::shared_ptr<ExprAST> expr;

        ExprStatAST(std::shared_ptr<ExprAST> expr)
            : expr(std::move(expr)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_expr(*this);
        }
    };

    // Function statement
    class FunctionStatAST : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const std::vector<SereLexer::TokenBase*> params;
        const std::shared_ptr<StatAST> body;

        FunctionStatAST(SereLexer::TokenBase* name, std::vector<SereLexer::TokenBase*> params, std::shared_ptr<StatAST> body)
            : name(*name), params(std::move(params)), body(std::move(body)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_function(*this);
        }
    };

    // If statement
    class IfStatAST : public StatAST {
    public:
        const std::shared_ptr<ExprAST> condition;
        const std::shared_ptr<StatAST> then_branch;
        const std::shared_ptr<StatAST> else_branch;

        IfStatAST(std::shared_ptr<ExprAST> condition, std::shared_ptr<StatAST> then_branch, std::shared_ptr<StatAST> else_branch)
            : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_if(*this);
        }
    };

    // While statement
    class WhileStatAST : public StatAST {
    public:
        const std::shared_ptr<ExprAST> condition;
        const std::shared_ptr<StatAST> body;

        WhileStatAST(std::shared_ptr<ExprAST> condition, std::shared_ptr<StatAST> body)
            : condition(std::move(condition)), body(std::move(body)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_while(*this);
        }
    };

    // Variable statement
    class VariableStatAST : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const SereLexer::TokenBase type;
        const std::shared_ptr<ExprAST> initializer;

        VariableStatAST(SereLexer::TokenBase* name, SereLexer::TokenBase* type, std::shared_ptr<ExprAST> initializer)
            : name(*name), type(*type), initializer(std::move(initializer)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_variable(*this);
        }
    };

    // Return statement
    class ReturnStatAST : public StatAST {
    public:
        const SereLexer::TokenBase keyword;
        const std::shared_ptr<ExprAST> value;

        ReturnStatAST(SereLexer::TokenBase* keyword, std::shared_ptr<ExprAST> value)
            : keyword(*keyword), value(std::move(value)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_return(*this);
        }
    };

} // namespace SereParser

#endif // PARSER_AST_STAT_HPP
