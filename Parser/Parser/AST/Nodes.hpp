#ifndef PARSER_AST_NODES_HPP
#define PARSER_AST_NODES_HPP

#include <vector>
#include <AST.hpp>
#include "../Scanner/Token.hpp"

namespace SereParser {

    class BinaryAST;
    class UnaryAST;
    class LiteralAST;
    class LogicalAST;
    class AssignAST;

    class ExprAST : public ASTNode {
        public:
            
            virtual ~ExprAST() = default;
            virtual SereObject accept(ASTVisitor& visitor) = 0;
    };

    class ASTVisitor { 
        public: // TODO
            virtual SereObject visit_binary(BinaryAST& expr) = 0;
            virtual SereObject visit_unary(UnaryAST& expr) = 0;
            virtual SereObject visit_literal(LiteralAST& expr) = 0;
            virtual SereObject visit_logical(LogicalAST& expr) = 0;
            virtual SereObject visit_assign(AssignAST& expr) = 0;
            virtual SereObject visit_call(CallAST& expr) = 0;
            virtual SereObject visit_group(GroupAST& expr) = 0;
            virtual SereObject visit_super(SuperAST& expr) = 0;
            virtual SereObject visit_self(SelfAST& expr) = 0;
            virtual SereObject visit_variable(VariableAST& expr) = 0;
    };

    class BinaryAST : public ExprAST {
        public:
            const SereLexer::TokenBase op;
            const std::unique_ptr<ExprAST> left;
            const std::unique_ptr<ExprAST> right;

            BinaryAST(SereLexer::TokenBase op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right)
                : op(op), left(std::move(left)), right(std::move(right)) {}

            SereObject accept(ASTVisitor& visitor) override {
                return visitor.visit_binary(*this);
            };
    };

    class LiteralAST : public ExprAST {
        public:
            const SereObject value;

            LiteralAST(SereObject value)
                : value(value) {}
            
            SereObject accept(ASTVisitor& visitor) override {
                return visitor.visit_literal(*this);
            }
    };
    
    class LogicalAST : ExprAST {
        public:
            const SereLexer::TokenBase op;
            const std::unique_ptr<ExprAST> left;
            const std::unique_ptr<ExprAST> right;

            LogicalAST(SereLexer::TokenBase op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right)
                : op(op), left(std::move(left)), right(std::move(right)) {}

            SereObject accept(ASTVisitor& visitor) override {
                return visitor.visit_logical(*this);
            }
    };

    class UnaryAST : public ExprAST {
        public:
            const SereLexer::TokenBase op;
            const std::unique_ptr<ExprAST> operand;

            UnaryAST(SereLexer::TokenBase op, std::unique_ptr<ExprAST> operand)
                : op(op), operand(std::move(operand)) {}

            SereObject accept(ASTVisitor& visitor) override {
                return visitor.visit_unary(*this);
            }
    };


    class AssignAST : public ExprAST {
        public:
            const SereLexer::TokenBase name;
            const std::unique_ptr<ExprAST> value; 

            AssignAST(SereLexer::TokenBase name, std::unique_ptr<ExprAST> value)
                : name(name), value(std::move(value)) {}
            
            SereObject accept(ASTVisitor& visitor) override {
                return visitor.visit_assign(*this);
            }

    };

    class CallAST : public ExprAST {
        public:
        const std::unique_ptr<ExprAST> callee; 
        const SereLexer::TokenBase paren;
        const std::vector<std::unique_ptr<ExprAST>> arguments;

        CallAST(std::unique_ptr<ExprAST> callee, SereLexer::TokenBase paren, std::vector<std::unique_ptr<ExprAST>> arguments)
            : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}
        
        SereObject accept(ASTVisitor& visitor) override {
            // Implement the visitor pattern for CallAST
            return visitor.visit_call(*this);
        }
    };

    class GroupAST : public ExprAST {
        public:
            const std::unique_ptr<ExprAST> expr;

            GroupAST(std::unique_ptr<ExprAST> expr)
                : expr(std::move(expr)) {}

            SereObject accept(ASTVisitor& visitor) override {
                // Implement the visitor pattern for GroupAST
                return visitor.visit_group(*this);
            }
    };

    class SuperAST : public ExprAST {
        public:
            const SereLexer::TokenBase keyword;
            const SereLexer::TokenBase method;

            SuperAST(SereLexer::TokenBase keyword, SereLexer::TokenBase method)
                : keyword(keyword), method(method) {}

            SereObject accept(ASTVisitor& visitor) override {
                // Implement the visitor pattern for SuperAST
                return visitor.visit_super(*this);
            }
    };

    class SelfAST : public ExprAST {
        public:
            const SereLexer::TokenBase keyword;

            SelfAST(SereLexer::TokenBase keyword)
                : keyword(keyword) {}

            SereObject accept(ASTVisitor& visitor) override {
                // Implement the visitor pattern for SelfAST
                return visitor.visit_self(*this);
            }
    };

    class VariableAST : public ExprAST {
        public:
            const SereLexer::TokenBase name;

            VariableAST(SereLexer::TokenBase name)
                : name(name) {}

            SereObject accept(ASTVisitor& visitor) override {
                // Implement the visitor pattern for VariableAST
                return visitor.visit_variable(*this);
            }
    };

}
#endif // PARSER_AST_NODES_HPP