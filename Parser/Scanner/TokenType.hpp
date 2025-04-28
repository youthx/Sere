#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

namespace SereLexer {
    enum TokenType {
        // Single-character tokens
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS, TOKEN_SEMICOLON,
        TOKEN_SLASH, TOKEN_STAR,

        // One or two character tokens
        TOKEN_BANG, TOKEN_BANG_EQUAL,
        TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER, TOKEN_GREATER_EQUAL,
        TOKEN_LESS, TOKEN_LESS_EQUAL,

        // Literals
        TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

        // Keywords
        TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
        TOKEN_FOR, TOKEN_DEF, TOKEN_IF, TOKEN_NONE,
        TOKEN_OR, TOKEN_RETURN,
        TOKEN_SUPER, TOKEN_SELF, TOKEN_TRUE,

        TOKEN_INDENT,
        TOKEN_DEDENT,
        TOKEN_NEWLINE,
        
        // End of file
        TOKEN_EOF
    };           
}

#endif // TOKEN_TYPE_HPP