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
            bool BOOLEAN;

            bool isNone () const {
                return INTEGER == 0 && FLOAT == 0.0f && STRING.empty() && !BOOLEAN;
            }
            
            SereObject() : INTEGER(0), FLOAT(0.0f), STRING(""), BOOLEAN(false) {}

            SereObject(int integerValue) : INTEGER(integerValue), FLOAT(0.0f), STRING(""), BOOLEAN(false) {}
            SereObject(float floatValue) : INTEGER(0), FLOAT(floatValue), STRING(""), BOOLEAN(false) {}
            SereObject(bool boolValue) : INTEGER(0), FLOAT(0.0f), STRING(""), BOOLEAN(boolValue) {}
            SereObject(const std::string& stringValue) : INTEGER(0), FLOAT(0.0f), STRING(stringValue), BOOLEAN(false) {}
            
            virtual ~SereObject() = default;
    };

    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        
        //virtual void accept(class ASTVisitor& visitor) = 0;
    }; 

}

#endif // SEREPARSER_AST_HPP