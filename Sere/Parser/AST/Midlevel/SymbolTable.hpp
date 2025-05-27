#ifndef MIDLEVEL_SYMBOLTABLE_HPP
#define MIDLEVEL_SYMBOLTABLE_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include <optional>
#include "../AST.hpp" // Adjust include path as needed

namespace Runtime {

class SymbolEntry {
public:
    SereParser::SereObject value; // Value associated with the symbol
    bool is_mutable;              // Is this symbol mutable (true) or const (false)?

    // Constructors
    SymbolEntry(const SereParser::SereObject& v, bool mut = true)
        : value(v), is_mutable(mut) {}

    SymbolEntry() : value(), is_mutable(true) {}

    // Copy and move
    SymbolEntry(const SymbolEntry&) = default;
    SymbolEntry(SymbolEntry&&) noexcept = default;
    SymbolEntry& operator=(const SymbolEntry&) = default;
    SymbolEntry& operator=(SymbolEntry&&) noexcept = default;

    // Utility for shared pointer
    std::shared_ptr<SymbolEntry> get_shared() const {
        return std::make_shared<SymbolEntry>(*this);
    }
};

class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
public:
    // Create a new symbol table, optionally with a parent scope
    explicit SymbolTable(std::shared_ptr<SymbolTable> parent = nullptr)
        : parent_scope(std::move(parent)) {}

    // Insert: only if not present in current scope
    void insert(const std::string& name, const SymbolEntry& entry) {
        auto [it, inserted] = symbols.emplace(name, entry);
        if (!inserted)
            throw std::runtime_error("Symbol already declared in this scope: " + name);
    }

    // Update: only if present in current scope
    void update(const std::string& name, const SymbolEntry& entry) {
        auto it = symbols.find(name);
        if (it == symbols.end())
            throw std::runtime_error("Cannot update undeclared symbol: " + name);
        it->second = entry;
    }

    // Insert or update (Python-like assignment semantics)
    void insert_or_update(const std::string& name, const SymbolEntry& entry) {
        symbols[name] = entry;
    }

    // Lookup: recursively up through parents
    std::shared_ptr<SymbolEntry> lookup(const std::string& name) const {
        auto it = symbols.find(name);
        if (it != symbols.end())
            return it->second.get_shared();
        if (parent_scope)
            return parent_scope->lookup(name);
        return nullptr;
    }

    // Check if symbol exists in current scope only
    bool exists_in_current(const std::string& name) const {
        return symbols.find(name) != symbols.end();
    }

    // Create a child scope (for blocks/functions)
    std::shared_ptr<SymbolTable> create_child_scope() {
        return std::make_shared<SymbolTable>(shared_from_this());
    }

private:
    std::unordered_map<std::string, SymbolEntry> symbols;
    std::shared_ptr<SymbolTable> parent_scope;
};

} // namespace Runtime

#endif // MIDLEVEL_SYMBOLTABLE_HPP