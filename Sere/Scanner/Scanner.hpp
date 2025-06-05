#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <cassert>
#include <stdexcept>
#include <limits>
#include <memory>
#include <algorithm>
#include <utility>
#include <cerrno>
#include <climits>
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
          line(1), column(1), at_line_start(true), current_indent(0), paren_level(0)
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
        // Ensure last token is NEWLINE if not already
        if (!token_list.getTokens().empty() && token_list.getTokens().back().get()->type != TOKEN_NEWLINE) {
            add_token(TOKEN_NEWLINE);
        }
        // Add EOF token at the end, with correct line/column
        token_list.add_token<int>(Token<int>(TOKEN_EOF, "", line, column));
        return token_list;
    }

private:
    std::string buffer;
    TokenList token_list;
    int line, column;
    int current, start;
    bool at_line_start;
    int current_indent;
    int paren_level;
    std::vector<int> indent_stack;

    const std::map<std::string, TokenType> Tok_keywords {
        {"and", TOKEN_AND}, {"as", TOKEN_AS}, {"assert", TOKEN_ASSERT},
        {"break", TOKEN_BREAK}, {"class", TOKEN_CLASS}, {"continue", TOKEN_CONTINUE},
        {"def", TOKEN_DEF}, {"del", TOKEN_DEL}, {"elif", TOKEN_ELIF},
        {"else", TOKEN_ELSE}, {"except", TOKEN_EXCEPT}, {"False", TOKEN_FALSE},
        {"finally", TOKEN_FINALLY}, {"for", TOKEN_FOR}, {"from", TOKEN_FROM},
        {"global", TOKEN_GLOBAL}, {"if", TOKEN_IF}, {"import", TOKEN_IMPORT},
        {"in", TOKEN_IN}, {"is", TOKEN_IS}, {"lambda", TOKEN_LAMBDA},
        {"None", TOKEN_NONE}, {"nonlocal", TOKEN_NONLOCAL}, {"not", TOKEN_NOT},
        {"or", TOKEN_OR}, {"pass", TOKEN_PASS}, {"raise", TOKEN_RAISE},
        {"return", TOKEN_RETURN}, {"self", TOKEN_SELF}, {"super", TOKEN_SUPER},
        {"True", TOKEN_TRUE}, {"try", TOKEN_TRY}, {"while", TOKEN_WHILE},
        {"with", TOKEN_WITH}, {"yield", TOKEN_YIELD}
    };

    void reset_position() {
        line = 1; column = 1; current = 0; start = 0;
        current_indent = 0; indent_stack.clear();
        token_list.clear();
        at_line_start = true;
        paren_level = 0;
        SCANNER_DEBUG_LOG("Position reset");
    }

    bool is_at_end() const noexcept {
        return SCANNER_UNLIKELY(current >= static_cast<int>(buffer.length()));
    }

    char advance() {
        if (is_at_end()) return '\0';
        char c = buffer[current++];
        if (c == '\n') { line++; column = 1; at_line_start = true; }
        else { column++; }
        SCANNER_DEBUG_LOG("Advanced to char: " << c << " (line " << line << ", col " << column << ")");
        return c;
    }

    char peek() const noexcept {
        if (is_at_end()) return '\0';
        return buffer[current];
    }

    char peek_next() const noexcept {
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

    void scan_one() {
        // Scan indent only at the true start of a line, and skip blank lines
        if (at_line_start && paren_level == 0) {
            int nl_count = 0;
            // Eat all blank lines (emit NEWLINE for each)
            while (peek() == '\n') {
                start = current;
                advance();
                add_token(TOKEN_NEWLINE);
                nl_count++;
                at_line_start = true;
            }
            // After blank lines, scan indentation (if not EOF or comment)
            if (!is_at_end() && peek() != '#' && peek() != '\n') {
                scan_indent();
                at_line_start = false;
            }
            // If it was just blank lines or EOF, return (don't double-read)
            if (peek() == '\n' || is_at_end()) return;
        }

        char c = advance();
        switch (c) {
        case '\n':
            if (paren_level == 0) {
                add_token(TOKEN_NEWLINE);
                at_line_start = true;
            }
            break;
        case '\0':
            break;
        case ' ':
        case '\t':
            // Ignore whitespace except for indentation (handled at line start)
            break;
        case '#':
            while (peek() != '\n' && !is_at_end()) advance();
            break;
        case '(': paren_level++; add_token(TOKEN_LEFT_PAREN); break;
        case ')': if (paren_level > 0) paren_level--; add_token(TOKEN_RIGHT_PAREN); break;
        case '{': add_token(TOKEN_LEFT_BRACE); break;
        case '}': add_token(TOKEN_RIGHT_BRACE); break;
        case '[': paren_level++; add_token(TOKEN_LEFT_BRACKET); break;
        case ']': if (paren_level > 0) paren_level--; add_token(TOKEN_RIGHT_BRACKET); break;
        case ',': add_token(TOKEN_COMMA); break;
        case '.': 
            if (std::isdigit(static_cast<unsigned char>(peek()))) {
                scan_number(true); // float starting with . (e.g. .5)
            } else {
                add_token(TOKEN_DOT);
            }
            break;
        case '+': add_token(match('=') ? TOKEN_PLUS_EQUAL : TOKEN_PLUS); break;
        case '-': add_token(match('=') ? TOKEN_MINUS_EQUAL : (match('>') ? TOKEN_ARROW : TOKEN_MINUS)); break;
        case '*': add_token(match('=') ? TOKEN_STAR_EQUAL : (match('*') ? (match('=') ? TOKEN_DOUBLE_STAR_EQUAL : TOKEN_DOUBLE_STAR) : TOKEN_STAR)); break;
        case '/': add_token(match('=') ? TOKEN_SLASH_EQUAL : (match('/') ? (match('=') ? TOKEN_DOUBLE_SLASH_EQUAL : TOKEN_DOUBLE_SLASH) : TOKEN_SLASH)); break;
        case '%': add_token(match('=') ? TOKEN_PERCENT_EQUAL : TOKEN_PERCENT); break;
        case ':': add_token(match('=') ? TOKEN_COLON_EQUAL : TOKEN_COLON); break;
        case ';': add_token(TOKEN_SEMICOLON); break;
        case '|': add_token(match('=') ? TOKEN_PIPE_EQUAL : (match('|') ? TOKEN_DOUBLE_PIPE : TOKEN_PIPE)); break;
        case '&': add_token(match('=') ? TOKEN_AMPERSAND_EQUAL : (match('&') ? TOKEN_DOUBLE_AMPERSAND : TOKEN_AMPERSAND)); break;
        case '^': add_token(match('=') ? TOKEN_CARET_EQUAL : TOKEN_CARET); break;
        case '~': add_token(TOKEN_TILDE); break;
        case '!': add_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG); break;
        case '=': add_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL); break;
        case '<':
            if (match('<')) {
                add_token(match('=') ? TOKEN_LEFT_SHIFT_EQUAL : TOKEN_LEFT_SHIFT);
            } else {
                add_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
            }
            break;
        case '>':
            if (match('>')) {
                add_token(match('=') ? TOKEN_RIGHT_SHIFT_EQUAL : TOKEN_RIGHT_SHIFT);
            } else {
                add_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
            }
            break;
        case '\'':
        case '"':
            scan_string(c);
            break;
        case '\\':
            // Line continuation
            if (peek() == '\n') {
                advance();
                at_line_start = true;
            }
            break;
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                scan_number(false, c);
            } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                scan_identifier(c);
            } else {
                Error::error(line, "Unexpected character: '" + std::string(1, c) + "'");
            }
            break;
        }
    }

    // INDENT/DEDENT LOGIC (Python-style)
    void scan_indent() {
        int spaces = 0;
        int orig_current = current;
        int orig_column = column;
        int count = 0;
        // Only count spaces/tabs at the very start of the line
        while (!is_at_end() && (buffer[current] == ' ' || buffer[current] == '\t')) {
            spaces += (buffer[current] == ' ') ? 1 : 8; // treat tab as 8 spaces (Python default)
            ++current;
            ++count;
        }
        column += count; // reflects the actual horizontal position

        if (spaces > current_indent) {
            // New indent
            indent_stack.push_back(current_indent);
            current_indent = spaces;
            add_token(TOKEN_INDENT);
        } else if (spaces < current_indent) {
            // DEDENT(s)
            while (!indent_stack.empty() && spaces < current_indent) {
                add_token(TOKEN_DEDENT);
                current_indent = indent_stack.back();
                indent_stack.pop_back();
            }
            if (spaces != current_indent) {
                Error::error(line, "Indentation error.");
            }
        }
        // If spaces matches current_indent, do nothing
        SCANNER_DEBUG_LOG("Indentation scanned: " << spaces << " (current_indent: " << current_indent << ")");
    }

    // Accepts float_start_dot for .5, -.5, etc.
    void scan_number(bool float_start_dot = false, char first_char = '\0') {
        int number_start = float_start_dot ? current - 1 : start;
        bool is_float = float_start_dot;
        std::string num_str;
        if (float_start_dot) {
            num_str = "0.";
        } else if (first_char) {
            num_str = first_char;
        }

        // Integer part
        while (std::isdigit(static_cast<unsigned char>(peek()))) num_str += advance();

        // Fractional part
        if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next()))) {
            is_float = true;
            num_str += advance(); // '.'
            while (std::isdigit(static_cast<unsigned char>(peek()))) num_str += advance();
        }

        // Exponent part
        if (peek() == 'e' || peek() == 'E') {
            is_float = true;
            num_str += advance();
            if (peek() == '+' || peek() == '-') num_str += advance();
            if (!std::isdigit(static_cast<unsigned char>(peek()))) {
                Error::error(line, "Invalid numeric literal: missing exponent digits");
                return;
            }
            while (std::isdigit(static_cast<unsigned char>(peek()))) num_str += advance();
        }

        // Integer literal bases (0x, 0o, 0b)
        if (!is_float && num_str == "0" && (peek() == 'x' || peek() == 'X' || peek() == 'o' || peek() == 'O' || peek() == 'b' || peek() == 'B')) {
            char base_char = advance();
            num_str += base_char;
            int base = 10;
            if (base_char == 'x' || base_char == 'X') base = 16;
            else if (base_char == 'o' || base_char == 'O') base = 8;
            else if (base_char == 'b' || base_char == 'B') base = 2;
            while (std::isxdigit(static_cast<unsigned char>(peek())) || (base == 2 && (peek() == '0' || peek() == '1'))) {
                num_str += advance();
            }
            try {
                errno = 0;
                long long val = std::stoll(num_str, nullptr, base);
                if (errno == ERANGE || val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
                    throw std::out_of_range("Integer literal out of range");
                add_token<int>(TOKEN_INTEGER, static_cast<int>(val));
            } catch (const std::exception& e) {
                Error::error(line, std::string("Invalid numeric literal: ") + e.what());
            }
            return;
        }

        try {
            if (is_float) {
                errno = 0;
                double val = std::stod(num_str);
                if (errno == ERANGE)
                    throw std::out_of_range("Float literal out of range");
                add_token<double>(TOKEN_FLOAT, val);
            } else {
                errno = 0;
                long long val = std::stoll(num_str);
                if (errno == ERANGE || val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
                    throw std::out_of_range("Integer literal out of range");
                add_token<int>(TOKEN_INTEGER, static_cast<int>(val));
            }
        } catch (const std::exception& e) {
            Error::error(line, std::string("Invalid numeric literal: ") + e.what());
        }
    }

    // Handles single/double/triple quoted strings, escapes, and multi-line
    void scan_string(char quote_type) {
        bool triple = false;
        int string_line = line;
        int string_column = column;
        std::string value;
        // Triple quote? Only check if enough characters remain
        if (peek() == quote_type && peek_next() == quote_type) {
            if (current + 2 < static_cast<int>(buffer.size())) {
                triple = true;
                advance(); advance();
            }
        }
        bool closed = false;
        while (!is_at_end()) {
            char c = peek();
            if (!triple && c == '\n') {
                // Single-line string cannot contain newlines
                break;
            }
            if (c == quote_type) {
                if (triple) {
                    // Only check for triple quote if enough characters remain
                    if (current + 2 < static_cast<int>(buffer.size()) &&
                        peek_next() == quote_type && buffer[current + 2] == quote_type) {
                        advance(); advance(); advance();
                        closed = true;
                        break;
                    } else {
                        value += advance();
                    }
                } else {
                    advance();
                    closed = true;
                    break;
                }
            } else if (c == '\\') {
                advance();
                char next = peek();
                switch (next) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '\'': value += '\''; break;
                case '"': value += '"'; break;
                case 'a': value += '\a'; break;
                case 'b': value += '\b'; break;
                case 'f': value += '\f'; break;
                case 'v': value += '\v'; break;
                case '0': value += '\0'; break;
                case '\n': break; // line continuation
                default: value += next; break;
                }
                advance();
            } else {
                value += advance();
            }
        }
        if (!closed) {
            Error::error(string_line, "Unterminated string literal");
            return;
        }
        add_token<std::string>(TOKEN_STRING, value);
    }

    // Accepts the first character (already read) for correct identifier start
    void scan_identifier(char first_char) {
        std::string text(1, first_char);
        while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')
            text += advance();
        // Check for keyword
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
