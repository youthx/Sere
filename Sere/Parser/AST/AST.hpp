#ifndef SEREPARSER_AST_HPP
#define SEREPARSER_AST_HPP

#include <string>
#include <stdexcept>
#include <utility>
#include <variant>
#include <optional>
#include <type_traits>
#include <sstream>
#include <memory>
#include <cmath>
namespace SereParser {

    // === Type and Symbol Type Definitions ===

    enum class SereObjectType {
        INTEGER,
        FLOAT,
        STRING,
        BOOLEAN,
        SYMBOLIC,
        NONE
    };

    enum class SereSymbolType {
        VARIABLE_REF,
        FUNCTION_REF,
        CLASS_REF,
        FUNCTION_CALL,
        CLASS_CALL,
        VARIABLE_REASSIGN,
        VARIABLE_ASSIGN,
        FUNCTION_ASSIGN,
        CLASS_ASSIGN,
        NOT_SYMBOLIC
    };

    // === SereObject: Unified Value and Symbolic Representation ===

    class SereObject {
    public:
        // --- Constructors for core types ---
        SereObject() noexcept
            : value_(std::monostate{}), type_(SereObjectType::NONE),
              symbol_type_(SereSymbolType::NOT_SYMBOLIC) {}

        explicit SereObject(int integerValue) noexcept
            : value_(integerValue), type_(SereObjectType::INTEGER),
              symbol_type_(SereSymbolType::NOT_SYMBOLIC) {}

        explicit SereObject(float floatValue) noexcept
            : value_(floatValue), type_(SereObjectType::FLOAT),
              symbol_type_(SereSymbolType::NOT_SYMBOLIC) {}

        explicit SereObject(bool boolValue) noexcept
            : value_(boolValue), type_(SereObjectType::BOOLEAN),
              symbol_type_(SereSymbolType::NOT_SYMBOLIC) {}

        explicit SereObject(const std::string& stringValue)
            : value_(stringValue), type_(SereObjectType::STRING),
              symbol_type_(SereSymbolType::NOT_SYMBOLIC) {}

        // --- Constructors for symbolic values (e.g. names, unresolved) ---
        SereObject(std::string symbol, SereSymbolType symtype)
            : value_(std::monostate{}), type_(SereObjectType::SYMBOLIC),
              symbol_type_(symtype), symbol_name_(std::move(symbol)) {}

        // --- Copy/move/default ---
        SereObject(const SereObject&) = default;
        SereObject& operator=(const SereObject&) = default;
        SereObject(SereObject&&) noexcept = default;
        SereObject& operator=(SereObject&&) noexcept = default;
        ~SereObject() = default;

        // --- Type and Symbol Info ---
        SereObjectType getType() const noexcept { return type_; }
        SereSymbolType getSymbolType() const { return symbol_type_; }
        void setSymbolType(SereSymbolType ty) { symbol_type_ = ty; }
        void setType(SereObjectType ty) { type_ = ty; }

        bool compatibleWith(const SereObject& other) const noexcept {
            return type_ == other.type_;
        }
        bool isSymbolic() const noexcept { return symbol_type_ != SereSymbolType::NOT_SYMBOLIC; }
        bool isNone() const noexcept { return type_ == SereObjectType::NONE; }
        const std::string& getSymbolName() const {
            if (!isSymbolic())
                throw std::logic_error("Not a symbolic type.");
            return symbol_name_;
        }

