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
#include "../errors.hpp"

// Debug macro: Enable debug output if needed
#ifdef SCANNER_DEBUG
#include <iostream>
#define SCANNER_DEBUG_LOG(msg) std::cerr << "[Scanner Debug] " << msg << std::endl
#else
#define SCANNER_DEBUG_LOG(msg) ((void)0)
#endif

// Security: Prevent buffer overflows and integer overflows
#define SCANNER_SAFE_ADVANCE(ptr, len) \
    do { \
        assert((ptr) >= 0 && (ptr) <= (len)); \
    } while (0)

// Optimization: Likely/unlikely branch prediction hints (GCC/Clang)
#if defined(__GNUC__) || defined(__clang__)
#define SCANNER_LIKELY(x)   __builtin_expect(!!(x), 1)
#define SCANNER_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SCANNER_LIKELY(x)   (x)
#define SCANNER_UNLIKELY(x) (x)
#endif

namespace SereLexer
{

    class Scanner
    {
    public:
        Scanner() = delete; // Disable default constructor to enforce proper initialization

        explicit Scanner(const char *input_buffer)
            : start(0), current(0), line(1), column(1), buffer(input_buffer ? input_buffer : "") 
        {
            SCANNER_DEBUG_LOG("Scanner initialized");
        }

        TokenList tokenize()
        {
            reset_position();

            while (SCANNER_LIKELY(!is_at_end()))
            {
                start = current;
                scan_one();
            }

            // Add EOF token safely
            token_list.add_token<int>(Token<int>(TOKEN_EOF, "", line, column));
            return token_list; // Return a copy of the token list
        }

    private:
        // Member variables
        std::string buffer;   // Use std::string for safer and more flexible string handling
        TokenList token_list; // Assuming TokenList is a valid type and properly defined

        int line;
        int column;

        int current;
        int start;

        int current_indent = 0;        // Track the current indentation level
        std::vector<int> indent_stack; // Stack to manage indentation levels

        const std::map<std::string, TokenType> Tok_keywords {
            {"and", TOKEN_AND},
            // {"as", TOKEN_AS}, // Undefined token type
            // {"assert", TOKEN_ASSERT}, // Undefined token type
            // {"break", TOKEN_BREAK}, // Undefined token type
            {"class", TOKEN_CLASS},
            // {"continue", TOKEN_CONTINUE}, // Undefined token type
            {"def", TOKEN_DEF},
            // {"del", TOKEN_DEL}, // Undefined token type
            {"elif", TOKEN_ELIF}, // Undefined token type
            {"else", TOKEN_ELSE},
            // {"except", TOKEN_EXCEPT}, // Undefined token type
            {"False", TOKEN_FALSE},
            // {"finally", TOKEN_FINALLY}, // Undefined token type
            {"for", TOKEN_FOR},
            // {"from", TOKEN_FROM}, // Undefined token type
            // {"global", TOKEN_GLOBAL}, // Undefined token type
            {"if", TOKEN_IF},
            // {"import", TOKEN_IMPORT}, // Undefined token type
            // {"in", TOKEN_IN}, // Undefined token type
            // {"is", TOKEN_IS}, // Undefined token type
            // {"lambda", TOKEN_LAMBDA}, // Undefined token type
            {"None", TOKEN_NONE},
            // {"nonlocal", TOKEN_NONLOCAL}, // Undefined token type
            {"not", TOKEN_NOT}, // Undefined token type
            {"or", TOKEN_OR},
            // {"pass", TOKEN_PASS}, // Undefined token type
            // {"raise", TOKEN_RAISE}, // Undefined token type
            {"return", TOKEN_RETURN},
            {"self", TOKEN_SELF},
            {"super", TOKEN_SUPER},
            {"True", TOKEN_TRUE},
            // {"try", TOKEN_TRY}, // Undefined token type
            {"while", TOKEN_WHILE}, // Undefined token type
            // {"with", TOKEN_WITH}, // Undefined token type
            // {"yield", TOKEN_YIELD} // Undefined token type
        };    

        // Utility methods
        void reset_position()
        {
            line = 1;
            column = 1;
            current = 0;
            start = 0;
            current_indent = 0;
            indent_stack.clear();
            SCANNER_DEBUG_LOG("Position reset");
        }

        bool is_at_end() const
        {
            return SCANNER_UNLIKELY(current >= static_cast<int>(buffer.length()));
        }

        char advance()
        {
            SCANNER_SAFE_ADVANCE(current, buffer.length());
            if (is_at_end())
            {
                return '\0'; // Return null character if at end of buffer
            }
            char c = buffer[current++];
            if (c == '\n')
            {
                line++;
                column = 1;
            }
            else
            {
                column++;
            }
            SCANNER_DEBUG_LOG("Advanced to char: " << c << " (line " << line << ", col " << column << ")");
            return c;
        }

