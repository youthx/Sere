#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>
#include <type_traits>
#include "../Scanner/Token.hpp"
#include "../Scanner/Scanner.hpp"
#include "AST/Expr.hpp"
#include "AST/Stat.hpp"

namespace SereParser {

// ========== Exception Class ==========

class ParserError : public std::runtime_error {
public:
    ParserError(const std::shared_ptr<SereLexer::TokenBase>& token, std::string msg)
        : std::runtime_error(std::move(msg)), token_(token) {}
    const char* what() const noexcept override {
        std::ostringstream oss;
        oss << "[ParserError] Line " << (token_ ? token_->line : 0) << ": " << std::runtime_error::what();
        if (token_) oss << " (got '" << token_->lexeme << "')";
        formatted_ = oss.str();
        return formatted_.c_str();
    }
private:
    std::shared_ptr<SereLexer::TokenBase> token_;
    mutable std::string formatted_;
};

// ========== Utility Macros ==========

#define SERE_ASSERT(cond, token, msg) \
    do { if (!(cond)) throw SereParser::ParserError(token, msg); } while(0)

#define SERE_FATAL(msg) \
    throw std::logic_error(std::string("[Fatal parser error] ") + msg)

// ========== Parser Class ==========

class Parser {
public:
    explicit Parser(const SereLexer::TokenList& tokens)
        : tokens_(tokens.getTokens()), current_(0)
    {
        SERE_ASSERT(!tokens_.empty(), nullptr, "Token stream is empty.");
        SERE_ASSERT(tokens_.back()->type == SereLexer::TOKEN_EOF, tokens_.back(), "Last token must be EOF.");
    }

    std::vector<std::shared_ptr<StatAST>> parse() {
        std::vector<std::shared_ptr<StatAST>> statements;
        skipNewlines();
        while (!isAtEnd()) {
            if (check(SereLexer::TOKEN_NEWLINE)) {
                advance();
                continue;
            }
            // Python allows simple_stmt (expr or assignment) at the top-level
            statements.push_back(simple_stmt());
            skipNewlines();
        }
        return statements;
    }

private:
    const std::vector<std::shared_ptr<SereLexer::TokenBase>>& tokens_;
    size_t current_;

    // === Token Helpers ===

    const std::shared_ptr<SereLexer::TokenBase>& peek() const { 
        SERE_ASSERT(current_ < tokens_.size(), nullptr, "Peek out of bounds.");
        return tokens_[current_]; 
    }
    const std::shared_ptr<SereLexer::TokenBase>& previous() const { 
        SERE_ASSERT(current_ > 0 && current_-1 < tokens_.size(), nullptr, "Previous out of bounds.");
        return tokens_[current_ - 1]; 
    }
    bool isAtEnd() const { 
        return peek()->type == SereLexer::TOKEN_EOF; 
    }
    const std::shared_ptr<SereLexer::TokenBase>& advance() { 
        if (!isAtEnd()) ++current_; 
        return previous(); 
    }
    bool check(SereLexer::TokenType type) const { 
        return !isAtEnd() && peek()->type == type; 
    }

    bool match(std::initializer_list<SereLexer::TokenType> types) {
        for (auto type : types) {
            if (check(type)) { advance(); return true; }
        }
        return false;
    }
    const std::shared_ptr<SereLexer::TokenBase>& consume(SereLexer::TokenType type, const std::string& msg) {
        if (check(type)) return advance();
        throw ParserError(peek(), msg);
    }
    void skipNewlines() {
        while (check(SereLexer::TOKEN_NEWLINE)) advance();
    }
    bool lookAheadIs(SereLexer::TokenType type) const {
        return (current_ + 1 < tokens_.size() && tokens_[current_ + 1]->type == type);
    }

    // Accept EOF as a valid statement terminator (for last line friendliness)
    void expectStatementEnd() {
        if (check(SereLexer::TOKEN_NEWLINE)) {
            advance();
        } else if (peek()->type != SereLexer::TOKEN_EOF) {
            throw ParserError(peek(), "Expected newline or EOF after statement.");
        }
    }

    // === Python-style Simple Statement (expr_stmt) ===

    std::shared_ptr<StatAST> simple_stmt() {
        // Assignment or expression statement
        if (check(SereLexer::TOKEN_IDENTIFIER)) {
            // Look ahead for assignment
            if (lookAheadIs(SereLexer::TOKEN_EQUAL) || lookAheadIs(SereLexer::TOKEN_COLON)) {
                return assignment_stmt();
            }
        }
        // Otherwise: expression statement
        auto expr = expression();
        expectStatementEnd();
        return std::make_shared<ExprStatAST>(expr);
    }

