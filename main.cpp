#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string> 
#include <cstring>
#include <assert.h>

#include "errors.hpp"
#include "./Parser/Scanner/Token.hpp"
#include "./Parser/Scanner/Scanner.hpp"
#include "./Parser/Parser/Parser.hpp"
#include "./Parser/Parser/AST/Visitor.hpp"
#include "./Parser/Parser/AST/AST.hpp"

std::vector<unsigned char> sere_read_file(const char *filepath)
{
    if (filepath == nullptr)
    {
        throw std::invalid_argument("Filepath is null");
    }

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw std::runtime_error("File not found or could not be opened");
    }

    std::streamsize size = file.tellg();
    if (size < 0)
    {
        throw std::runtime_error("Failed to determine file size");
    }
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        throw std::runtime_error("File read error");
    }

    return buffer;
}

int main(int argc, char *argv[])
{

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: \n\t" << argv[0] << " <input_file>" << std::endl;
            return 64;
        }
        


        const char *filepath = argv[1];
        if (filepath == nullptr || filepath[0] == '\0')
        {
            std::cerr << "Invalid file path provided" << std::endl;
            return 65;
        }

        std::vector<unsigned char> buffer = sere_read_file(filepath);
        SereLexer::Scanner scanner(reinterpret_cast<const char *>(buffer.data()));
        SereLexer::TokenList tokens = scanner.tokenize();
        if (tokens.getTokens()[0]->type == SereLexer::TOKEN_EOF)
        {
            std::cout << "File is empty or contains only skipped tokens." << std::endl;
            return 0;
        }
        SereParser::Parser parser(tokens);
        auto expr = parser.parse();
        if (expr)
        {
            
            
            std::cout << "Parsed expression successfully." << std::endl;
            std::shared_ptr<SereParser::ExprVisitor<SereParser::SereObject>> visitor = std::make_shared<SereParser::ExprVisitor<SereParser::SereObject>>();
            SereParser::SereObject result = visitor.get()->accept_expression(*expr.get());
    
            std::cout << "Result: " << result.to_string() << std::endl;
        }
        else
        {
            std::cerr << "Failed to parse expression." << std::endl;
            return 66;
        }
        
        /*
        for (const auto &token : tokens.getTokens())
        {
            std::cout << "Token: " << token->type << ", Lexeme: " << token->lexeme << std::endl;
        }

        */
        // Uncomment and implement the following lines if needed
        // SereLexer::Lexer lexer(buffer);
        // SereToken::TokenList tokens = lexer.tokenize();

        // for (const auto& token : tokens) {
        //     std::cout << "Token: " << token.type << ", Value: " << token.value << std::endl;
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred" << std::endl;
        return 2;
    }

    return 0;
}