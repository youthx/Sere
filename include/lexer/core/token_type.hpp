#ifndef SERE_TOKEN_TYPE_HPP
#define SERE_TOKEN_TYPE_HPP

#pragma once 

#include <cassert>

#ifdef SERE_DEBUG
    #include <iostream>
    #define SERE_DEBUG_PRINT(x) std::cerr << "[SERE DEBUG] " << x << std::endl
#else
    #define SERE_DEBUG_PRINT(x) do {} while (0)
#endif

namespace sere::lexer {
    enum class TokenType {
        // Single-character tokens
        LeftParen, RightParen, LeftBrace, RightBrace,
        LeftBracket, RightBracket,
        Comma, Dot, Minus, Plus, Semicolon,
        Slash, Star, Percent, Colon,
        Pipe, Ampersand, Caret, Tilde,

        // Compound/special tokens
        Arrow,  // '->'
        Bang, BangEqual,
        Equal, EqualEqual,
        Greater, GreaterEqual,
        Less, LessEqual,
        PlusEqual, MinusEqual, StarEqual, SlashEqual,
        PercentEqual, PipeEqual, AmpersandEqual, CaretEqual,
        ColonEqual,
        DoubleStar, DoubleStarEqual,
        DoubleSlash, DoubleSlashEqual,
        DoublePipe, DoubleAmpersand,
        LeftShift, LeftShiftEqual,
        RightShift, RightShiftEqual,

        // Literals
        Identifier, String, Integer, Float,

        // Keywords
        And, As, Assert, Break, Class, Continue,
        Def, Del, Elif, Else, Except, False,
        Finally, For, From, Global, If, Import,
        In, Is, Lambda, None, Nonlocal, Not,
        Or, Pass, Raise, Return, Self, Super,
        True, Try, While, With, Yield,

        // Structural
        Indent,
        Dedent,
        Newline,

        // EOF
        Eof
    };

    static_assert(static_cast<int>(TokenType::Eof) == 89,
                "TokenType enum size mismatch. Update static_assert if tokens are added/removed.");

} // namespace sere::lexer

#endif // SERE_TOKEN_TYPE_HPP