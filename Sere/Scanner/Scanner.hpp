#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <cassert>
#include <stdexcept>
#include <limits>
#include "Token.hpp"
#include "../../errors.hpp"

// Debug macro: Enable debug output if needed
#ifdef SCANNER_DEBUG
#include <iostream>
#define SCANNER_DEBUG_LOG(msg) std::cerr << "[Scanner Debug] " << msg << std::endl
#else
#define SCANNER_DEBUG_LOG(msg) ((void)0)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SCANNER_LIKELY(x)   __builtin_expect(!!(x), 1)
#define SCANNER_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SCANNER_LIKELY(x)   (x)
#define SCANNER_UNLIKELY(x) (x)
#endif

namespace SereLexer {

class Scanner {
public:
    Scanner() = delete;

    explicit Scanner(const char* input_buffer)
        : buffer(input_buffer ? input_buffer : ""), current(0), start(0),
          line(1), column(1), current_indent(0)
    {
        SCANNER_DEBUG_LOG("Scanner initialized");
    }

    TokenList tokenize() {
        reset_position();
        while (!is_at_end()) {
            start = current;
            scan_one();
        }
        // Emit remaining DEDENTs at EOF
        while (!indent_stack.empty()) {
            add_token(TOKEN_DEDENT);
            indent_stack.pop_back();
        }
        // Add EOF
        token_list.add_token<int>(Token<int>(TOKEN_EOF, "", line, column));
        return token_list;
    }

private:
    std::string buffer;
    TokenList token_list;
    int line, column;
    int current, start;
    int current_indent;
    std::vector<int> indent_stack;

    const std::map<std::string, TokenType> Tok_keywords {
        {"and", TOKEN_AND},
        {"class", TOKEN_CLASS},
        {"def", TOKEN_DEF},
        {"elif", TOKEN_ELIF},
        {"else", TOKEN_ELSE},
        {"False", TOKEN_FALSE},
        {"for", TOKEN_FOR},
        {"if", TOKEN_IF},
        {"not", TOKEN_NOT},
        {"or", TOKEN_OR},
        {"return", TOKEN_RETURN},
        {"self", TOKEN_SELF},
        {"super", TOKEN_SUPER},
        {"None", TOKEN_NONE},
        {"True", TOKEN_TRUE},
        {"while", TOKEN_WHILE}
    };

    void reset_position() {
        line = 1; column = 1; current = 0; start = 0;
        current_indent = 0; indent_stack.clear();
        token_list.clear();
        SCANNER_DEBUG_LOG("Position reset");
    }

    bool is_at_end() const {
        return SCANNER_UNLIKELY(current >= static_cast<int>(buffer.length()));
    }

    char advance() {
        if (is_at_end()) return '\0';
        char c = buffer[current++];
        if (c == '\n') { line++; column = 1; }
        else { column++; }
        SCANNER_DEBUG_LOG("Advanced to char: " << c << " (line " << line << ", col " << column << ")");
        return c;
    }

    char peek() const {
        if (is_at_end()) return '\0';
        return buffer[current];
    }

    char peek_next() const {
        if (current + 1 >= static_cast<int>(buffer.length())) return '\0';
        return buffer[current + 1];
    }

    bool match(char expected_char) {
        if (is_at_end() || buffer[current] != expected_char) return false;
        current++; column++;
        return true;
    }

    void add_token(TokenType type) {
        std::string text = buffer.substr(start, current - start);
        token_list.add_token<int>(Token<int>(type, text, line, column));
        SCANNER_DEBUG_LOG("Token added: " << text << " type: " << type);
    }
    template <typename T>
    void add_token(TokenType type, T literal) {
        std::string text = buffer.substr(start, current - start);
        token_list.add_token<T>(Token<T>(type, text, literal, line, column));
        SCANNER_DEBUG_LOG("Token added: " << text << " type: " << type << " literal: " << literal);
    }

