#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include <string>
#include "Token.hpp"
#include "../errors.hpp"

namespace SereLexer {

class Scanner {
public:
    Scanner() = delete; // Disable default constructor to enforce proper initialization

    explicit Scanner(const char* input_buffer) 
        : start(0), current(0), line(1), column(1), buffer(input_buffer ? input_buffer : "") {}

    TokenList tokenize() {
        reset_position();

        while (!is_at_end()) {
            start = current;
            scan_one();
        }

        // Add EOF token safely
        token_list.add_token<void>(Token<void>(TOKEN_EOF, "", line, column));
        return token_list; // Return a copy of the token list
    }

private:
    std::string buffer; // Use std::string for safer and more flexible string handling
    TokenList token_list; // Assuming TokenList is a valid type and properly defined

    int line;
    int column;

    int current;
    int start;

    int current_indent = 0; // Track the current indentation level
    std::vector<int> indent_stack; // Stack to manage indentation levels

    void reset_position() {
        line = 1;
        column = 1;
        current = 0;
        start = 0;
    }

    bool is_at_end() const {
        return current >= buffer.length();
    }

    char advance() {
        if (is_at_end()) {
            return '\0'; // Return null character if at end of buffer
        }
        char c = buffer[current++];
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        return c;
    }

    char peek() const {
        if (is_at_end()) {
            return '\0'; // Return null character if at end of buffer
        }
        return buffer[current];
    }

    bool match(char expected_char) {
        if (is_at_end() || buffer[current] != expected_char) {
            return false;
        }
        current++;
        column++;
        return true;
    }

    void add_token(TokenType type) {
        std::string text = buffer.substr(start, current - start);
        token_list.add_token<void>(Token<void>(type, text, line, column));
    }

    template <typename T>
    void add_token(TokenType type, T literal) {
        std::string text = buffer.substr(start, current - start);
        token_list.add_token<T>(Token<T>(type, text, literal, line, column));
    }

    void scan_indent() {
        int spaces = 0;

        // Count spaces or tabs at the start of the line
        while (!is_at_end() && (buffer[current] == ' ' || buffer[current] == '\t')) {
            if (buffer[current] == ' ') {
                spaces++;
            } else if (buffer[current] == '\t') {
                spaces += 4; // Assuming a tab equals 4 spaces
            }
            advance();
        }

        // Compare the current indentation level with the previous one
        if (spaces > current_indent) {
            add_token(TOKEN_INDENT);
            indent_stack.push_back(spaces);
        } else if (spaces < current_indent) {
            while (!indent_stack.empty() && spaces < indent_stack.back()) {
                add_token(TOKEN_DEDENT);
                indent_stack.pop_back();
            }
        }

        current_indent = spaces;
    }

    void scan_one() {
        char c = advance();

        switch (c) {
            case ' ':
            case '\t':
                // Skip whitespace but scan for indentation at the start of a line
                if (column == 1) {
                    scan_indent();
                }
                break;

            case '\n':
                // Reset column for new line
                column = 1;
                break;

            case '#':
                // Skip comments until the end of the line
                while (peek() != '\n' && !is_at_end()) {
                    advance();
                }
                break;

            case '(':
                add_token(TOKEN_LEFT_PAREN);
                break;
            case ')':
                add_token(TOKEN_RIGHT_PAREN);
                break;
            case '{':
                add_token(TOKEN_LEFT_BRACE);
                break;
            case '}':
                add_token(TOKEN_RIGHT_BRACE);
                break;
            case ',':
                add_token(TOKEN_COMMA);
                break;
            case '.':
                add_token(TOKEN_DOT);
                break;
            case '-':
                add_token(TOKEN_MINUS);
                break;
            case '+':
                add_token(TOKEN_PLUS);
                break;
            case ';':
                add_token(TOKEN_SEMICOLON);
                break;
            case '*':
                add_token(TOKEN_STAR);
                break;

            default:
                scan_two(c);
                break;
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

            default:
                Error::error(line, "Unexpected character.");
                break;
        }
    }
};

} // namespace SereLexer

#endif // SCANNER_HPP
