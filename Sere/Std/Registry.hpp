#ifndef STD_REGISTRY_HPP
#define STD_REGISTRY_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>


#include "./Standard.hpp"
#include "../Parser/AST/Visitor.hpp"

namespace SereLib {
    
    using LibRegistry = std::function<void(llvm::Module&, llvm::LLVMContext&)>;

    inline std::unordered_map<std::string, LibRegistry>& get_library_registry() {
        static std::unordered_map<std::string, LibRegistry> registry;
        return registry;
    }

    inline void register_library(const std::string& lib_name, LibRegistry func) {
        get_library_registry()[lib_name] = std::move(func);
    }

    inline void include_lib(const std::string& lib) {
        auto &module = *SereParser::RT::ctx.get_module();

        auto &reg = get_library_registry();
        auto it = reg.find(lib);
        if (it == reg.end()) {
            throw std::domain_error("Cannot find library " + lib + " in the std registry.");
        }        

        it->second(module, SereParser::RT::ctx.llvm_ctx);
    }

    inline void load_registry() {
        register_library("core", init_core);

    }
}

#endif // STD_REGISTRY_HPP