#ifndef SEREPARSER_AST_HPP
#define SEREPARSER_AST_HPP

#include <memory>
#include <vector>
#include <string>

#include "../Scanner/Token.hpp"

namespace SereParser {

    class SereObject {
        public:
            int INTEGER;
            float FLOAT;
            std::string STRING;
            SereObject() : INTEGER(0), FLOAT(0.0f), STRING("") {}

            SereObject(int integerValue) : INTEGER(integerValue), FLOAT(0.0f), STRING("") {}
            SereObject(float floatValue) : INTEGER(0), FLOAT(floatValue), STRING("") {}
            SereObject(const std::string& stringValue) : INTEGER(0), FLOAT(0.0f), STRING(stringValue) {}
            virtual ~SereObject() = default;
    };

    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        
        //virtual void accept(class ASTVisitor& visitor) = 0;
    }; 

}

#endif // SEREPARSER_AST_HPP