#ifndef IR_CODEGENCONTEXT_HPP
#define IR_CODEGENCONTEXT_HPP

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>
#include <stack>
#include <vector>
#include <unordered_map>
#include <optional>

namespace SereIR {

    class CodeGenContext {
    public:
        llvm::LLVMContext llvm_ctx;
        std::unique_ptr<llvm::Module> module;
        llvm::IRBuilder<> builder;

        llvm::Function* function = nullptr;
        llvm::Function* entry_point = nullptr;

        CodeGenContext()
            : module(std::make_unique<llvm::Module>("__module__", llvm_ctx)),
              builder(llvm_ctx) {
            // Always push global scope at the bottom
            named_value_stack.emplace_back(); 
            create_entry();
            done();
        }

        ~CodeGenContext() = default;

        llvm::Module* get_module() { return module.get(); }

        void done() {
            if (entry_point && !entry_point->empty() && !entry_point->back().getTerminator()) {
                builder.CreateRetVoid();
            }
        }

        //
        // ===== Scoped Variable Support =====
        //

        void push_scope() {
            named_value_stack.emplace_back();
        }

        void pop_scope() {
            if (named_value_stack.size() <= 1) {
                throw std::runtime_error("CodeGenContext: Cannot pop global scope.");
            }
            named_value_stack.pop_back();
        }

        void set_named_value(const std::string& name, llvm::Value* val) {
            if (named_value_stack.empty()) {
                throw std::runtime_error("CodeGenContext: named_value_stack is empty on set.");
            }
            named_value_stack.back()[name] = val;
        }

        llvm::Value* get_named_value(const std::string& name) const {
            for (auto it = named_value_stack.rbegin(); it != named_value_stack.rend(); ++it) {
                auto found = it->find(name);
                if (found != it->end()) return found->second;
            }
            return nullptr;
        }

        void clear_local_scopes() {
            if (named_value_stack.size() > 1)
                named_value_stack.erase(named_value_stack.begin() + 1, named_value_stack.end());
        }

        llvm::Value* get_global_value(const std::string& name) const {
            const auto& global = named_value_stack.front();
            auto found = global.find(name);
            return (found != global.end()) ? found->second : nullptr;
        }

    private:
        void create_entry() {
            std::vector<llvm::Type*> arg_types;
            auto func_type = llvm::FunctionType::get(
                llvm::Type::getVoidTy(llvm_ctx), arg_types, false
            );

            auto *enterance = llvm::Function::Create(
                func_type, llvm::Function::ExternalLinkage, "__init__", module.get()
            );

            auto* bb = llvm::BasicBlock::Create(llvm_ctx, "entry", enterance);
            builder.SetInsertPoint(bb);

            entry_point = enterance;
            function = enterance;
        }

        // [0] is always the global scope
        std::vector<std::unordered_map<std::string, llvm::Value*>> named_value_stack;
    };

} // namespace SereIR

#endif // IR_CODEGENCONTEXT_HPP
