#ifndef MIDLEVEL_SYMBOLTABLE_HPP
#define MIDLEVEL_SYMBOLTABLE_HPP
#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include <stdexcept>

#include "../AST.hpp" // Adjust include path as needed

namespace Runtime {
    class SymbolEntry {
    public:
        SereParser::SereObject value; // Optional: default or constant value if known
        bool is_mutable;              // For variable mutability (const vs var)

        // Constructor with explicit value and mutability
        SymbolEntry(const SereParser::SereObject& v, bool mut)
            : value(v), is_mutable(mut) {}

        // Constructor with only value (defaults to mutable)
        explicit SymbolEntry(const SereParser::SereObject& v)
            : value(v), is_mutable(true) {}

        // Default constructor (default-initialized value, mutable)
        SymbolEntry()
            : value(), is_mutable(true) {}

        // Copy constructor
        SymbolEntry(const SymbolEntry& other)
            : value(other.value), is_mutable(other.is_mutable) {}

        // Move constructor
        SymbolEntry(SymbolEntry&& other) noexcept
            : value(std::move(other.value)), is_mutable(other.is_mutable) {}

        // Assignment operators
        SymbolEntry& operator=(const SymbolEntry& other) {
            if (this != &other) {
                value = other.value;
                is_mutable = other.is_mutable;
            }
            return *this;
        }

        SymbolEntry& operator=(SymbolEntry&& other) noexcept {
            if (this != &other) {
                value = std::move(other.value);
                is_mutable = other.is_mutable;
            }
            return *this;
        }

        std::shared_ptr<SymbolEntry> get_shared() const {
            return std::make_shared<SymbolEntry>(*this);
        }
    };

    class SymbolTable {
    public:
        explicit SymbolTable(std::shared_ptr<SymbolTable> parent = nullptr)
            : parent_scope(std::move(parent)) {}
        
        void insert(const std::string& name, SymbolEntry entry) {
            if (symbols.find(name) != symbols.end())
                throw std::runtime_error("Symbol already declared in this scope: " + name);
            symbols[name] = std::move(entry);
        }

        // Lookup symbol recursively through parent scopes
        std::shared_ptr<SymbolEntry> lookup(const std::string& name) const {
            auto it = symbols.find(name);
            if (it != symbols.end()) return it->second.get_shared();
            if (parent_scope) return parent_scope->lookup(name);
            return nullptr;
        }

        // Check if symbol exists in current scope only
        bool exists_in_current(const std::string& name) const {
            return symbols.find(name) != symbols.end();
        }

        // Create nested scope from current scope
        std::shared_ptr<SymbolTable> create_child_scope() {
            std::shared_ptr<SymbolTable> table = std::make_shared<SymbolTable>(*this);
            return table;
        }

    private:
        std::unordered_map<std::string, SymbolEntry> symbols;
        std::shared_ptr<SymbolTable> parent_scope;
    };
}

#endif //MIDLEVEL_SYMBOLTABLE_HPP