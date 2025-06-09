#ifndef SERE_TOKEN_HPP
#define SERE_TOKEN_HPP

#include "lexer/core/token_type.hpp"

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <cassert>

namespace sere::lexer
{
#ifndef NDEBUG
#define SERE_ASSERT(expr) assert(expr)
#else
#define SERE_ASSERT(expr) ((void)0)
#endif

    using TokenLiteral = std::variant<std::monostate, int, float, std::string>;

    class Token
    {
    public:
        TokenType type;
        std::string lexeme;
        TokenLiteral literal;
        int line;
        int column;

        Token();

        Token(TokenType type, std::string lexeme, TokenLiteral literal,
              int line, int column);

        virtual ~Token() = default;
    };

    class TokenStream
    {
    public:
        TokenStream() = default;
        ~TokenStream() = default;

        void clear();

        void add_token(const Token &token);

        const std::vector<std::shared_ptr<Token>> &get_tokens() const;

    private:
        std::vector<std::shared_ptr<Token>> tokens_;
    };
}

#endif // SERE_TOKEN_HPP
