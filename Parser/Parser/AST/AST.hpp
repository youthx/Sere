#ifndef SEREPARSER_AST_HPP
#define SEREPARSER_AST_HPP
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>
#include <type_traits>

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

        [[nodiscard]] SereObjectType getType() const noexcept { return TYPE; }
        [[nodiscard]] bool isNone() const noexcept { return TYPE == SereObjectType::NONE; }

        void perform_add(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                if (TYPE == SereObjectType::FLOAT || other.TYPE == SereObjectType::FLOAT) {
                    FLOAT = toFloat() + other.toFloat();
                    TYPE = SereObjectType::FLOAT;
                } else {
                    INTEGER += other.INTEGER;
                }
            } else if (TYPE == SereObjectType::STRING && other.TYPE == SereObjectType::STRING) {
                STRING += other.STRING;
            } else {
                throw std::invalid_argument("Invalid addition operation; Mismatched types.");
            }
        }

        void perform_subtract(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                if (TYPE == SereObjectType::FLOAT || other.TYPE == SereObjectType::FLOAT) {
                    FLOAT = toFloat() - other.toFloat();
                    TYPE = SereObjectType::FLOAT;
                } else {
                    INTEGER -= other.INTEGER;
                }
            } else {
                throw std::invalid_argument("Invalid subtraction operation; Mismatched types.");
            }
        }

        void perform_multiply(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                if (TYPE == SereObjectType::FLOAT || other.TYPE == SereObjectType::FLOAT) {
                    FLOAT = toFloat() * other.toFloat();
                    TYPE = SereObjectType::FLOAT;
                } else {
                    INTEGER *= other.INTEGER;
                }
            } else {
                throw std::invalid_argument("Invalid multiplication operation; Mismatched types.");
            }
        }

        void perform_divide(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                if ((TYPE == SereObjectType::FLOAT || other.TYPE == SereObjectType::FLOAT)) {
                    float divisor = other.toFloat();
                    if (divisor == 0.0f) throw std::domain_error("Division by zero.");
                    FLOAT = toFloat() / divisor;
                    TYPE = SereObjectType::FLOAT;
                } else {
                    if (other.INTEGER == 0) throw std::domain_error("Division by zero.");
                    INTEGER /= other.INTEGER;
                }
            } else {
                throw std::invalid_argument("Invalid division operation; Mismatched types.");
            }
        }

        void perform_negate() noexcept {
            if (TYPE == SereObjectType::INTEGER) {
                INTEGER = -INTEGER;
            } else if (TYPE == SereObjectType::FLOAT) {
                FLOAT = -FLOAT;
            }
        }

        void perform_greater(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                BOOLEAN = toFloat() > other.toFloat();
                TYPE = SereObjectType::BOOLEAN;
            } else {
                throw std::invalid_argument("Invalid greater operation; Mismatched types.");
            }
        }

        void perform_greater_equal(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                BOOLEAN = toFloat() >= other.toFloat();
                TYPE = SereObjectType::BOOLEAN;
            } else {
                throw std::invalid_argument("Invalid greater or equal operation; Mismatched types.");
            }
        }

        void perform_less(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                BOOLEAN = toFloat() < other.toFloat();
                TYPE = SereObjectType::BOOLEAN;
            } else {
                throw std::invalid_argument("Invalid less operation; Mismatched types.");
            }
        }

        void perform_less_equal(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                BOOLEAN = toFloat() <= other.toFloat();
                TYPE = SereObjectType::BOOLEAN;
            } else {
                throw std::invalid_argument("Invalid less or equal operation; Mismatched types.");
            }
        }

        void perform_equalequal(const SereObject& other) {
            if (isNumeric() && other.isNumeric()) {
                BOOLEAN = toFloat() == other.toFloat();
                TYPE = SereObjectType::BOOLEAN;
            } else if (TYPE == SereObjectType::STRING && other.TYPE == SereObjectType::STRING) {
                BOOLEAN = STRING == other.STRING;
                TYPE = SereObjectType::BOOLEAN;
            } else if (TYPE == SereObjectType::BOOLEAN && other.TYPE == SereObjectType::BOOLEAN) {
                BOOLEAN = BOOLEAN == other.BOOLEAN;
                TYPE = SereObjectType::BOOLEAN;
            } else if (TYPE == SereObjectType::NONE && other.TYPE == SereObjectType::NONE) {
                BOOLEAN = true;
                TYPE = SereObjectType::BOOLEAN;
            } else {
                BOOLEAN = false;
                TYPE = SereObjectType::BOOLEAN;
            }
        }

        void perform_notequal(const SereObject& other) {
            perform_equalequal(other);
            BOOLEAN = !BOOLEAN;
        }

        void perform_not() noexcept {
            if (TYPE == SereObjectType::BOOLEAN) {
                BOOLEAN = !BOOLEAN;
            }
        }

        // Prevent copying/moving if not needed
        SereObject(const SereObject&) = default;
        SereObject& operator=(const SereObject&) = default;
        SereObject(SereObject&&) noexcept = default;
        SereObject& operator=(SereObject&&) noexcept = default;

        ~SereObject() = default;

        // Accessors
        [[nodiscard]] int getInteger() const {
            if (TYPE != SereObjectType::INTEGER) throw std::logic_error("Not an integer type.");
            return INTEGER;
        }

        [[nodiscard]] float getFloat() const {
            if (TYPE != SereObjectType::FLOAT) throw std::logic_error("Not a float type.");
            return FLOAT;
        }

        [[nodiscard]] const std::string& getString() const {
            if (TYPE != SereObjectType::STRING) throw std::logic_error("Not a string type.");
            return STRING;
        }

        [[nodiscard]] bool getBoolean() const {
            if (TYPE != SereObjectType::BOOLEAN) throw std::logic_error("Not a boolean type.");
            return BOOLEAN;
        }

        // String representation
        [[nodiscard]] std::string to_string() const {
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

        [[nodiscard]] bool isNumeric() const noexcept {
            return TYPE == SereObjectType::INTEGER || TYPE == SereObjectType::FLOAT;
        }

        [[nodiscard]] float toFloat() const noexcept {
            return TYPE == SereObjectType::FLOAT ? FLOAT : static_cast<float>(INTEGER);
        }
    };

    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        //virtual void accept(class ASTVisitor& visitor) = 0;
    };

}

#endif // SEREPARSER_AST_HPP
