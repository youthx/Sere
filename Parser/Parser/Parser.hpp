#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include <vector>
#include <exception>
#include <string>
#include <memory>
#include <stdexcept>

#include "../Scanner/Token.hpp"
#include "../Scanner/TokenType.hpp"
#include "../Scanner/Scanner.hpp"

#include "AST/Expr.hpp"
#include "AST/Stat.hpp"

namespace SereParser {

    class ParserError : public std::runtime_error {
    private:
        std::shared_ptr<SereLexer::TokenBase> token;

    public:
        explicit ParserError(std::shared_ptr<SereLexer::TokenBase> token, const std::string& msg)
            : std::runtime_error(msg), token(std::move(token)) {}

        explicit ParserError(const std::string& msg)
            : std::runtime_error(msg), token(nullptr) {}

        const char* what() const noexcept override {
            static std::string formattedMessage;
            if (token) {
                formattedMessage = "[Error] line " + std::to_string(token->line) + ":\n\t" + std::runtime_error::what();
            } else {
                formattedMessage = "[Error]\n\t" + std::string(std::runtime_error::what());
            }
            return formattedMessage.c_str();
        }
    };

    class Parser {
    public:
        explicit Parser(SereLexer::TokenList token_list)
            : tokens(token_list.getTokens()), token_index_pointer(0) {}

        std::shared_ptr<ExprAST> parse() {
            try {
                return parse_expr();
            } catch (const ParserError&) {
                synchronize();
                return nullptr;
            }
        }

    private:
        std::vector<std::shared_ptr<SereLexer::TokenBase>> tokens;
        size_t token_index_pointer;

        void synchronize() {
            advance();

            while (!is_at_end()) {
                if (get_previous()->type == SereLexer::TOKEN_NEWLINE ||
                    get_previous()->type == SereLexer::TOKEN_SEMICOLON) {
                    return;
                }

                switch (peek()->type) {
                    case SereLexer::TOKEN_CLASS:
                    case SereLexer::TOKEN_DEF:
                    case SereLexer::TOKEN_IF:
                    case SereLexer::TOKEN_ELSE:
                    case SereLexer::TOKEN_WHILE:
                    case SereLexer::TOKEN_RETURN:
                        return;
                    default:
                        break;
                }
                advance();
            }
        }

        template <typename... TokenTypes>
        bool token_isa(TokenTypes... types) {
            for (const auto& type : {types...}) {
                if (expect(type)) {
                    advance();
                    return true;
                }
            }
            return false;
        }

        bool expect(SereLexer::TokenType type) const {
            return !is_at_end() && peek()->type == type;
        }

        bool is_at_end() const {
            return token_index_pointer >= tokens.size();
        }

        ParserError error(std::shared_ptr<SereLexer::TokenBase> token, const std::string& message) const {
            return ParserError(std::move(token), message);
        }

        std::shared_ptr<SereLexer::TokenBase> advance_assert(SereLexer::TokenType type, const std::string& message) {
            if (expect(type)) return advance();
            throw error(peek(), message);
        }

        std::shared_ptr<SereLexer::TokenBase> advance() {
            if (!is_at_end()) {
                token_index_pointer++;
            }

            return get_previous();
        }

        std::shared_ptr<SereLexer::TokenBase> peek() const {
            return tokens.at(token_index_pointer);
        }

        std::shared_ptr<SereLexer::TokenBase> get_previous() const {
            if (token_index_pointer == 0) {
                throw std::out_of_range("Attempted to access token before the start of the token list.");
            }
            return tokens.at(token_index_pointer - 1);
        }

        std::shared_ptr<ExprAST> parse_expr() {
            return parse_equality();
        }

        std::shared_ptr<ExprAST> parse_equality() {
            auto left = parse_comp();

            while (token_isa(SereLexer::TOKEN_BANG_EQUAL, SereLexer::TOKEN_EQUAL_EQUAL)) {
                auto op = get_previous();
                auto right = parse_comp();
                left = std::make_shared<BinaryExprAST>(op.get(), left, right);
            }
            return left;
        }

        std::shared_ptr<ExprAST> parse_comp() {
            auto expr = parse_term();

            while (token_isa(SereLexer::TOKEN_LESS, SereLexer::TOKEN_LESS_EQUAL,
                             SereLexer::TOKEN_GREATER, SereLexer::TOKEN_GREATER_EQUAL)) {
                auto op = get_previous();
                auto right = parse_term();
                expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
            }
            return expr;
        }

        std::shared_ptr<ExprAST> parse_term() {
            auto expr = parse_factor();

            while (token_isa(SereLexer::TOKEN_MINUS, SereLexer::TOKEN_PLUS)) {
                auto op = get_previous();
                auto right = parse_factor();
                expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
            }

            return expr;
        }

        std::shared_ptr<ExprAST> parse_factor() {
            auto expr = parse_unary();

            while (token_isa(SereLexer::TOKEN_SLASH, SereLexer::TOKEN_STAR)) {
                auto op = get_previous();
                auto right = parse_unary();
                expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
            }

            return expr;
        }

        std::shared_ptr<ExprAST> parse_unary() {
            if (token_isa(SereLexer::TOKEN_MINUS, SereLexer::TOKEN_BANG)) {
                auto op = get_previous();
                auto right = parse_unary();
                return std::make_shared<UnaryExprAST>(op.get(), right);
            }

            return parse_primary();
        }

        std::shared_ptr<ExprAST> parse_primary() {
            if (token_isa(SereLexer::TOKEN_TRUE))
                return std::make_shared<LiteralExprAST>(SereObject(true));
            if (token_isa(SereLexer::TOKEN_FALSE))
                return std::make_shared<LiteralExprAST>(SereObject(false));
            if (token_isa(SereLexer::TOKEN_NONE))
                return std::make_shared<LiteralExprAST>(SereObject());

            if (token_isa(SereLexer::TOKEN_INTEGER)) {
                auto value = get_previous()->literal.INTEGER;
                return std::make_shared<LiteralExprAST>(SereObject(value));
            }
            if (token_isa(SereLexer::TOKEN_FLOAT)) {
                auto value = get_previous()->literal.FLOAT;
                return std::make_shared<LiteralExprAST>(SereObject(value));
            }
            if (token_isa(SereLexer::TOKEN_STRING)) {
                auto value = get_previous()->literal.STRING;
                return std::make_shared<LiteralExprAST>(SereObject(value));
            }

            if (token_isa(SereLexer::TOKEN_LEFT_PAREN)) {
                auto expr = parse_expr();
                advance_assert(SereLexer::TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
                return expr;
            }

            throw error(peek(), "Expected expression.");
        }
    };

} // namespace SereParser

#endif // PARSER_PARSER_HPP
