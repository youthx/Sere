#ifndef PARSER_AST_EXPR_HPP
#define PARSER_AST_EXPR_HPP

#include <vector>
#include <AST.hpp>
#include "../Scanner/Token.hpp"

namespace SereParser {
    class ExprAST : public ASTNode {
        public:
            
            virtual ~ExprAST() = default;
            virtual SereObject accept(ExprVisitor& visitor) = 0;
    };

    class ExprVisitor { 
        public: // TODO
            virtual SereObject visit_binary(BinaryExprAST& expr) = 0;
            virtual SereObject visit_unary(UnaryExprAST& expr) = 0;
            virtual SereObject visit_literal(LiteralExprAST& expr) = 0;
            virtual SereObject visit_logical(LogicalExprAST& expr) = 0;
            virtual SereObject visit_assign(AssignExprAST& expr) = 0;
            virtual SereObject visit_call(CallExprAST& expr) = 0;
            virtual SereObject visit_group(GroupExprAST& expr) = 0;
            virtual SereObject visit_super(SuperExprAST& expr) = 0;
            virtual SereObject visit_self(SelfExprAST& expr) = 0;
            virtual SereObject visit_variable(VariableExprAST& expr) = 0;
    };

    class BinaryExprAST : public ExprAST {
        public:
            const SereLexer::TokenBase op;
            const std::unique_ptr<ExprAST> left;
            const std::unique_ptr<ExprAST> right;

            BinaryExprAST(SereLexer::TokenBase op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right)
                : op(op), left(std::move(left)), right(std::move(right)) {}

            SereObject accept(ExprVisitor& visitor) override {
                return visitor.visit_binary(*this);
            };
    };

    class LiteralExprAST : public ExprAST {
        public:
            const SereObject value;

            LiteralExprAST(SereObject value)
                : value(value) {}
            
            SereObject accept(ExprVisitor& visitor) override {
                return visitor.visit_literal(*this);
            }
    };
    
    class LogicalExprAST : ExprAST {
        public:
            const SereLexer::TokenBase op;
            const std::unique_ptr<ExprAST> left;
            const std::unique_ptr<ExprAST> right;

            LogicalExprAST(SereLexer::TokenBase op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right)
                : op(op), left(std::move(left)), right(std::move(right)) {}

            SereObject accept(ExprVisitor& visitor) override {
                return visitor.visit_logical(*this);
            }
    };

    class UnaryExprAST : public ExprAST {
        public:
            const SereLexer::TokenBase op;
            const std::unique_ptr<ExprAST> operand;

            UnaryExprAST(SereLexer::TokenBase op, std::unique_ptr<ExprAST> operand)
                : op(op), operand(std::move(operand)) {}

            SereObject accept(ExprVisitor& visitor) override {
                return visitor.visit_unary(*this);
            }
    };


    class AssignExprAST : public ExprAST {
        public:
            const SereLexer::TokenBase name;
            const std::unique_ptr<ExprAST> value; 

            AssignExprAST(SereLexer::TokenBase name, std::unique_ptr<ExprAST> value)
                : name(name), value(std::move(value)) {}
            
            SereObject accept(ExprVisitor& visitor) override {
                return visitor.visit_assign(*this);
            }

    };

    class CallExprAST : public ExprAST {
        public:
        const std::unique_ptr<ExprAST> callee; 
        const SereLexer::TokenBase paren;
        const std::vector<std::unique_ptr<ExprAST>> arguments;

        CallExprAST(std::unique_ptr<ExprAST> callee, SereLexer::TokenBase paren, std::vector<std::unique_ptr<ExprAST>> arguments)
            : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}
        
        SereObject accept(ExprVisitor& visitor) override {
            // Implement the visitor pattern for CallAST
            return visitor.visit_call(*this);
        }
    };

    class GroupExprAST : public ExprAST {
        public:
            const std::unique_ptr<ExprAST> expr;

            GroupExprAST(std::unique_ptr<ExprAST> expr)
                : expr(std::move(expr)) {}

            SereObject accept(ExprVisitor& visitor) override {
                // Implement the visitor pattern for GroupAST
                return visitor.visit_group(*this);
            }
    };

    class SuperExprAST : public ExprAST {
        public:
            const SereLexer::TokenBase keyword;
            const SereLexer::TokenBase method;

            SuperExprAST(SereLexer::TokenBase keyword, SereLexer::TokenBase method)
                : keyword(keyword), method(method) {}

            SereObject accept(ExprVisitor& visitor) override {
                // Implement the visitor pattern for SuperAST
                return visitor.visit_super(*this);
            }
    };

    class SelfExprAST : public ExprAST {
        public:
            const SereLexer::TokenBase keyword;

            SelfExprAST(SereLexer::TokenBase keyword)
                : keyword(keyword) {}

            SereObject accept(ExprVisitor& visitor) override {
                // Implement the visitor pattern for SelfAST
                return visitor.visit_self(*this);
            }
    };

    class VariableExprAST : public ExprAST {
        public:
            const SereLexer::TokenBase name;

            VariableExprAST(SereLexer::TokenBase name)
                : name(name) {}

            SereObject accept(ExprVisitor& visitor) override {
                // Implement the visitor pattern for VariableAST
                return visitor.visit_variable(*this);
            }
    };

}
#endif // PARSER_AST_EXPR_HPP