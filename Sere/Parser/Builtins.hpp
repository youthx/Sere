#pragma once
#ifndef SERE_BUILTINS_HPP
#define SERE_BUILTINS_HPP

#include "AST/Midlevel/Environments.hpp"
#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>

namespace BuiltIn {

    // Base class for all Sere types (built-in and user-defined)
    class SereType {
    public:
        using Ptr = std::shared_ptr<SereType>;
        virtual ~SereType() = default;

        explicit SereType(Runtime::SereTypeKind parent_type)
            : parent(parent_type) {}

        // Returns the name of the type (override for custom types)
        virtual std::string name() const = 0;

        // Type comparison utility
        virtual bool is_assignable_from(const SereType& other) const {
            return parent == other.parent;
        }

        // For debugging
        virtual void debug_print(std::ostream& os = std::cout) const {
            os << name() << " (parent: " << Runtime::to_string(parent) << ")";
        }

        Runtime::SereTypeKind parent;
    };

    // Built-in type singletons
    class IntType : public SereType {
    public:
        IntType() : SereType(Runtime::SereTypeKind::INT) {}
        std::string name() const override { return "int"; }
    };
    class FloatType : public SereType {
    public:
        FloatType() : SereType(Runtime::SereTypeKind::FLOAT) {}
        std::string name() const override { return "float"; }
    };
    class BoolType : public SereType {
    public:
        BoolType() : SereType(Runtime::SereTypeKind::BOOL) {}
        std::string name() const override { return "bool"; }
    };
    class StringType : public SereType {
    public:
        StringType() : SereType(Runtime::SereTypeKind::STRING) {}
        std::string name() const override { return "str"; }
    };
    class NoneType : public SereType {
    public:
        NoneType() : SereType(Runtime::SereTypeKind::NONE) {}
        std::string name() const override { return "none"; }
    };

    // Convenient access to built-in types (as singletons)
    inline SereType::Ptr INT  = std::make_shared<IntType>();
    inline SereType::Ptr FLOAT = std::make_shared<FloatType>();
    inline SereType::Ptr BOOL  = std::make_shared<BoolType>();
    inline SereType::Ptr STR   = std::make_shared<StringType>();
    inline SereType::Ptr NONE  = std::make_shared<NoneType>();

    // Built-in type registry for lookup by name
    inline std::unordered_map<std::string, SereType::Ptr> builtins = {
        {"int", INT},
        {"float", FLOAT},
        {"bool", BOOL},
        {"str", STR},
        {"string", STR},
        {"None", NONE}
    };


    
    // Lookup utility
    inline SereType::Ptr get_builtin_type(const std::string& name) {
        auto it = builtins.find(name);
        if (it != builtins.end()) return it->second;
        return nullptr;
    }
    
}

#endif // SERE_BUILTINS_HPP