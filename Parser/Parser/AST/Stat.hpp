#ifndef PARSER_AST_STAT_HPP
#define PARSER_AST_STAT_HPP

#include <vector>
#include <AST.hpp>
#include "../Scanner/Token.hpp"
#include "./Expr.hpp"

namespace SereParser {
    class StatAST {
        public:
            virtual ~StatAST() = default;
            virtual SereObject accept(StatVisitor& visitor) = 0;
    };

    class StatVisitor {
        public:
            virtual SereObject visit_block(BlockStatAST& stat) = 0;
            virtual SereObject visit_class(ClassStatAST& stat) = 0;
            virtual SereObject visit_expr(ExprStatAST& stat) = 0;
            virtual SereObject visit_function(FunctionStatAST& stat) = 0;
            virtual SereObject visit_if(IfStatAST& stat) = 0;
            virtual SereObject visit_while(WhileStatAST& stat) = 0;
            virtual SereObject visit_variable(VariableStatAST& stat) = 0;
            virtual SereObject visit_return(ReturnStatAST& stat) = 0;
    };

    class BlockStatAST : public StatAST {
        public:
            const std::vector<std::unique_ptr<StatAST>> statements;

            BlockStatAST(std::vector<std::unique_ptr<StatAST>> statements)
                : statements(std::move(statements)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_block(*this);
            }
    };

    class ClassStatAST : public StatAST {
        public:
            const SereLexer::TokenBase name;
            const VariableExprAST superclass;
            const std::vector<std::unique_ptr<FunctionStatAST>> methods;

            ClassStatAST(SereLexer::TokenBase name, VariableExprAST superclass, std::vector<std::unique_ptr<FunctionStatAST>> methods)
                : name(name), superclass(superclass), methods(std::move(methods)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_class(*this);
            }
    };

    class ExprStatAST : public StatAST {
        public:
            const std::unique_ptr<ExprAST> expr;

            ExprStatAST(std::unique_ptr<ExprAST> expr)
                : expr(std::move(expr)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_expr(*this);
            }
    };

    class FunctionStatAST : public StatAST {
        public:
            const SereLexer::TokenBase name;
            const std::vector<SereLexer::TokenBase> params;
            const std::unique_ptr<StatAST> body;

            FunctionStatAST(SereLexer::TokenBase name, std::vector<SereLexer::TokenBase> params, std::unique_ptr<StatAST> body)
                : name(name), params(std::move(params)), body(std::move(body)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_function(*this);
            }

    };

    class IfStatAST : public StatAST {
        public:
            const std::unique_ptr<ExprAST> condition;
            const std::unique_ptr<StatAST> then_branch;
            const std::unique_ptr<StatAST> else_branch;
            // const std::unique_ptr<StatAST> elif_branch;
        
            IfStatAST(std::unique_ptr<ExprAST> condition, std::unique_ptr<StatAST> then_branch, std::unique_ptr<StatAST> else_branch)
                : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_if(*this);
            }
    };

    class WhileStatAST : public StatAST {
        public:
            const std::unique_ptr<ExprAST> condition;
            const std::unique_ptr<StatAST> body;

            WhileStatAST(std::unique_ptr<ExprAST> condition, std::unique_ptr<StatAST> body)
                : condition(std::move(condition)), body(std::move(body)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_while(*this);
            }
    };

    class VariableStatAST : public StatAST {
        public:
            const SereLexer::TokenBase name;
            const SereLexer::TokenBase type;
            const std::unique_ptr<ExprAST> initializer;

            VariableStatAST(SereLexer::TokenBase name, SereLexer::TokenBase type, std::unique_ptr<ExprAST> initializer)
                : name(name), type(type), initializer(std::move(initializer)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_variable(*this);
            }
    };

    class ReturnStatAST : public StatAST {
        public:
            const SereLexer::TokenBase keyword;
            const std::unique_ptr<ExprAST> value;

            ReturnStatAST(SereLexer::TokenBase keyword, std::unique_ptr<ExprAST> value)
                : keyword(keyword), value(std::move(value)) {}

            SereObject accept(StatVisitor& visitor) override {
                return visitor.visit_return(*this);
            }
    };

}
#endif // PARSER_AST_STAT_HPP