    // MAIN TOKENIZATION
    void scan_one() {
        char c = advance();
        switch (c) {
        case '\n':
            add_token(TOKEN_NEWLINE);
            scan_indent();
            break;
        case '\0':
            break;
        case ' ':
        case '\t':
            // Only trigger indent detection if at col 1 (start of line).
            if (column <= 2) scan_indent(); // column is post-incremented
            break;
        case '#':
            while (peek() != '\n' && !is_at_end()) advance();
            break;
        case '(': add_token(TOKEN_LEFT_PAREN); break;
        case ')': add_token(TOKEN_RIGHT_PAREN); break;
        case '{': add_token(TOKEN_LEFT_BRACE); break;
        case '}': add_token(TOKEN_RIGHT_BRACE); break;
        case ',': add_token(TOKEN_COMMA); break;
        case '.': add_token(TOKEN_DOT); break;
        case '-': add_token(TOKEN_MINUS); break;
        case '+': add_token(TOKEN_PLUS); break;
        case ';': add_token(TOKEN_SEMICOLON); break;
        case ':': add_token(TOKEN_COLON); break;
        case '*': add_token(TOKEN_STAR); break;
        default: scan_two(c); break;
        }
    }

    void scan_two(char c) {
        switch (c) {
        case '!':
            add_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
            break;
        case '=':
            add_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
            break;
        case '<':
            add_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
            break;
        case '>':
            add_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
            break;
        case '"':
            scan_string();
            break;
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                scan_number();
            } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                scan_identifier();
            } else {
                Error::error(line, "Unexpected character: '" + std::string(1, c) + "'");
            }
            break;
        }
    }

    // INDENT/DEDENT LOGIC (Python-style)
    void scan_indent() {
        int spaces = 0;
        int old_current = current;
        // Only count spaces/tabs at the start of the line
        while (!is_at_end() && (buffer[current] == ' ' || buffer[current] == '\t')) {
            spaces += (buffer[current] == ' ') ? 1 : 4;
            ++current;
        }
        // Only emit tokens if at start of line (not for mid-line whitespace)
        if (column == 1) {
            if (spaces > current_indent) {
                indent_stack.push_back(current_indent);
                current_indent = spaces;
                add_token(TOKEN_INDENT);
            } else if (spaces < current_indent) {
                while (!indent_stack.empty() && spaces < current_indent) {
                    add_token(TOKEN_DEDENT);
                    current_indent = indent_stack.back();
                    indent_stack.pop_back();
                }
                if (spaces != current_indent)
                    Error::error(line, "Indentation error.");
            }
        }
        // Restore current if this was not a real indent
        if (column != 1) current = old_current;
        SCANNER_DEBUG_LOG("Indentation scanned: " << spaces);
    }

    void scan_number() {
        int number_start = start;
        bool is_float = false;
        while (std::isdigit(static_cast<unsigned char>(peek()))) advance();
        if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next()))) {
            is_float = true;
            advance(); // Consume '.'
            while (std::isdigit(static_cast<unsigned char>(peek()))) advance();
        }
        std::string num_str = buffer.substr(number_start, current - number_start);
        try {
            if (is_float) {
                float val = std::stof(num_str);
                add_token<float>(TOKEN_FLOAT, val);
            } else {
                long long val = std::stoll(num_str);
                if (val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
                    throw std::out_of_range("Integer literal out of range");
                add_token<int>(TOKEN_INTEGER, static_cast<int>(val));
            }
        } catch (const std::exception& e) {
            Error::error(line, std::string("Invalid numeric literal: ") + e.what());
        }
    }

    void scan_string() {
        std::string value;
        while (!is_at_end() && peek() != '"') {
            if (peek() == '\\') { // Handle escape sequences
                advance();
                char next = peek();
                switch (next) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default: value += next; break;
                }
                advance();
            } else if (peek() == '\n') {
                // Strings cannot cross newlines
                Error::error(line, "Unterminated string.");
                return;
            } else {
                value += advance();
            }
        }
        if (is_at_end()) {
            Error::error(line, "Unterminated string.");
            return;
        }
        advance(); // Closing quote
        add_token<std::string>(TOKEN_STRING, value);
    }

    void scan_identifier() {
        while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')
            advance();
        std::string text = buffer.substr(start, current - start);
        auto it = Tok_keywords.find(text);
        if (it != Tok_keywords.end()) {
            add_token(it->second);
        } else {
            add_token(TOKEN_IDENTIFIER);
        }
    }
};

} // namespace SereLexer

#endif // SCANNER_HPP