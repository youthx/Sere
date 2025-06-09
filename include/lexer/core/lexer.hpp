#ifndef SERE_LEXER_HPP
#define SERE_LEXER_HPP

#pragma once

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
#include "token.hpp"

// Debug macro: Enable debug output if needed
#ifdef SCANNER_DEBUG
#include <iostream>
#define SCANNER_DEBUG_LOG(msg) std::cerr << "[Scanner Debug] " << msg << std::endl
#else
#define SCANNER_DEBUG_LOG(msg) ((void)0)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SCANNER_LIKELY(x) __builtin_expect(!!(x), 1)
#define SCANNER_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SCANNER_LIKELY(x) (x)
#define SCANNER_UNLIKELY(x) (x)
#endif

namespace sere::lexer
{

    class LexerCore
    {
    public:
        LexerCore() = delete;

        LexerCore(const char *input_buffer);

        TokenStream compile_token_stream();

    private:
        std::string buffer;
        TokenStream token_stream;

        int line, column, current, start;
        bool at_line_start;
        int current_indent;
        int paren_level;
        std::vector<int> indent_stack;

        const std::map<std::string, TokenType> reserved_toks{
            {"and", TokenType::And},
            {"as", TokenType::As},
            {"assert", TokenType::Assert},
            {"break", TokenType::Break},
            {"class", TokenType::Class},
            {"continue", TokenType::Continue},
            {"def", TokenType::Def},
            {"del", TokenType::Del},
            {"elif", TokenType::Elif},
            {"else", TokenType::Else},
            {"except", TokenType::Except},
            {"False", TokenType::False},
            {"finally", TokenType::Finally},
            {"for", TokenType::For},
            {"from", TokenType::From},
            {"global", TokenType::Global},
            {"if", TokenType::If},
            {"import", TokenType::Import},
            {"in", TokenType::In},
            {"is", TokenType::Is},
            {"lambda", TokenType::Lambda},
            {"None", TokenType::None},
            {"nonlocal", TokenType::Nonlocal},
            {"not", TokenType::Not},
            {"or", TokenType::Or},
            {"pass", TokenType::Pass},
            {"raise", TokenType::Raise},
            {"return", TokenType::Return},
            {"self", TokenType::Self},
            {"super", TokenType::Super},
            {"True", TokenType::True},
            {"try", TokenType::Try},
            {"while", TokenType::While},
            {"with", TokenType::With},
            {"yield", TokenType::Yield}};

        void reset_position();

        bool is_at_end() const noexcept;

        char advance();
        char peek() const noexcept;
        char peek_next() const noexcept;

        bool match(char expected_char);

        void add_token(TokenType type);

        template <typename T>
        void add_token(TokenType type, T literal);

        void scan_next();
        void scan_indent();

        void scan_number(bool float_start_dot = false, char first_char = '\0');
        void scan_string(char quote_type);
        void scan_identifier(char first_char);
    };

    inline LexerCore instance(const char *stream) {
        return LexerCore(stream);
    };
}

#endif // SERE_LEXER_HPP
