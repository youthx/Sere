
#include "lexer/core/token.hpp"
#include <vector>

namespace sere::lexer
{

	Token::Token()
		: type(TokenType::Eof),
		  lexeme(""),
		  literal(std::monostate{}),
		  line(0),
		  column(0) {}

	Token::Token(TokenType type,
				 std::string lexeme,
				 TokenLiteral literal,
				 int line,
				 int column)
		: type(type),
		  lexeme(std::move(lexeme)),
		  literal(std::move(literal)),
		  line(line),
		  column(column) {}

	void TokenStream::clear()
	{
		tokens_.clear();
	}

	void TokenStream::add_token(const Token &token)
	{
		tokens_.emplace_back(std::make_shared<Token>(token));
	}

	const std::vector<std::shared_ptr<Token>> &TokenStream::get_tokens() const
	{
		return tokens_;
	}
}
