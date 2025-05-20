#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "TokenType.hpp"

#include <vector>
#include <variant>
#include <memory>
#include <any>

namespace SereLexer {
    
    // Enable debug assertions if not in release mode
    #ifndef NDEBUG
    #define SERE_ASSERT(expr) assert(expr)
    #else
    #define SERE_ASSERT(expr) ((void)0)
    #endif

    class TokenValue {
        public:

            TokenValue() = default;
            TokenValue(int value) : INTEGER(value), FLOAT(0), STRING("") {}
            TokenValue(float value) : FLOAT(value), INTEGER(0), STRING("") {}
            TokenValue(const std::string& value) : STRING(value), INTEGER(0), FLOAT(0) {}
        
            const int INTEGER;
            const float FLOAT;
            const std::string STRING;
    };

    class TokenBase {
        public:
            const TokenType type;
            const std::string lexeme;
            const int column;
            const int line;

            const TokenValue literal;

            TokenBase() = default;
            TokenBase(TokenType type, const std::string& lexeme, const TokenValue& literal, int line, int col)
                : type(type), lexeme(lexeme), literal(literal), line(line), column(col) {}

            virtual ~TokenBase() = default;
    };

    template <typename T>
    class Token : public TokenBase {
        public:
            Token(TokenType type, const std::string& lexeme, T literal, int line, int col)
                : TokenBase(type, lexeme, TokenValue(literal), line, col) {}

            Token(TokenType type, const std::string& lexeme, int line, int col)
                : TokenBase(type, lexeme, TokenValue(0), line, col) {}            
    };

    class TokenList {
        public:
            TokenList() = default;
            ~TokenList() = default;

            template <typename T>
            void add_token(const Token<T>& token) {
                tokens.emplace_back(std::make_shared<Token<T>>(token));
            }

            const std::vector<std::shared_ptr<TokenBase>>& getTokens() const {
                return tokens;
            }

            void clear() {
                tokens.clear();
            }

        private:
            std::vector<std::shared_ptr<TokenBase>> tokens;
    };
}

#endif // TOKEN_HPP