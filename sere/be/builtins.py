from sere.me.Symbols import ScopedSymbolTable, Symbol
from llvmlite import ir


def register_builtin_types(scope: ScopedSymbolTable) -> None:
    scope.insert(Symbol("int", sym_type="type"))
    scope.insert(Symbol("float", sym_type="type"))
    scope.insert(Symbol("str", sym_type="type"))
    scope.insert(Symbol("bool", sym_type="type"))
    scope.insert(Symbol("Any", sym_type="type"))
    scope.insert(Symbol("NoneType", sym_type="type"))


def register_builtin_constants(scope: ScopedSymbolTable) -> None:
    scope.insert(Symbol("True", sym_type="bool", is_mutable=False))
    scope.insert(Symbol("False", sym_type="bool", is_mutable=False))
    scope.insert(Symbol("None", sym_type="NoneType", is_mutable=False, llvm_ref=None))
