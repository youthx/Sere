#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include <vector>
#include "../Scanner/Token.hpp"
#include "../Scanner/TokenType.hpp"
#include "../Scanner/Scanner.hpp"

#include "AST/Expr.hpp"
#include "AST/Stat.hpp"

namespace SereParser {

    class ParserError : public std::runtime_error {
    public:

        std::shared_ptr<SereLexer::TokenBase> token;
        ParserError(const std::string& message)
            : std::runtime_error(message), token(nullptr) {}
        
        ParserError(std::shared_ptr<SereLexer::TokenBase> token, const std::string& message)
            : std::runtime_error(message), token(token) {}

        std::shared_ptr<SereLexer::TokenBase> get_token() const {
            return token;
            
        }

        
    };

    class Parser {
    public:
        Parser() = delete; // Disable default constructor to enforce proper initialization

        explicit Parser(SereLexer::TokenList token_list) {
            tokens = token_list.getTokens();      
        }

    private:
        std::vector<std::shared_ptr<SereLexer::TokenBase>> tokens;
        int token_index_pointer = 0;

        template <typename... TokenTypes>
        bool token_isa(TokenTypes... types) {
            for (auto type : {types...}) {
                if (expect(type)) {
                    return true;
                }
            }
            return false;
        }

        bool expect(SereLexer::TokenType type) {
            if (is_at_end()) {
                return false;
            }
            return peek()->type == type;
        }

        bool is_at_end() {
            return token_index_pointer >= tokens.size();
        }


        ParserError error(std::shared_ptr<SereLexer::TokenBase> token, const std::string& message) {
            if (token->type == SereLexer::TOKEN_EOF) {
                //report(token->line, " at end", message);
            } else {
                //report(token->line, " at '" + token->lexeme + "'", message);
            }

            return ParserError(token, message);
        }

        std::shared_ptr<SereLexer::TokenBase> advance_assert(SereLexer::TokenType type, const std::string& message) {
            if (expect(type)) return advance();
            throw error(peek(), message);
        }

        std::shared_ptr<SereLexer::TokenBase>  advance() {
            if (!is_at_end()) token_index_pointer++;
            return get_previous();
        }


        std::shared_ptr<SereLexer::TokenBase>  peek() {
            return tokens.at(token_index_pointer);
        }

        std::shared_ptr<SereLexer::TokenBase> get_previous() {
            return tokens.at(token_index_pointer - 1);
        }

        std::shared_ptr<ExprAST> parse_expr() {
            return parse_equality();
        }

        std::shared_ptr<ExprAST> parse_comp() {
            auto expr = parse_term();

            while (token_isa(SereLexer::TOKEN_LESS, SereLexer::TOKEN_LESS_EQUAL,
                             SereLexer::TOKEN_GREATER, SereLexer::TOKEN_GREATER_EQUAL)) {
                std::shared_ptr<SereLexer::TokenBase> op = get_previous();
                auto right = parse_term();
                expr = std::make_unique<BinaryExprAST>(expr, right, op);
            }
            return expr;
        }

        std::shared_ptr<ExprAST> parse_term() {
            auto expr = parse_factor();

            while (token_isa(SereLexer::TOKEN_MINUS, SereLexer::TOKEN_PLUS)) {
                std::shared_ptr<SereLexer::TokenBase> op = get_previous();
                auto right = parse_factor();
                expr = std::make_unique<BinaryExprAST>(expr, right, op);
            }

            return expr;
        }

        std::shared_ptr<ExprAST> parse_factor() {
            auto expr = parse_unary();

            while (token_isa(SereLexer::TOKEN_SLASH, SereLexer::TOKEN_STAR)) {
                std::shared_ptr<SereLexer::TokenBase> op = get_previous();
                auto right = parse_unary();
                expr = std::make_unique<BinaryExprAST>(expr, right, op);
            }

            return expr;
        }

        std::shared_ptr<ExprAST> parse_unary() {
            if (token_isa(SereLexer::TOKEN_MINUS, SereLexer::TOKEN_BANG)) {
                std::shared_ptr<SereLexer::TokenBase> op = get_previous();
                auto right = parse_unary();
                return std::make_unique<UnaryExprAST>(op, right);
            }

            return parse_primary();
        }

        std::shared_ptr<ExprAST> parse_primary() {
            if (token_isa(SereLexer::TOKEN_TRUE))
                return std::make_unique<LiteralExprAST>(SereObject(true));
            if (token_isa(SereLexer::TOKEN_FALSE))
                return std::make_unique<LiteralExprAST>(SereObject(false));
            if (token_isa(SereLexer::TOKEN_NONE)) 
                return std::make_unique<LiteralExprAST>(SereObject());
            
            if (token_isa(SereLexer::TOKEN_INTEGER)) {
                auto value = get_previous().get()->literal.INTEGER;
                return std::make_unique<LiteralExprAST>(SereObject(value));
            }
            if (token_isa(SereLexer::TOKEN_FLOAT)) {
                auto value = get_previous().get()->literal.FLOAT;
                return std::make_unique<LiteralExprAST>(SereObject(value));
            }

            if (token_isa(SereLexer::TOKEN_STRING)) {
                auto value = get_previous().get()->literal.STRING;
                return std::make_unique<LiteralExprAST>(SereObject(value));
            }

            if (token_isa(SereLexer::TOKEN_LEFT_PAREN)) {
                auto expr = parse_expr();
                advance_assert(SereLexer::TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
                return std::make_unique<GroupExprAST>(expr);
            }
        }

        std::shared_ptr<ExprAST> parse_equality() {
            auto left = parse_comp();

            while (token_isa(SereLexer::TOKEN_BANG_EQUAL, SereLexer::TOKEN_EQUAL_EQUAL)) {
                std::shared_ptr<SereLexer::TokenBase>  op = get_previous(); 
                auto right = parse_comp();
                left = std::make_unique<BinaryExprAST>(left, right, op);
            }
            return left;
     }

        
    };

};

#endif // PARSER_PARSER_HPP