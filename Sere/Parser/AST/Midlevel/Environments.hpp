#pragma once
#ifndef SEREPARSER_ENVIRONMENTS_HPP
#define SEREPARSER_ENVIRONMENTS_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

namespace Runtime {

    // TypeKind for semantic/type checking
    enum class SereTypeKind {
        INT,
        FLOAT,
        STRING,
        BOOL,
        NONE,
        UNKNOWN
    };

    // Utility for type-to-string conversion
    inline std::string to_string(SereTypeKind kind) {
        switch (kind) {
            case SereTypeKind::INT:     return "int";
            case SereTypeKind::FLOAT:   return "float";
            case SereTypeKind::STRING:  return "str";
            case SereTypeKind::BOOL:    return "bool";
            case SereTypeKind::NONE:    return "none";
            case SereTypeKind::UNKNOWN: return "unknown";
        }
        return "unknown";
    }

    // Type environment for semantic analysis (scoped)
    class TypeEnvironment : public std::enable_shared_from_this<TypeEnvironment> {
    public:
        std::shared_ptr<TypeEnvironment> parent_env;
        std::unordered_map<std::string, SereTypeKind> table;
        
        explicit TypeEnvironment(std::shared_ptr<TypeEnvironment> parent = nullptr)
            : parent_env(std::move(parent)) {}

        // Add or update a type binding in the current scope
        void set(const std::string& name, SereTypeKind kind) {
            table[name] = kind;
        }

        // Lookup a type binding recursively through parent scopes
        SereTypeKind get_(const std::string& name) const {
            auto it = table.find(name);
            if (it != table.end()) return it->second;
            if (parent_env) {
                // Debug: print parent_env address for tracing
                // std::cerr << "Recursing to parent_env: " << parent_env.get() << std::endl;
                return parent_env->get_(name);
            }
            return SereTypeKind::UNKNOWN;
        }

        // Check if the binding exists in the current scope only
        bool exists_in_current(const std::string& name) const {
            return table.find(name) != table.end();
        }

        // Create a child environment/scope
        std::shared_ptr<TypeEnvironment> create_child_scope() {
            // Always use shared_from_this for parent_env!
            return std::make_shared<TypeEnvironment>(shared_from_this());
        }
    };

} // namespace Runtime

#endif // SEREPARSER_ENVIRONMENTS_HPP