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
#include <ostream>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>


namespace SereParser {

// === Type and Symbol Type Definitions ===

enum class SereObjectType {
    INTEGER,
    FLOAT,
    STRING,
    BOOLEAN,
    DYNAMIC,
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
          symbol_type_(SereSymbolType::NOT_SYMBOLIC), llvm_value_(nullptr), llvm_dirty_(true) {}

    explicit SereObject(int integerValue) noexcept
        : value_(integerValue), type_(SereObjectType::INTEGER),
          symbol_type_(SereSymbolType::NOT_SYMBOLIC), llvm_value_(nullptr), llvm_dirty_(true) {}

    explicit SereObject(float floatValue) noexcept
        : value_(floatValue), type_(SereObjectType::FLOAT),
          symbol_type_(SereSymbolType::NOT_SYMBOLIC), llvm_value_(nullptr), llvm_dirty_(true) {}

    explicit SereObject(bool boolValue) noexcept
        : value_(boolValue), type_(SereObjectType::BOOLEAN),
          symbol_type_(SereSymbolType::NOT_SYMBOLIC), llvm_value_(nullptr), llvm_dirty_(true) {}

    explicit SereObject(const std::string& stringValue)
        : value_(stringValue), type_(SereObjectType::STRING),
          symbol_type_(SereSymbolType::NOT_SYMBOLIC), llvm_value_(nullptr), llvm_dirty_(true) {}

    // --- Constructors for symbolic values (e.g. names, unresolved) ---
    SereObject(std::string symbol, SereSymbolType symtype)
        : value_(std::monostate{}), type_(SereObjectType::SYMBOLIC),
          symbol_type_(symtype), symbol_name_(std::move(symbol)), llvm_value_(nullptr), llvm_dirty_(true) {}

    // --- Copy/move/default ---
    SereObject(const SereObject& other)
        : value_(other.value_), type_(other.type_), symbol_type_(other.symbol_type_),
          symbol_name_(other.symbol_name_), llvm_value_(nullptr), llvm_dirty_(true) {}

    SereObject& operator=(const SereObject& other) {
        if (this != &other) {
            value_ = other.value_;
            type_ = other.type_;
            symbol_type_ = other.symbol_type_;
            symbol_name_ = other.symbol_name_;
            llvm_value_ = nullptr;
            llvm_dirty_ = true;
        }
        return *this;
    }

    SereObject(SereObject&& other) noexcept
        : value_(std::move(other.value_)), type_(other.type_), symbol_type_(other.symbol_type_),
          symbol_name_(std::move(other.symbol_name_)), llvm_value_(other.llvm_value_), llvm_dirty_(other.llvm_dirty_) {
        other.llvm_value_ = nullptr;
        other.llvm_dirty_ = true;
    }

    SereObject& operator=(SereObject&& other) noexcept {
        if (this != &other) {
            value_ = std::move(other.value_);
            type_ = other.type_;
            symbol_type_ = other.symbol_type_;
            symbol_name_ = std::move(other.symbol_name_);
            llvm_value_ = other.llvm_value_;
            llvm_dirty_ = other.llvm_dirty_;
            other.llvm_value_ = nullptr;
            other.llvm_dirty_ = true;
        }
        return *this;
    }

    virtual ~SereObject() = default;

    // --- Type and Symbol Info ---
    SereObjectType getType() const noexcept { return type_; }
    SereSymbolType getSymbolType() const noexcept { return symbol_type_; }
    void setSymbolType(SereSymbolType ty) noexcept { symbol_type_ = ty; }
    void setType(SereObjectType ty) noexcept { type_ = ty; }

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

