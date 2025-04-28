#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "TokenType.hpp"

#include <vector>
#include <variant>
#include <memory>
#include <any>

namespace SereLexer {

    class TokenBase {
        public:
            const TokenType type;
            const std::string lexeme;
            const int column;
            const int line;

            TokenBase(TokenType type, const std::string& lexeme, int line, int col)
                : type(type), lexeme(lexeme), line(line), column(col) {}

            virtual ~TokenBase() = default;
    };

    template <typename T>
    class Token : public TokenBase {
        public:
            const T value;

            Token(TokenType type, const std::string& lexeme, T value, int line, int col)
                : TokenBase(type, lexeme, line, col), value(value) {}

            Token(TokenType type, const std::string& lexeme, int line, int col)
                : TokenBase(type, lexeme, line, col), value(T()) {}
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