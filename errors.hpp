#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <iostream>

namespace Error {

    void report(int line, const std::string& where, const std::string& msg) {
        std::cerr << "[line " << line << " ] Error" << where << ": " << msg << std::endl;
    }

    void error(int line, const std::string& message) {
        report(line, "", message);
    }
}


#endif // ERRORS_HPP