    std::shared_ptr<StatAST> assignment_stmt() {
        // assignment: NAME (':' TYPE)? '=' expr
        auto name = consume(SereLexer::TOKEN_IDENTIFIER, "Expected variable name.");
        std::shared_ptr<SereLexer::TokenBase> type = nullptr;
        if (match({SereLexer::TOKEN_COLON})) {
            type = consume(SereLexer::TOKEN_IDENTIFIER, "Expected type name after ':'.");
        }
        consume(SereLexer::TOKEN_EQUAL, "Expected '=' in assignment.");
        auto value = expression();
        expectStatementEnd();
        return std::make_shared<AssignStatAST>(*name, type ? *type : *name, value);
    }

    // === Expression Grammar (matches Python operator precedence) ===

    std::shared_ptr<ExprAST> expression() { return or_test(); }

    std::shared_ptr<ExprAST> or_test() {
        auto expr = and_test();
        while (match({SereLexer::TOKEN_OR})) {
            auto op = previous();
            auto right = and_test();
            expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
        }
        return expr;
    }
    std::shared_ptr<ExprAST> and_test() {
        auto expr = not_test();
        while (match({SereLexer::TOKEN_AND})) {
            auto op = previous();
            auto right = not_test();
            expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
        }
        return expr;
    }
    std::shared_ptr<ExprAST> not_test() {
        if (match({SereLexer::TOKEN_NOT})) {
            auto op = previous();
            auto right = not_test();
            return std::make_shared<UnaryExprAST>(op.get(), right);
        }
        return comparison();
    }
    std::shared_ptr<ExprAST> comparison() {
        auto expr = arith_expr();
        while (match({SereLexer::TOKEN_LESS, SereLexer::TOKEN_LESS_EQUAL,
                      SereLexer::TOKEN_GREATER, SereLexer::TOKEN_GREATER_EQUAL,
                      SereLexer::TOKEN_EQUAL_EQUAL, SereLexer::TOKEN_BANG_EQUAL})) {
            auto op = previous();
            auto right = arith_expr();
            expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
        }
        return expr;
    }
    std::shared_ptr<ExprAST> arith_expr() {
        auto expr = term();
        while (match({SereLexer::TOKEN_PLUS, SereLexer::TOKEN_MINUS})) {
            auto op = previous();
            auto right = term();
            expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
        }
        return expr;
    }
    std::shared_ptr<ExprAST> term() {
        auto expr = factor();
        while (match({SereLexer::TOKEN_STAR, SereLexer::TOKEN_SLASH})) {
            auto op = previous();
            auto right = factor();
            expr = std::make_shared<BinaryExprAST>(op.get(), expr, right);
        }
        return expr;
    }
    std::shared_ptr<ExprAST> factor() {
        if (match({SereLexer::TOKEN_PLUS, SereLexer::TOKEN_MINUS})) {
            // Python supports unary + and -
            auto op = previous();
            auto right = factor();
            return std::make_shared<UnaryExprAST>(op.get(), right);
        }
        return power();
    }
    std::shared_ptr<ExprAST> power() {
        auto expr = atom();
        // No "**" operator in your grammar yet, but easy to add here
        return expr;
    }
    std::shared_ptr<ExprAST> atom() {
        if (match({SereLexer::TOKEN_TRUE})) return std::make_shared<LiteralExprAST>(SereObject(true));
        if (match({SereLexer::TOKEN_FALSE})) return std::make_shared<LiteralExprAST>(SereObject(false));
        if (match({SereLexer::TOKEN_NONE})) return std::make_shared<LiteralExprAST>(SereObject());
        if (match({SereLexer::TOKEN_INTEGER})) return std::make_shared<LiteralExprAST>(SereObject(previous()->literal.INTEGER));
        if (match({SereLexer::TOKEN_FLOAT}))   return std::make_shared<LiteralExprAST>(SereObject(previous()->literal.FLOAT));
        if (match({SereLexer::TOKEN_STRING}))  return std::make_shared<LiteralExprAST>(SereObject(previous()->literal.STRING));
        if (match({SereLexer::TOKEN_IDENTIFIER})) return std::make_shared<VariableExprAST>(*previous());
        if (match({SereLexer::TOKEN_LEFT_PAREN})) {
            auto expr = expression();
            consume(SereLexer::TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
            return expr;
        }
        throw ParserError(peek(), "Expected expression.");
    }
};

} // namespace SereParser

#endif // PARSER_HPP