        // --- Operators for Preprocessing & Compilation ---
        void perform_add(const SereObject& other) {
            if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::INTEGER) {
                setValue(getInteger() + other.getInteger());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::FLOAT) {
                setValue(getFloat() + other.getFloat());
            } else if (type_ == SereObjectType::STRING && other.type_ == SereObjectType::STRING) {
                setValue(getString() + other.getString());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::INTEGER) {
                setValue(getFloat() + other.getInteger());
            } else if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::FLOAT) {
                setValue(getInteger() + other.getFloat());
            } else {
                throw std::invalid_argument("Invalid addition operation; Mismatched types.");
            }
        }

        void perform_subtract(const SereObject& other) {
            if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::INTEGER) {
                setValue(getInteger() - other.getInteger());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::FLOAT) {
                setValue(getFloat() - other.getFloat());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::INTEGER) {
                setValue(getFloat() - other.getInteger());
            } else if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::FLOAT) {
                setValue(getInteger() - other.getFloat());
            } else {
                throw std::invalid_argument("Invalid subtraction operation; Mismatched types.");
            }
        }

        void perform_multiply(const SereObject& other) {
            if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::INTEGER) {
                setValue(getInteger() * other.getInteger());
            } else if (type_ == SereObjectType::STRING && other.type_ == SereObjectType::INTEGER) {
                if (other.getInteger() < 0)
                    throw std::invalid_argument("Cannot multiply string by negative integer.");
                std::string result;
                for (int i = 0; i < other.getInteger(); ++i) result += getString();
                setValue(result);
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::FLOAT) {
                setValue(getFloat() * other.getFloat());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::INTEGER) {
                setValue(getFloat() * other.getInteger());
            } else if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::FLOAT) {
                setValue(getInteger() * other.getFloat());
            } else {
                throw std::invalid_argument("Invalid multiplication operation; Mismatched types.");
            }
        }

        void perform_divide(const SereObject& other) {
            if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::INTEGER) {
                if (other.getInteger() == 0) throw std::domain_error("Division by zero.");
                setValue(getInteger() / other.getInteger());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::FLOAT) {
                if (std::abs(other.getFloat()) < 1e-7) throw std::domain_error("Division by zero.");
                setValue(getFloat() / other.getFloat());
            } else if (type_ == SereObjectType::FLOAT && other.type_ == SereObjectType::INTEGER) {
                setValue(getFloat() / other.getInteger());
            } else if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::FLOAT) {
                setValue(getInteger() / other.getFloat());
            } else {
                throw std::invalid_argument("Invalid division operation; Mismatched types.");
            }
        }

        void perform_negate() {
            if (type_ == SereObjectType::INTEGER) setValue(-getInteger());
            else if (type_ == SereObjectType::FLOAT) setValue(-getFloat());
            // No-op for non-numeric types
        }

        void perform_not() {
            if (type_ == SereObjectType::BOOLEAN) setValue(!getBoolean());
            // No-op for non-boolean types
        }

        // --- Preprocessing helpers ---
        bool isConstant() const noexcept {
            // Not symbolic, not NONE
            return !isSymbolic() && type_ != SereObjectType::NONE;
        }

        // --- Value Accessors ---
        int getInteger() const {
            if (type_ != SereObjectType::INTEGER)
                throw std::logic_error("Not an integer type.");
            return std::get<int>(value_);
        }
        float getFloat() const {
            if (type_ != SereObjectType::FLOAT)
                throw std::logic_error("Not a float type.");
            return std::get<float>(value_);
        }
        const std::string& getString() const {
            if (type_ != SereObjectType::STRING)
                throw std::logic_error("Not a string type.");
            return std::get<std::string>(value_);
        }
        bool getBoolean() const {
            if (type_ != SereObjectType::BOOLEAN)
                throw std::logic_error("Not a boolean type.");
            return std::get<bool>(value_);
        }

        // --- Value Setters (internal/private use) ---
        void setValue(int v)    { value_ = v; type_ = SereObjectType::INTEGER; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; }
        void setValue(float v)  { value_ = v; type_ = SereObjectType::FLOAT; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; }
        void setValue(bool v)   { value_ = v; type_ = SereObjectType::BOOLEAN; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; }
        void setValue(const std::string& v) { value_ = v; type_ = SereObjectType::STRING; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; }
        void setValue(std::string&& v) { value_ = std::move(v); type_ = SereObjectType::STRING; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; }

        // --- String Representation ---
        std::string to_string() const {
            switch (type_) {
                case SereObjectType::INTEGER: return std::to_string(getInteger());
                case SereObjectType::FLOAT:   return std::to_string(getFloat());
                case SereObjectType::STRING:  return getString();
                case SereObjectType::BOOLEAN: return getBoolean() ? "true" : "false";
                case SereObjectType::SYMBOLIC: return "<symbol:" + symbol_name_ + ">";
                default: return "none";
            }
        }

        // --- Comparison (for preprocessing, folding, etc) ---
        bool operator==(const SereObject& other) const {
            if (type_ != other.type_) return false;
            switch (type_) {
                case SereObjectType::INTEGER: return getInteger() == other.getInteger();
                case SereObjectType::FLOAT:   return std::abs(getFloat() - other.getFloat()) < 1e-7;
                case SereObjectType::STRING:  return getString() == other.getString();
                case SereObjectType::BOOLEAN: return getBoolean() == other.getBoolean();
                case SereObjectType::NONE:    return true;
                case SereObjectType::SYMBOLIC: return symbol_type_ == other.symbol_type_ && symbol_name_ == other.symbol_name_;
                default: return false;
            }
        }

        bool operator!=(const SereObject& other) const { return !(*this == other); }

        // --- Additional: Preprocessor/Compiler metadata ---
        // For future: add source location, AST reference, or IR ref as needed.

    private:
        // Value is stored as a variant for type safety
        std::variant<std::monostate, int, float, std::string, bool> value_;
        SereObjectType type_;
        SereSymbolType symbol_type_;
        std::string symbol_name_; // Only set for SYMBOLIC types
    };

    // === AST Node Base ===
    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        // Extend with accept(visitor) if needed.
    };

} // namespace SereParser

#endif // SEREPARSER_AST_HPP