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
            : op(op), left(std::move(left)), right(std::move(right)) 
        {
            if (!this->left || !this->right) {
                throw std::invalid_argument("LogicalExprAST: left and right expressions must not be null");
            }
        }

        LogicalExprAST(const SereLexer::TokenBase* op, std::shared_ptr<ExprAST> left, std::shared_ptr<ExprAST> right)
            : op(*op), left(std::move(left)), right(std::move(right))
        {
            if (!op) {
                throw std::invalid_argument("LogicalExprAST: op pointer must not be null");
            }
            if (!this->left || !this->right) {
                throw std::invalid_argument("LogicalExprAST: left and right expressions must not be null");
            }
        }

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            // Defensive: visitor reference should not be null (by contract)
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


    class CallExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase callee;
        const std::vector<std::shared_ptr<ExprAST>> arguments;

        CallExprAST(SereLexer::TokenBase& callee, std::vector<std::shared_ptr<ExprAST>> arguments)
            : callee(std::move(callee)), arguments(std::move(arguments)) {}

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

    class TypeAnnotationExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase name;
        const std::shared_ptr<ExprAST> subtype;

        explicit TypeAnnotationExprAST(const SereLexer::TokenBase& name)
            : name(name), subtype(nullptr) {}
        
        TypeAnnotationExprAST(const SereLexer::TokenBase& name, std::shared_ptr<ExprAST> subtype)
            : name(name), subtype(std::move(subtype)) {}
        
        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return SereObject();
        }
    };

        class VariableExprAST : public ExprAST {
    public:
        const SereLexer::TokenBase name;
        const std::shared_ptr<TypeAnnotationExprAST> type_annotation;

        explicit VariableExprAST(const SereLexer::TokenBase& name)
            : name(name), type_annotation(nullptr) {}

        VariableExprAST(const SereLexer::TokenBase& name, std::shared_ptr<TypeAnnotationExprAST> type)
            : name(name), type_annotation(type) {}

        SereObject accept(ExprVisitor<SereObject>& visitor) const override {
            return visitor.visit_variable(*this);
        }
    };


} // namespace SereParser

#endif // PARSER_AST_EXPR_HPP
