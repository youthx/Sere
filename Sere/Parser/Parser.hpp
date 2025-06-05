#ifndef SERE_PARSER_HPP
#define SERE_PARSER_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>
#include "../Scanner/Token.hpp"
#include "../Scanner/Scanner.hpp"
#include "AST/Expr.hpp"
#include "AST/Stat.hpp"

namespace SereParser {

// ===================== Exception Class =====================
class ParserError : public std::runtime_error {
public:
    ParserError(const std::shared_ptr<SereLexer::TokenBase>& token, std::string msg)
        : std::runtime_error(std::move(msg)), token_(token) {}

    const char* what() const noexcept override {
        try {
            if (!formatted_.empty()) return formatted_.c_str();
            std::ostringstream oss;
            oss << "[ParserError] Line " << (token_ ? token_->line : 0) << ": " << std::runtime_error::what();
            if (token_) oss << " (got '" << token_->lexeme << "')";
            formatted_ = oss.str();
            return formatted_.c_str();
        } catch (...) {
            return "[ParserError] (error formatting message)";
        }
    }

    const std::shared_ptr<SereLexer::TokenBase>& token() const noexcept { return token_; }

private:
    std::shared_ptr<SereLexer::TokenBase> token_;
    mutable std::string formatted_;
};

// ===================== Utility Macros =====================
#ifndef SERE_PARSER_ASSERT_DEFINED
#define SERE_PARSER_ASSERT_DEFINED
#define SERE_ASSERT(cond, token, msg) \
    do { if (!(cond)) throw SereParser::ParserError(token, msg); } while(0)
#endif

#define SERE_FATAL(msg) \
    throw std::logic_error(std::string("[Fatal parser error] ") + msg)


// ===================== Parser Class =====================
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
            if (check(SereLexer::TOKEN_EOF)) break;
            statements.push_back(statement());
            skipNewlines();
        }
        return statements;
    }

