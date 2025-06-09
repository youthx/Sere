#include <iostream>
#include "lexer/core/lexer.hpp"
int main()
{
    auto lexer = sere::lexer::instance(std::string("hello").c_str());
    auto tokstream = lexer.compile_token_stream().get_tokens();
    int i = 0;
    std::cout << tokstream[1].get()->lexeme << std::endl;
    return 0;
}
