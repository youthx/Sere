#pragma once

#include <vector>
#include <memory>
#include "./Expr.hpp"
#include "./AST.hpp"

namespace SereParser {

    // Forward declarations
    class BlockStatAST;
    class ClassStatAST;
    class ExprStatAST;
    class FunctionStatAST;
    class IfStatAST;
    class WhileStatAST;
    class VariableStatAST;
    class ReturnStatAST;

    // Visitor interface
    template <typename R>
    class StatVisitor {
    public:
        virtual ~StatVisitor() = default;

        virtual R visit_block(const BlockStatAST& stat) = 0;
        virtual R visit_class(const ClassStatAST& stat) = 0;
        virtual R visit_expr(const ExprStatAST& stat) = 0;
        virtual R visit_function(const FunctionStatAST& stat) = 0;
        virtual R visit_if(const IfStatAST& stat) = 0;
        virtual R visit_while(const WhileStatAST& stat) = 0;
        virtual R visit_variable(const VariableStatAST& stat) = 0;
        virtual R visit_return(const ReturnStatAST& stat) = 0;
    };

    // Base class for all statements
    class StatAST {
    public:
        virtual ~StatAST() = default;
        [[nodiscard]] virtual SereObject accept(StatVisitor<SereObject>& visitor) const = 0;
    };

    // Block statement
    class BlockStatAST final : public StatAST {
    public:
        const std::vector<std::shared_ptr<StatAST>> statements;

        explicit BlockStatAST(std::vector<std::shared_ptr<StatAST>> statements) noexcept
            : statements(std::move(statements)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_block(*this);
        }
    };

    // Class statement
    class ClassStatAST final : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const std::shared_ptr<VariableExprAST> superclass;
        const std::vector<std::shared_ptr<FunctionStatAST>> methods;

        ClassStatAST(const SereLexer::TokenBase& name,
                     std::shared_ptr<VariableExprAST> superclass,
                     std::vector<std::shared_ptr<FunctionStatAST>> methods) noexcept
            : name(name), superclass(std::move(superclass)), methods(std::move(methods)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_class(*this);
        }
    };

    // Expression statement
    class ExprStatAST final : public StatAST {
    public:
        const std::shared_ptr<ExprAST> expr;

        explicit ExprStatAST(std::shared_ptr<ExprAST> expr) noexcept
            : expr(std::move(expr)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_expr(*this);
        }
    };

    // Function statement
    class FunctionStatAST final : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const std::vector<SereLexer::TokenBase> params;
        const std::shared_ptr<StatAST> body;

        FunctionStatAST(const SereLexer::TokenBase& name,
                        std::vector<SereLexer::TokenBase> params,
                        std::shared_ptr<StatAST> body) noexcept
            : name(name), params(std::move(params)), body(std::move(body)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_function(*this);
        }
    };

    // If statement
    class IfStatAST final : public StatAST {
    public:
        const std::shared_ptr<ExprAST> condition;
        const std::shared_ptr<StatAST> then_branch;
        const std::shared_ptr<StatAST> else_branch;

        IfStatAST(std::shared_ptr<ExprAST> condition,
                  std::shared_ptr<StatAST> then_branch,
                  std::shared_ptr<StatAST> else_branch) noexcept
            : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_if(*this);
        }
    };

    // While statement
    class WhileStatAST final : public StatAST {
    public:
        const std::shared_ptr<ExprAST> condition;
        const std::shared_ptr<StatAST> body;

        WhileStatAST(std::shared_ptr<ExprAST> condition,
                     std::shared_ptr<StatAST> body) noexcept
            : condition(std::move(condition)), body(std::move(body)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_while(*this);
        }
    };

    // Variable statement
    class VariableStatAST final : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const SereLexer::TokenBase type;
        const std::shared_ptr<ExprAST> initializer;

        VariableStatAST(const SereLexer::TokenBase& name,
                        const SereLexer::TokenBase& type,
                        std::shared_ptr<ExprAST> initializer) noexcept
            : name(name), type(type), initializer(std::move(initializer)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_variable(*this);
        }
    };

    // Return statement
    class ReturnStatAST final : public StatAST {
    public:
        const SereLexer::TokenBase keyword;
        const std::shared_ptr<ExprAST> value;

        ReturnStatAST(const SereLexer::TokenBase& keyword,
                      std::shared_ptr<ExprAST> value) noexcept
            : keyword(keyword), value(std::move(value)) {}

        [[nodiscard]] SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_return(*this);
        }
    };

} // namespace SereParser
#