private:
    const std::vector<std::shared_ptr<SereLexer::TokenBase>>& tokens_;
    size_t current_;

    // ===================== Token Helpers =====================
    const std::shared_ptr<SereLexer::TokenBase>& peek() const {
        SERE_ASSERT(current_ < tokens_.size(), nullptr, "Peek out of bounds.");
        return tokens_[current_];
    }
    const std::shared_ptr<SereLexer::TokenBase>& previous() const {
        SERE_ASSERT(current_ > 0 && current_ - 1 < tokens_.size(), nullptr, "Previous out of bounds.");
        return tokens_[current_ - 1];
    }
    bool isAtEnd() const noexcept {
        return peek()->type == SereLexer::TOKEN_EOF || current_ >= tokens_.size();
    }
    const std::shared_ptr<SereLexer::TokenBase>& advance() {
        if (!isAtEnd()) ++current_;
        return previous();
    }
    bool check(SereLexer::TokenType type) const noexcept {
        return !isAtEnd() && peek()->type == type;
    }
    bool lookAheadIs(SereLexer::TokenType type) const noexcept {
        return (current_ + 1 < tokens_.size() && tokens_[current_ + 1]->type == type);
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

    // ===================== Statement End Handling =====================
    void expectStatementEnd() {
        const auto& tok = peek();
        if (tok->type == SereLexer::TOKEN_EOF) return;
        if (tok->type == SereLexer::TOKEN_NEWLINE) { advance(); return; }
        if (tok->type == SereLexer::TOKEN_DEDENT) return;
        if (current_ >= tokens_.size()) return;
        throw ParserError(tok, "Expected newline, DEDENT, or EOF after statement.");
    }
    void expectFreshLine() {
        const auto& tok = peek();
        if (tok->type == SereLexer::TOKEN_NEWLINE) {
            advance();
        } else if (tok->type == SereLexer::TOKEN_EOF) {
            return;
        } else {
            throw ParserError(tok, "Expected newline after statement.");
        }
    }

    // ===================== Type Parsing =====================
    std::shared_ptr<TypeAnnotationExprAST> parse_type() {
        // Parse a type name (e.g., int, str, List[T], etc.)
        auto name_token = consume(SereLexer::TOKEN_IDENTIFIER, "Expected type name.");
        if (check(SereLexer::TOKEN_LEFT_BRACKET)) {
            advance(); // consume '['
            auto subtype = parse_type();
            consume(SereLexer::TOKEN_RIGHT_BRACKET, "Expected ']' after type parameter.");
            return std::make_shared<TypeAnnotationExprAST>(*name_token, subtype);
        }
        return std::make_shared<TypeAnnotationExprAST>(*name_token);
    }

    // ===================== Statement Dispatch =====================
    std::shared_ptr<StatAST> statement() {
        if (check(SereLexer::TOKEN_DEF)) {
            return funcdef_stmt();
        } else if (check(SereLexer::TOKEN_RETURN)) {
            return return_stmt();
        } else if (check(SereLexer::TOKEN_IF)) {
            //return if_stmt();
        } else if (check(SereLexer::TOKEN_WHILE)) {
            //return while_stmt();
        } else if (check(SereLexer::TOKEN_CLASS)) {
            //return class_stmt();
        } else if (check(SereLexer::TOKEN_IMPORT) || check(SereLexer::TOKEN_FROM)) {
            //return import_stmt();
        } else if (check(SereLexer::TOKEN_IDENTIFIER)) {
            if (lookAheadIs(SereLexer::TOKEN_EQUAL) || lookAheadIs(SereLexer::TOKEN_COLON)) {
                return assignment_stmt();
            }
        }
        return expr_stmt();
    }

    // ===================== Function Definition =====================
    std::shared_ptr<StatAST> funcdef_stmt() {
        auto def_tok = consume(SereLexer::TOKEN_DEF, "Expected 'def' keyword.");
        auto name = consume(SereLexer::TOKEN_IDENTIFIER, "Expected function name after 'def'.");
        consume(SereLexer::TOKEN_LEFT_PAREN, "Expected '(' after function name.");
        std::vector<std::shared_ptr<VariableExprAST>> params;

        if (!check(SereLexer::TOKEN_RIGHT_PAREN)) {
            do {
                auto param_name = consume(SereLexer::TOKEN_IDENTIFIER, "Expected parameter name.");
                std::shared_ptr<TypeAnnotationExprAST> param_type = nullptr;
                if (match({SereLexer::TOKEN_COLON})) {
                    param_type = parse_type();
                }
                params.push_back(std::make_shared<VariableExprAST>(*param_name, param_type));
            } while (match({SereLexer::TOKEN_COMMA}));
        }

        consume(SereLexer::TOKEN_RIGHT_PAREN, "Expected ')' after parameters.");
        std::shared_ptr<TypeAnnotationExprAST> return_type = nullptr;
        if (match({SereLexer::TOKEN_ARROW})) {
            return_type = parse_type();
        }
        consume(SereLexer::TOKEN_COLON, "Expected ':' after function signature.");
        auto body = block_stmt();
        return std::make_shared<FunctionStatAST>(*name, params, body, return_type);
    }

    // ===================== Return Statement =====================
    std::shared_ptr<StatAST> return_stmt() {
        consume(SereLexer::TOKEN_RETURN, "Expected 'return' keyword.");
        std::shared_ptr<ExprAST> value = nullptr;
        if (!check(SereLexer::TOKEN_NEWLINE) && !check(SereLexer::TOKEN_EOF)) {
            value = expression();
        }
        expectStatementEnd();
        return std::make_shared<ReturnStatAST>(value);
    }

    // ===================== Block Parsing =====================
    std::shared_ptr<StatAST> block_stmt() {
        expectFreshLine();
        consume(SereLexer::TOKEN_INDENT, "Expected INDENT to start block.");
        std::vector<std::shared_ptr<StatAST>> statements;
        skipNewlines();
        while (!check(SereLexer::TOKEN_DEDENT) && !isAtEnd()) {
            if (check(SereLexer::TOKEN_NEWLINE)) {
                advance();
                continue;
            }
            if (check(SereLexer::TOKEN_EOF)) break;
            statements.push_back(statement());
            skipNewlines();
        }
        consume(SereLexer::TOKEN_DEDENT, "Expected DEDENT to end block.");
        return std::make_shared<BlockStatAST>(std::move(statements));
    }

    // ===================== Assignment =====================
    std::shared_ptr<StatAST> assignment_stmt() {
        auto name = consume(SereLexer::TOKEN_IDENTIFIER, "Expected variable name.");
        std::shared_ptr<TypeAnnotationExprAST> type = nullptr;
        if (match({SereLexer::TOKEN_COLON})) {
            type = parse_type();
        }
        consume(SereLexer::TOKEN_EQUAL, "Expected '=' in assignment.");
        auto value = expression();
        expectStatementEnd();
        return std::make_shared<AssignStatAST>(*name, value, type);
    }

    // ===================== Expression Statement =====================
    std::shared_ptr<StatAST> expr_stmt() {
        auto expr = expression();
        expectStatementEnd();
        return std::make_shared<ExprStatAST>(expr);
    }

    // ===================== Expression Grammar =====================
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
            auto op = previous();
            auto right = factor();
            return std::make_shared<UnaryExprAST>(op.get(), right);
        }
        return power();
    }
    std::shared_ptr<ExprAST> power() {
        auto expr = call();
        // "**" operator could be added here if needed
        return expr;
    }
    std::shared_ptr<ExprAST> call() {
        
        if (match({SereLexer::TOKEN_IDENTIFIER})) {
            auto callee = *previous();
            while (true) {
                if (match({SereLexer::TOKEN_LEFT_PAREN})) {
                    return finish_call(callee);
                } else {
                    return std::make_shared<VariableExprAST>(callee);
                }
            }

        }
        return atom();
    }

    std::shared_ptr<ExprAST> finish_call(SereLexer::TokenBase callee) {
        std::vector<std::shared_ptr<ExprAST>> arguments;
        if (!check(SereLexer::TOKEN_RIGHT_PAREN)) {
            do {
                arguments.push_back(expression());
            } while (match({SereLexer::TOKEN_COMMA}));
        }
        consume(SereLexer::TOKEN_RIGHT_PAREN, "Expected ')' after arguments.");
        return std::make_shared<CallExprAST>(callee, std::move(arguments));
    }
    std::shared_ptr<ExprAST> atom() {
        if (match({SereLexer::TOKEN_TRUE})) return std::make_shared<LiteralExprAST>(SereObject(true));
        if (match({SereLexer::TOKEN_FALSE})) return std::make_shared<LiteralExprAST>(SereObject(false));
        if (match({SereLexer::TOKEN_NONE})) return std::make_shared<LiteralExprAST>(SereObject());
        if (match({SereLexer::TOKEN_INTEGER})) return std::make_shared<LiteralExprAST>(SereObject(previous()->literal.INTEGER));
        if (match({SereLexer::TOKEN_FLOAT}))   return std::make_shared<LiteralExprAST>(SereObject(previous()->literal.FLOAT));
        if (match({SereLexer::TOKEN_STRING}))  return std::make_shared<LiteralExprAST>(SereObject(previous()->literal.STRING));
        if (match({SereLexer::TOKEN_IDENTIFIER})) {return std::make_shared<VariableExprAST>(*previous());}
        if (match({SereLexer::TOKEN_LEFT_PAREN})) {
            auto expr = expression();
            consume(SereLexer::TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
            return expr;
        }
        throw ParserError(peek(), "Expected expression.");
    }
};

} // namespace SereParser

#endif // SERE_PARSER_HPP
