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
        TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,
        TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS, TOKEN_SEMICOLON,
        TOKEN_SLASH, TOKEN_STAR, TOKEN_PERCENT, TOKEN_COLON,
        TOKEN_PIPE, TOKEN_AMPERSAND, TOKEN_CARET, TOKEN_TILDE,

        // Compound/special tokens
        TOKEN_ARROW, // '->'
        TOKEN_BANG, TOKEN_BANG_EQUAL,
        TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER, TOKEN_GREATER_EQUAL,
        TOKEN_LESS, TOKEN_LESS_EQUAL,
        TOKEN_PLUS_EQUAL, TOKEN_MINUS_EQUAL, TOKEN_STAR_EQUAL, TOKEN_SLASH_EQUAL,
        TOKEN_PERCENT_EQUAL, TOKEN_PIPE_EQUAL, TOKEN_AMPERSAND_EQUAL, TOKEN_CARET_EQUAL,
        TOKEN_COLON_EQUAL,
        TOKEN_DOUBLE_STAR, TOKEN_DOUBLE_STAR_EQUAL,
        TOKEN_DOUBLE_SLASH, TOKEN_DOUBLE_SLASH_EQUAL,
        TOKEN_DOUBLE_PIPE, TOKEN_DOUBLE_AMPERSAND,
        TOKEN_LEFT_SHIFT, TOKEN_LEFT_SHIFT_EQUAL,
        TOKEN_RIGHT_SHIFT, TOKEN_RIGHT_SHIFT_EQUAL,

        // Literals
        TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_INTEGER, TOKEN_FLOAT,

        // Keywords (add more as needed)
        TOKEN_AND, TOKEN_AS, TOKEN_ASSERT, TOKEN_BREAK, TOKEN_CLASS, TOKEN_CONTINUE,
        TOKEN_DEF, TOKEN_DEL, TOKEN_ELIF, TOKEN_ELSE, TOKEN_EXCEPT, TOKEN_FALSE,
        TOKEN_FINALLY, TOKEN_FOR, TOKEN_FROM, TOKEN_GLOBAL, TOKEN_IF, TOKEN_IMPORT,
        TOKEN_IN, TOKEN_IS, TOKEN_LAMBDA, TOKEN_NONE, TOKEN_NONLOCAL, TOKEN_NOT,
        TOKEN_OR, TOKEN_PASS, TOKEN_RAISE, TOKEN_RETURN, TOKEN_SELF, TOKEN_SUPER,
        TOKEN_TRUE, TOKEN_TRY, TOKEN_WHILE, TOKEN_WITH, TOKEN_YIELD,

        // Indentation and newlines
        TOKEN_INDENT,
        TOKEN_DEDENT,
        TOKEN_NEWLINE,

        // End of file
        TOKEN_EOF
    };

    // Update this value if you add/remove tokens above
    SERE_STATIC_ASSERT_ENUM_SIZE(89);

}

#endif // TOKEN_TYPE_HPP