#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <stdexcept>
#include "Token.hpp"
#include "../errors.hpp"

namespace SereLexer
{

class Scanner
{
public:
    Scanner() = delete;

    explicit Scanner(const char *input_buffer)
        : buffer(input_buffer ? input_buffer : ""),
          line(1), column(1), current(0), start(0),
          current_indent(0)
    {}

    [[nodiscard]]
    TokenList tokenize()
    {
        reset_position();
        while (!is_at_end())
        {
            start = current;
            scan_one();
        }
        token_list.add_token<int>(Token<int>(TOKEN_EOF, "", line, column));
        return std::move(token_list);
    }

private:
    std::string buffer;
    TokenList token_list;

    int line;
    int column;

    std::size_t current;
    std::size_t start;

    int current_indent;
    std::vector<int> indent_stack;

    const std::map<std::string, TokenType> Tok_keywords{
        {"and", TOKEN_AND},
        {"class", TOKEN_CLASS},
        {"def", TOKEN_DEF},
        {"elif", TOKEN_ELIF},
        {"else", TOKEN_ELSE},
        {"False", TOKEN_FALSE},
        {"for", TOKEN_FOR},
        {"if", TOKEN_IF},
        {"None", TOKEN_NONE},
        {"not", TOKEN_NOT},
        {"or", TOKEN_OR},
        {"return", TOKEN_RETURN},
        {"self", TOKEN_SELF},
        {"super", TOKEN_SUPER},
        {"True", TOKEN_TRUE},
        {"while", TOKEN_WHILE}
    };

    void reset_position()
    {
        line = 1;
        column = 1;
        current = 0;
        start = 0;
        current_indent = 0;
        indent_stack.clear();
        token_list.clear();
    }

    [[nodiscard]]
    inline bool is_at_end() const noexcept
    {
        return current >= buffer.length();
    }

    char advance()
    {
        if (is_at_end())
            return '\0';
        char c = buffer[current++];
        if (c == '\n')
        {
            ++line;
            column = 1;
        }
        else
        {
            ++column;
        }
        return c;
    }

    [[nodiscard]]
    char peek() const noexcept
    {
        return is_at_end() ? '\0' : buffer[current];
    }

    [[nodiscard]]
    char peek_next() const noexcept
    {
        return (current + 1 >= buffer.length()) ? '\0' : buffer[current + 1];
    }

    bool match(char expected_char)
    {
        if (is_at_end() || buffer[current] != expected_char)
            return false;
        ++current;
        ++column;
        return true;
    }

    void add_token(TokenType type)
    {
        std::string text = buffer.substr(start, current - start);
        token_list.add_token<int>(Token<int>(type, text, line, column));
    }

    template <typename T>
    void add_token(TokenType type, T literal)
    {
        std::string text = buffer.substr(start, current - start);
        token_list.add_token<T>(Token<T>(type, text, literal, line, column));
    }

    void scan_one()
    {
        char c = advance();
        switch (c)
        {
        case '\0': break;
        case ' ':
        case '\r':
        case '\t':
        case '\n':
            if (column == 1)
                scan_indent();
            break;
        case '#':
            while (peek() != '\n' && !is_at_end())
                advance();
            break;
        case '(': add_token(TOKEN_LEFT_PAREN); break;
        case ')': add_token(TOKEN_RIGHT_PAREN); break;
        case '{': add_token(TOKEN_LEFT_BRACE); break;
        case '}': add_token(TOKEN_RIGHT_BRACE); break;
        case ',': add_token(TOKEN_COMMA); break;
        case '.':
            if (std::isdigit(static_cast<unsigned char>(peek())))
            {
                --current; // step back to include '.'
                --column;
                scan_number(true); // float starting with '.'
            }
            else
            {
                add_token(TOKEN_DOT);
            }
            break;
        case '-': add_token(TOKEN_MINUS); break;
        case '+': add_token(TOKEN_PLUS); break;
        case ';': add_token(TOKEN_SEMICOLON); break;
        case '*': add_token(TOKEN_STAR); break;
        case '/': add_token(TOKEN_SLASH); break;
        default:
            scan_two(c);
            break;
        }
    }

    void scan_two(char c)
    {
        switch (c)
        {
        case '!': add_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG); break;
        case '=': add_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL); break;
        case '<': add_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS); break;
        case '>': add_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER); break;
        case '"': scan_string(); break;
        default:
            if (std::isdigit(static_cast<unsigned char>(c)))
            {
                --current; --column; // step back to include first digit
                scan_number(false);
            }
            else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
            {
                --current; --column;
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
        std::size_t pos = current;
        while (pos < buffer.size() && (buffer[pos] == ' ' || buffer[pos] == '\t'))
        {
            if (buffer[pos] == ' ')
                ++spaces;
            else if (buffer[pos] == '\t')
                spaces += 4;
            ++pos;
        }
        current = pos;
        column = spaces + 1;

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
    }

    void scan_number(bool leading_dot)
    {
        std::size_t num_start = leading_dot ? current : start;
        if (leading_dot)
        {
            advance(); // consume '.'
            while (std::isdigit(static_cast<unsigned char>(peek())))
                advance();
            std::string num_str = buffer.substr(num_start, current - num_start);
            try
            {
                add_token<float>(TOKEN_FLOAT, std::stof(std::string(num_str)));
            }
            catch (const std::exception&)
            {
                Error::error(line, "Invalid float literal.");
            }
            return;
        }

        while (std::isdigit(static_cast<unsigned char>(peek())))
            advance();

        if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next())))
        {
            advance(); // consume '.'
            while (std::isdigit(static_cast<unsigned char>(peek())))
                advance();
            std::string num_str = buffer.substr(start, current - start);
            try
            {
                add_token<float>(TOKEN_FLOAT, std::stof(std::string(num_str)));
            }
            catch (const std::exception&)
            {
                Error::error(line, "Invalid float literal.");
            }
        }
        else
        {
            std::string num_str = buffer.substr(start, current - start);
            try
            {
                add_token<int>(TOKEN_INTEGER, std::stoi(std::string(num_str)));
            }
            catch (const std::exception&)
            {
                Error::error(line, "Invalid integer literal.");
            }
        }
    }

    void scan_string()
    {
        while (peek() != '"' && !is_at_end())
        {
            if (peek() == '\n')
            {
                ++line;
                column = 1;
            }
            advance();
        }
        if (is_at_end())
        {
            Error::error(line, "Unterminated string.");
            return;
        }
        advance(); // closing '"'
        std::string value = buffer.substr(start + 1, current - start - 2);
        add_token<std::string>(TOKEN_STRING, value);
    }

    void scan_identifier()
    {
        while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')
            advance();
        std::string text = buffer.substr(start, current - start);
        auto it = Tok_keywords.find(text);
        TokenType type = (it != Tok_keywords.end()) ? it->second : TOKEN_IDENTIFIER;
        add_token(type);
    }
};

} // namespace SereLexer

#endif // SCANNER_HPP
