#pragma once

#include <vector>
#include <memory>

#include "./AST.hpp"
#include "./Visitor.hpp"

namespace SereParser {

    // Base class for all statements
    class StatAST {
    public:
        virtual ~StatAST() = default;
        virtual SereObject accept(StatVisitor<SereObject>& visitor) const = 0;
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

        ClassStatAST(const SereLexer::TokenBase& name,
                     std::shared_ptr<VariableExprAST> superclass,
                     std::vector<std::shared_ptr<FunctionStatAST>> methods)
            : name(name), superclass(std::move(superclass)), methods(std::move(methods)) {}

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
            return expr.get()->accept(*visitor.expr_visitor);
        }
    };

    
    // Function statement
    class FunctionStatAST : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const std::vector<SereLexer::TokenBase> params;
        const std::shared_ptr<StatAST> body;

        FunctionStatAST(const SereLexer::TokenBase& name,
                        std::vector<SereLexer::TokenBase> params,
                        std::shared_ptr<StatAST> body) 
            : name(name), params(std::move(params)), body(std::move(body)) {}

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

        IfStatAST(std::shared_ptr<ExprAST> condition,
                  std::shared_ptr<StatAST> then_branch,
                  std::shared_ptr<StatAST> else_branch)
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

        WhileStatAST(std::shared_ptr<ExprAST> condition,
                     std::shared_ptr<StatAST> body)
            : condition(std::move(condition)), body(std::move(body)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_while(*this);
        }
    };

    // Variable statement
    class AssignStatAST : public StatAST {
    public:
        const SereLexer::TokenBase name;
        const SereLexer::TokenBase type;
        const std::shared_ptr<ExprAST> initializer;

        AssignStatAST(const SereLexer::TokenBase& name,
                        const SereLexer::TokenBase& type,
                        std::shared_ptr<ExprAST> initializer)
            : name(name), type(type), initializer(std::move(initializer)) {}

        SereObject accept(StatVisitor<SereObject>& visitor) const override {
            return visitor.visit_assign(*this);
        }
    };

    // Return statement
    class ReturnStatAST : public StatAST {
    public:
        const SereLexer::TokenBase keyword;
        const std::shared_ptr<ExprAST> value;

        ReturnStatAST(const SereLexer::TokenBase& keyword,
                      std::shared_ptr<ExprAST> value)
            : keyword(keyword), value(std::move(value)) {}  

            SereObject accept(StatVisitor<SereObject>& visitor) const override {
                return visitor.visit_return(*this);
            }
    };

} // namespace SereParser
#