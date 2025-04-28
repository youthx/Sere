#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "errors.hpp"

std::vector<unsigned char> sere_read_file(const char* filepath) {
    if (filepath == nullptr) {
        throw std::invalid_argument("Filepath is null");
    }

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("File not found or could not be opened");
    }

    std::streamsize size = file.tellg();
    if (size < 0) {
        throw std::runtime_error("Failed to determine file size");
    }
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("File read error");
    }

    return buffer;
}

int main(int argc, char* argv[]) {

    try {
        if (argc < 2) {
            std::cerr << "Usage: \n\t" << argv[0] << " <input_file>" << std::endl;
            return 64;
        }

        const char* filepath = argv[1];
        if (filepath == nullptr || filepath[0] == '\0') {
            std::cerr << "Invalid file path provided" << std::endl;
            return 65;
        }

        std::vector<unsigned char> buffer = sere_read_file(filepath);

        // Uncomment and implement the following lines if needed
        // SereLexer::Lexer lexer(buffer);
        // SereToken::TokenList tokens = lexer.tokenize();

        // for (const auto& token : tokens) {
        //     std::cout << "Token: " << token.type << ", Value: " << token.value << std::endl;
        // }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred" << std::endl;
        return 2;
    }

    return 0;
}