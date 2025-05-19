#ifndef SEREPARSER_AST_HPP
#define SEREPARSER_AST_HPP
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

namespace SereParser {
    enum class SereObjectType {
        INTEGER,
        FLOAT,
        STRING,
        BOOLEAN,
        NONE
    };

    class SereObject {
    public:
        SereObject() noexcept
            : INTEGER(0), FLOAT(0.0f), STRING(), BOOLEAN(false), TYPE(SereObjectType::NONE) {}

        explicit SereObject(int integerValue) noexcept
            : INTEGER(integerValue), FLOAT(0.0f), STRING(), BOOLEAN(false), TYPE(SereObjectType::INTEGER) {}

        explicit SereObject(float floatValue) noexcept
            : INTEGER(0), FLOAT(floatValue), STRING(), BOOLEAN(false), TYPE(SereObjectType::FLOAT) {}

        explicit SereObject(bool boolValue) noexcept
            : INTEGER(0), FLOAT(0.0f), STRING(), BOOLEAN(boolValue), TYPE(SereObjectType::BOOLEAN) {}

        explicit SereObject(std::string stringValue)
            : INTEGER(0), FLOAT(0.0f), STRING(std::move(stringValue)), BOOLEAN(false), TYPE(SereObjectType::STRING) {}

        SereObjectType getType() const noexcept { return TYPE; }
        bool isNone() const noexcept { return TYPE == SereObjectType::NONE; }

        void perform_add(const SereObject& other) {
            if (TYPE == SereObjectType::INTEGER && other.TYPE == SereObjectType::INTEGER) {
                INTEGER += other.INTEGER;
            } else if (TYPE == SereObjectType::FLOAT && other.TYPE == SereObjectType::FLOAT) {
                FLOAT += other.FLOAT;
            } else if (TYPE == SereObjectType::STRING && other.TYPE == SereObjectType::STRING) {
                STRING += other.STRING;
            } else {
                throw std::invalid_argument("Invalid addition operation; Mismatched types.");
            }
        }

        void perform_subtract(const SereObject& other) {
            if (TYPE == SereObjectType::INTEGER && other.TYPE == SereObjectType::INTEGER) {
                INTEGER -= other.INTEGER;
            } else if (TYPE == SereObjectType::FLOAT && other.TYPE == SereObjectType::FLOAT) {
                FLOAT -= other.FLOAT;
            } else {
                throw std::invalid_argument("Invalid subtraction operation; Mismatched types.");
            }
        }

        void perform_multiply(const SereObject& other) {
            if (TYPE == SereObjectType::INTEGER && other.TYPE == SereObjectType::INTEGER) {
                INTEGER *= other.INTEGER;
            } else if (TYPE == SereObjectType::FLOAT && other.TYPE == SereObjectType::FLOAT) {
                FLOAT *= other.FLOAT;
            } else {
                throw std::invalid_argument("Invalid multiplication operation; Mismatched types.");
            }
        }

        void perform_divide(const SereObject& other) {
            if (TYPE == SereObjectType::INTEGER && other.TYPE == SereObjectType::INTEGER) {
                if (other.INTEGER == 0) throw std::domain_error("Division by zero.");
                INTEGER /= other.INTEGER;
            } else if (TYPE == SereObjectType::FLOAT && other.TYPE == SereObjectType::FLOAT) {
                if (other.FLOAT == 0.0f) throw std::domain_error("Division by zero.");
                FLOAT /= other.FLOAT;
            } else {
                throw std::invalid_argument("Invalid division operation; Mismatched types.");
            }
        }

        void perform_negate() {
            if (TYPE == SereObjectType::INTEGER) {
                INTEGER = -INTEGER;
            } else if (TYPE == SereObjectType::FLOAT) {
                FLOAT = -FLOAT;
            } else {
                // throw std::invalid_argument("Invalid negation operation; Type is not numeric.");
                return; // No-op for non-numeric types
            }
        }

        void perform_not() {
            if (TYPE == SereObjectType::BOOLEAN) {
                BOOLEAN = !BOOLEAN;
            } else {
                // throw std::invalid_argument("Invalid NOT operation; Type is not boolean.");
                return; // No-op for non-boolean types
            }
        }

        // Prevent copying/moving if not needed
        SereObject(const SereObject&) = default;
        SereObject& operator=(const SereObject&) = default;
        SereObject(SereObject&&) noexcept = default;
        SereObject& operator=(SereObject&&) noexcept = default;

        ~SereObject() = default;

        // Accessors
        int getInteger() const {
            if (TYPE != SereObjectType::INTEGER) throw std::logic_error("Not an integer type.");
            return INTEGER;
        }

        float getFloat() const {
            if (TYPE != SereObjectType::FLOAT) throw std::logic_error("Not a float type.");
            return FLOAT;
        }

        const std::string& getString() const {
            if (TYPE != SereObjectType::STRING) throw std::logic_error("Not a string type.");
            return STRING;
        }

        bool getBoolean() const {
            if (TYPE != SereObjectType::BOOLEAN) throw std::logic_error("Not a boolean type.");
            return BOOLEAN;
        }

        // String representation
        std::string to_string() const {
            switch (TYPE) {
                case SereObjectType::INTEGER: return std::to_string(INTEGER);
                case SereObjectType::FLOAT: return std::to_string(FLOAT);
                case SereObjectType::STRING: return STRING;
                case SereObjectType::BOOLEAN: return BOOLEAN ? "true" : "false";
                default: return "none";
            }
        }

    private:
        int INTEGER;
        float FLOAT;
        std::string STRING;
        bool BOOLEAN;
        SereObjectType TYPE;
    };

    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        //virtual void accept(class ASTVisitor& visitor) = 0;
    };

}

#endif // SEREPARSER_AST_HPP