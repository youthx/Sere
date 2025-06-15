from copy import deepcopy
from typing import Optional
from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker
from antlr4.tree.Tree import ParseTree

from llvmlite import ir
from llvmlite import binding as llvm


from sere.fe.gram.SereLexer import SereLexer
from sere.fe.gram.SereParser import SereParser

from sere.me.SereListener import SemanticAnalyzer
from sere.me.Symbols import ScopedSymbolTable
from sere.me.SereVisitor import IRBuilderVisitor

from sere.be.builtins import register_builtin_constants, register_builtin_types


def compile_source(source_code: str, filename: Optional[str] = "<stdin>") -> ir.Module:
    """
    Frontend compilation pipeline: Lexing + Parsing only.
    """

    llvm.initialize()
    llvm.initialize_native_target()
    llvm.initialize_native_asmprinter()

    # Stage 1: Lexing
    input_stream = InputStream(source_code)
    lexer = SereLexer(input_stream)
    token_stream = CommonTokenStream(lexer)

    # Stage 2: Parsing
    parser = SereParser(token_stream)
    walker = ParseTreeWalker()
    # parser.removeErrorListeners()  # Add custom listeners later if needed
    tree: ParseTree = parser.file_input()  # Start symbol: file_input

    global_scope = ScopedSymbolTable("global")
    register_builtin_types(global_scope)
    register_builtin_constants(global_scope)

    semantic_analyzer = SemanticAnalyzer(scope=deepcopy(global_scope))
    walker.walk(semantic_analyzer, tree)

    if semantic_analyzer.errors:
        for error in semantic_analyzer.errors:
            print(f"{error}")

    # Stage 3: (Future) Visiting / LLVM IR generation
    ir_builder = IRBuilderVisitor(scope=global_scope)
    module = ir_builder.visitFile_input(tree)

    llvm_ir = str(module)
    llvm_module = llvm.parse_assembly(llvm_ir)
    llvm_module.source_file_name = "<stdin>"
    llvm_module.verify()

    target = llvm.Target.from_default_triple()
    target_machine = target.create_target_machine()

    pass_manager = llvm.create_module_pass_manager()
    pass_manager.add_constant_merge_pass()
    pass_manager.add_global_dce_pass()

    pass_builder = llvm.create_pass_manager_builder()
    pass_builder.opt_level = 3

    pass_builder.populate(pass_manager)
    pass_manager.add_strip_dead_prototypes_pass()
    pass_manager.add_strip_symbols_pass()
    pass_manager.run(llvm_module)

    return llvm_module


def emit_artifacts(module: ir.Module, basename: str = "output"):
    target = llvm.Target.from_default_triple()
    target_machine = target.create_target_machine()

    llvm_module = llvm.parse_assembly(str(module))
    llvm_module.verify()

    obj_code = target_machine.emit_object(llvm_module)
    asm_code = target_machine.emit_assembly(llvm_module)
    ir_code = str(llvm_module)

    return {
        "obj": obj_code,
        "asm": asm_code,
        "ir": str(ir_code),
    }
