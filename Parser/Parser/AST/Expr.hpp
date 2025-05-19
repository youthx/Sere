#ifndef PARSER_AST_EXPR_HPP
#define PARSER_AST_EXPR_HPP

#include <vector>
#include <memory>
#include <utility>
#include "./AST.hpp"
#include "./Visitor.hpp"
namespace SereParser {

    class ExprAST {
    public:
        virtual ~ExprAST() = default;
        virtual SereObject accept(ExprVisitor<SereObject>& visitor) const = 0;
    };


    class BinaryExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase op;
        const std::shared_ptr<ExprAST> left;
        const std::shared_ptr<ExprAST> right;

        BinaryExprAST(const SereLexer::TokenBase& op, std::shared_ptr<ExprAST> left, std::shared_ptr<ExprAST> right)
            : op(op), left(std::move(left)), right(std::move(right)) {}

        BinaryExprAST(const SereLexer::TokenBase* op, std::shared_ptr<ExprAST> left, std::shared_ptr<ExprAST> right)
            : op(*op), left(std::move(left)), right(std::move(right)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_binary(*this);
        }
    };

    class LiteralExprAST : public ExprAST {
    public:
        const SereObject value;

        explicit LiteralExprAST(SereObject value)
            : value(std::move(value)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_literal(*this);
        }
    };

    class LogicalExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase op;
        const std::shared_ptr<ExprAST> left;
        const std::shared_ptr<ExprAST> right;

        LogicalExprAST(const SereLexer::TokenBase& op, std::shared_ptr<ExprAST> left, std::shared_ptr<ExprAST> right)
            : op(op), left(std::move(left)), right(std::move(right)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_logical(*this);
        }
    };

    class UnaryExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase op;
        const std::shared_ptr<ExprAST> operand;

        UnaryExprAST(const SereLexer::TokenBase& op, std::shared_ptr<ExprAST> operand)
            : op(op), operand(std::move(operand)) {}

        UnaryExprAST(const SereLexer::TokenBase* op, std::shared_ptr<ExprAST> operand)
            : op(*op), operand(std::move(operand)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_unary(*this);
        }
    };

    class AssignExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase name;
        const std::shared_ptr<ExprAST> value;

        AssignExprAST(const SereLexer::TokenBase& name, std::shared_ptr<ExprAST> value)
            : name(name), value(std::move(value)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_assign(*this);
        }
    };

    class CallExprAST : public ExprAST {
    public:
        const std::shared_ptr<ExprAST> callee;
        const SereLexer::TokenBase paren;
        const std::vector<std::shared_ptr<ExprAST>> arguments;

        CallExprAST(std::shared_ptr<ExprAST> callee, const SereLexer::TokenBase& paren, std::vector<std::shared_ptr<ExprAST>> arguments)
            : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_call(*this);
        }
    };

    class GroupExprAST : public ExprAST {
    public:
        const std::shared_ptr<ExprAST> expr;

        explicit GroupExprAST(std::shared_ptr<ExprAST> expr)
            : expr(std::move(expr)) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_group(*this);
        }
    };

    class SuperExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase keyword;
        const SereLexer::TokenBase method;

        SuperExprAST(const SereLexer::TokenBase& keyword, const SereLexer::TokenBase& method)
            : keyword(keyword), method(method) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_super(*this);
        }
    };

    class SelfExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase keyword;

        explicit SelfExprAST(const SereLexer::TokenBase& keyword)
            : keyword(keyword) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_self(*this);
        }
    };

    class VariableExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase name;

        explicit VariableExprAST(const SereLexer::TokenBase& name)
            : name(name) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_variable(*this);
        }
    };

} // namespace SereParser

#endif // PARSER_AST_EXPR_HPP
