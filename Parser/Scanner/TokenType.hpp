#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include <map>
#include <cassert>

// Debug macro: Enable debug prints if SERE_DEBUG is defined
#ifdef SERE_DEBUG
    #include <iostream>
    #define SERE_DEBUG_PRINT(x) std::cerr << "[SERE DEBUG] " << x << std::endl
#else
    #define SERE_DEBUG_PRINT(x) do {} while(0)
#endif

// Static assert for enum size stability
#define SERE_STATIC_ASSERT_ENUM_SIZE(expected) \
    static_assert(static_cast<int>(SereLexer::TOKEN_EOF) == (expected), \
    "TokenType enum size mismatch. Update expected value if you add/remove tokens.")

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
        TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_INTEGER, TOKEN_FLOAT,

        // Keywords
        TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
        TOKEN_FOR, TOKEN_DEF, TOKEN_IF, TOKEN_NONE,
        TOKEN_OR, TOKEN_RETURN,
        TOKEN_SUPER, TOKEN_SELF, TOKEN_TRUE, TOKEN_WHILE,
        TOKEN_NOT, TOKEN_ELIF,

        TOKEN_INDENT, // 39
        TOKEN_DEDENT, // 40
        TOKEN_NEWLINE, // 41

        // End of file
        TOKEN_EOF // 42
    };

    SERE_STATIC_ASSERT_ENUM_SIZE(42);

}

#endif // TOKEN_TYPE_HPP