    // --- LLVM Value* Support ---
    // Ensures the LLVM value is up to date with the current SereObject value.
    llvm::Value* getLLVMValue(llvm::LLVMContext* context = nullptr) {
        if (!llvm_dirty_ && llvm_value_) return llvm_value_;
        if (!context) return llvm_value_; // Can't update without context

        switch (type_) {
            case SereObjectType::INTEGER:
                llvm_value_ = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), getInteger(), true);
                break;
            case SereObjectType::FLOAT:
                llvm_value_ = llvm::ConstantFP::get(llvm::Type::getFloatTy(*context), getFloat());
                break;
            case SereObjectType::BOOLEAN:
                llvm_value_ = llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), getBoolean());
                break;
            case SereObjectType::STRING:
                // Strings require special handling in LLVM IR; left as nullptr for now.
                llvm_value_ = nullptr;
                break;
            default:
                llvm_value_ = nullptr;
                break;
        }
        llvm_dirty_ = false;
        return llvm_value_;
    }
    void setLLVMValue(llvm::Value* v) noexcept { llvm_value_ = v; llvm_dirty_ = false; }

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
            setValue(std::move(result));
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
            if (other.getInteger() == 0) throw std::domain_error("Division by zero.");
            setValue(getFloat() / other.getInteger());
        } else if (type_ == SereObjectType::INTEGER && other.type_ == SereObjectType::FLOAT) {
            if (std::abs(other.getFloat()) < 1e-7) throw std::domain_error("Division by zero.");
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
    void setValue(int v)    { value_ = v; type_ = SereObjectType::INTEGER; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; markLLVMDirty(); }
    void setValue(float v)  { value_ = v; type_ = SereObjectType::FLOAT; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; markLLVMDirty(); }
    void setValue(bool v)   { value_ = v; type_ = SereObjectType::BOOLEAN; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; markLLVMDirty(); }
    void setValue(const std::string& v) { value_ = v; type_ = SereObjectType::STRING; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; markLLVMDirty(); }
    void setValue(std::string&& v) { value_ = std::move(v); type_ = SereObjectType::STRING; symbol_type_ = SereSymbolType::NOT_SYMBOLIC; markLLVMDirty(); }

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

    // --- IR Conversion Support ---
    // Returns a string suitable for IR representation (e.g. for symbol tables, SSA, etc)
    virtual std::string toIRString() const {
        switch (type_) {
            case SereObjectType::INTEGER: return "i32 " + std::to_string(getInteger());
            case SereObjectType::FLOAT:   return "f32 " + std::to_string(getFloat());
            case SereObjectType::STRING:  return "str \"" + getString() + "\"";
            case SereObjectType::BOOLEAN: return std::string("bool ") + (getBoolean() ? "true" : "false");
            case SereObjectType::SYMBOLIC: return "<sym:" + symbol_name_ + ">";
            default: return "none";
        }
    }

protected:
    void markLLVMDirty() noexcept { llvm_dirty_ = true; }

    std::variant<std::monostate, int, float, std::string, bool> value_;
    SereObjectType type_;
    SereSymbolType symbol_type_;
    std::string symbol_name_; // Only set for SYMBOLIC types
    llvm::Value* llvm_value_ = nullptr; // LLVM IR value for this object, if any
    bool llvm_dirty_ = true; // True if llvm_value_ needs to be regenerated
};

// === Symbolic SereObject Subclasses for IR Conversion ===

class SereParam : public SereObject {
public:
    SereObjectType param_type;
    explicit SereParam(const std::string& name, SereObjectType ty)
        : SereObject(name, SereSymbolType::VARIABLE_REF), param_type(ty) {}
};

class SereVariableRef : public SereObject {
public:
    explicit SereVariableRef(const std::string& name)
        : SereObject(name, SereSymbolType::VARIABLE_REF) {}
    std::string toIRString() const override {
        return "%var_" + getSymbolName();
    }
};

class SereFunctionRef : public SereObject {
public:
    explicit SereFunctionRef(const std::string& name)
        : SereObject(name, SereSymbolType::FUNCTION_REF) {}
    std::string toIRString() const override {
        return "@func_" + getSymbolName();
    }
};

class SereClassRef : public SereObject {
public:
    explicit SereClassRef(const std::string& name)
        : SereObject(name, SereSymbolType::CLASS_REF) {}
    std::string toIRString() const override {
        return "@class_" + getSymbolName();
    }
};

class SereFuncAssign : public SereObject {
public:
    llvm::Function* llvm_function = nullptr;

    explicit SereFuncAssign(const std::string& name, llvm::Function* func)
        : SereObject(name, SereSymbolType::FUNCTION_ASSIGN), llvm_function(func) {
        setLLVMValue(func);
    }
};

// Add more subclasses as needed for other symbolic types (calls, assigns, etc).

// === AST Node Base ===
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toIR() const { return ""; }
    // Optionally, carry a context LLVM Value* for this node
    llvm::Value* getLLVMValue() const noexcept { return llvm_value_; }
    void setLLVMValue(llvm::Value* v) noexcept { llvm_value_ = v; }
protected:
    llvm::Value* llvm_value_ = nullptr;
};

} // namespace SereParser

#endif // SEREPARSER_AST_HPP