#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include <vector>
#include <exception>
#include <string>
#include <memory>
#include <stdexcept>
#include <utility>
#include <type_traits>

#include "../Scanner/Token.hpp"
#include "../Scanner/TokenType.hpp"
#include "../Scanner/Scanner.hpp"

#include "AST/Expr.hpp"
#include "AST/Stat.hpp"

// Forward declarations for AST classes to avoid circular dependencies and speed up compilation
class ExprAST;
class BinaryExprAST;
class UnaryExprAST;
class LiteralExprAST;
class SereObject;

namespace SereParser {

    // Exception for parser errors, includes token context
    class ParserError : public std::runtime_error {
    public:
        ParserError(const std::shared_ptr<SereLexer::TokenBase>& token, const std::string& msg)
            : std::runtime_error(msg), token_(token) {}

        explicit ParserError(const std::string& msg)
            : std::runtime_error(msg), token_(nullptr) {}

        const char* what() const noexcept override {
            try {
                if (token_) {
                    formatted_message_ = "[Error] line " + std::to_string(token_->line) + ":\n\t" + std::runtime_error::what();
                } else {
                    formatted_message_ = "[Error]\n\t" + std::string(std::runtime_error::what());
                }
                return formatted_message_.c_str();
            } catch (...) {
                return "ParserError: unknown error formatting message";
            }
        }

        std::shared_ptr<SereLexer::TokenBase> token() const noexcept { return token_; }

    private:
        std::shared_ptr<SereLexer::TokenBase> token_;
        mutable std::string formatted_message_;
    };

    class Parser {
    public:
        explicit Parser(const SereLexer::TokenList& token_list)
            : tokens_(token_list.getTokens()), token_index_(0) {}

        // Parse entry point
        std::shared_ptr<ExprAST> parse() {
            try {
                return parse_expr();
            } catch (const ParserError&) {
                synchronize();
                return nullptr;
            }
        }

        // For scalability: add parse_statements(), parse_declarations(), etc.

    private:
        std::vector<std::shared_ptr<SereLexer::TokenBase>> tokens_;
        size_t token_index_;

        // --- Token Navigation ---

        bool is_at_end() const noexcept {
            return token_index_ >= tokens_.size();
        }

        std::shared_ptr<SereLexer::TokenBase> peek() const {
            if (is_at_end()) throw std::out_of_range("Peek past end of tokens.");
            return tokens_[token_index_];
        }

        std::shared_ptr<SereLexer::TokenBase> get_previous() const {
            if (token_index_ == 0) throw std::out_of_range("No previous token.");
            return tokens_[token_index_ - 1];
        }

        std::shared_ptr<SereLexer::TokenBase> advance() {
            if (!is_at_end()) ++token_index_;
            return get_previous();
        }

        bool expect(SereLexer::TokenType type) const noexcept {
            return !is_at_end() && peek()->type == type;
        }

        template <typename... TokenTypes>
        bool token_isa(TokenTypes... types) {
            static_assert((std::is_same<TokenTypes, SereLexer::TokenType>() && ...), "token_isa only accepts TokenType arguments");
            for (const auto type : {types...}) {
                if (expect(type)) {
                    advance();
                    return true;
                }
            }
            return false;
        }

        std::shared_ptr<SereLexer::TokenBase> advance_assert(SereLexer::TokenType type, const std::string& message) {
            if (expect(type)) return advance();
            throw error(peek(), message);
        }

        // --- Error Handling ---

        [[nodiscard]] ParserError error(const std::shared_ptr<SereLexer::TokenBase>& token, const std::string& message) const {
            return ParserError(token, message);
        }

        void synchronize() noexcept {
            try {
                advance();
                while (!is_at_end()) {
                    auto prev_type = get_previous()->type;
                    if (prev_type == SereLexer::TOKEN_NEWLINE || prev_type == SereLexer::TOKEN_SEMICOLON)
                        return;

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
            } catch (...) {
                // Swallow exceptions during synchronization to avoid cascading errors
            }
        }

        // --- Parsing Methods ---

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