        char peek() const
        {
            if (is_at_end())
            {
                return '\0'; // Return null character if at end of buffer
            }
            return buffer[current];
        }

        char peek_next() const
        {
            if (current + 1 >= static_cast<int>(buffer.length()))
            {
                return '\0'; // Return null character if at end of buffer
            }
            return buffer[current + 1];
        }

        bool match(char expected_char)
        {
            if (is_at_end() || buffer[current] != expected_char)
            {
                return false;
            }
            current++;
            column++;
            return true;
        }

        // Token management
        void add_token(TokenType type)
        {
            std::string text = buffer.substr(start, current - start);
            token_list.add_token<int>(Token<int>(type, text, line, column));
            SCANNER_DEBUG_LOG("Token added: " << text << " type: " << type);
        }

        template <typename T>
        void add_token(TokenType type, T literal)
        {
            std::string text = buffer.substr(start, current - start);
            token_list.add_token<T>(Token<T>(type, text, literal, line, column));
            SCANNER_DEBUG_LOG("Token added: " << text << " type: " << type << " literal: " << literal);
        }

        // Scanning methods
        void scan_one()
        {
            char c = advance();

            switch (c)
            {
            case '\n': add_token(TOKEN_NEWLINE);
            case '\0': break;
            case ' ':
            case '\r':
            case '\t':
                // Handle indentation at the start of a line
                if (column == 1)
                {
                    scan_indent();
                }
                break;
            case '#':
                // Skip comments until the end of the line
                while (peek() != '\n' && !is_at_end())
                {
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
            case ':':
                add_token(TOKEN_COLON);
                break;
            case '*':
                add_token(TOKEN_STAR);
                break;

            default:
                scan_two(c);
                break;
            }
        }

        void scan_two(char c)
        {
            switch (c)
            {
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
                if (std::isdigit(static_cast<unsigned char>(c)))
                {
                    scan_number();
                }
                else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
                {
                    scan_identifier();
                }
                else
                {
                    Error::error(line, "Unexpected character.");
                }
                break;
            }
        }

        void scan_indent()
        {
            int spaces = 0;

            // Count spaces or tabs at the start of the line
            while (!is_at_end() && (buffer[current] == ' ' || buffer[current] == '\t'))
            {
                if (buffer[current] == ' ')
                {
                    spaces++;
                }
                else if (buffer[current] == '\t')
                {
                    spaces += 4; // Assuming a tab equals 4 spaces
                }
                advance();
            }

            // Compare the current indentation level with the previous one
            if (spaces > current_indent)
            {
                add_token(TOKEN_INDENT);
                indent_stack.push_back(spaces);
            }
            else if (spaces < current_indent)
            {
                while (!indent_stack.empty() && spaces < indent_stack.back())
                {
                    add_token(TOKEN_DEDENT);
                    indent_stack.pop_back();
                }
            }

            current_indent = spaces;
            SCANNER_DEBUG_LOG("Indentation scanned: " << spaces);
        }

        void scan_number()
        {
            int number_start = start;
            while (std::isdigit(static_cast<unsigned char>(peek())))
                advance();
            if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next())))
            {
                advance(); // '.'
                while (std::isdigit(static_cast<unsigned char>(peek())))
                    advance();
                goto SCANNED_FLOAT;
            }

        SCANNED_INTEGER:
            try {
                std::string num_str = buffer.substr(number_start, current - number_start);
                long long val = std::stoll(num_str);
                if (val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
                    throw std::out_of_range("Integer literal out of range");
                add_token<int>(TOKEN_INTEGER, static_cast<int>(val));
            } catch (const std::exception& e) {
                Error::error(line, "Invalid integer literal.");
            }
            return;

        SCANNED_FLOAT:
            try {
                std::string num_str = buffer.substr(number_start, current - number_start);
                float val = std::stof(num_str);
                add_token<float>(TOKEN_FLOAT, val);
            } catch (const std::exception& e) {
                Error::error(line, "Invalid float literal.");
            }
            return;
        }

        void scan_string()
        {
            while (peek() != '"' && !is_at_end())
            {
                if (peek() == '\n')
                {
                    line++;
                    column = 1;
                }
                advance();
            }
            if (is_at_end())
            {
                Error::error(line, "Unterminated string.");
                return;
            }
            advance(); // Consume the closing '"'
            std::string value = buffer.substr(start + 1, current - start - 2); // Exclude quotes
            add_token<std::string>(TOKEN_STRING, value);
        }

        void scan_identifier()
        {
            while (std::isalnum(static_cast<unsigned char>(peek())))
                advance();
            std::string text = buffer.substr(start, current - start);
            auto it = Tok_keywords.find(text);
            TokenType type = (it != Tok_keywords.end()) ? it->second : TOKEN_IDENTIFIER;
            add_token(type);
        }

    }; // End of Scanner class

}; // End of SereLexer namespace

#endif // SCANNER_HPP
