from typing import Optional, Dict, List, Any, Tuple
from sere.me.Error import SemanticError
from llvmlite import ir

_BUILTIN_TYPEMAP = {
    "int": ir.IntType(32),
    "float": ir.FloatType(),
    "bool": ir.IntType(1),
    "str": ir.IntType(8).as_pointer(),
    "NoneType": ir.VoidType(),
}


class Symbol:
    """
    Represents a symbol in a symbol table.
    Could be variable, function, class, etc.
    """

    def __init__(
        self,
        name: str,
        sym_type: Optional[str] = None,
        is_mutable: bool = True,
        value: Any = None,
        params: Optional[List[str]] = None,
        llvm_ref: Optional[ir.Value] = None,
    ):
        self.name = name
        self.type = sym_type
        self.is_mutable = is_mutable
        self.value = value
        self.params = params or []
        self.llvm_ref = llvm_ref

    def as_llvm_type(self) -> ir.Type:
        try:
            return _BUILTIN_TYPEMAP[self.name]
        except Exception as e:
            raise TypeError(f"invalid type '{self.type}'")

    def __repr__(self):
        return f"<Symbol name={self.name} type={self.type}>"


class ClassSymbol(Symbol):
    def __init__(self, name: str, fields: Dict[str, Symbol], llvm_ref: Any = None):
        super().__init__(name=name, sym_type="class", llvm_ref=llvm_ref)
        self.fields: Dict[str, Symbol] = fields


class ScopedSymbolTable:
    """
    Symbol table with support for nested scopes.
    """

    def __init__(self, scope_name: str, parent: Optional["ScopedSymbolTable"] = None):
        self.scope_name = scope_name
        self.parent = parent
        self.symbols: Dict[str, Symbol] = {}
        self.scope_level = 0 if parent is None else parent.scope_level + 1

    def dump(self, indent: int = 0):
        pad = "  " * indent
        print(f"{pad}Scope: {self.scope_name} (Level {self.scope_level})")
        for name, sym in self.symbols.items():
            print(f"{pad}  {name}: {sym}")
        if self.parent:
            self.parent.dump(indent + 1)

    def insert(self, symbol: Symbol) -> None:
        if symbol.name in self.symbols:
            raise SemanticError(
                f"Duplicate symbol '{symbol.name}' in scope '{self.scope_name}'"
            )
        self.symbols[symbol.name] = symbol
        # Could add logging here

    def lookup(self, name: str) -> Optional[Symbol]:
        sym = self.symbols.get(name)
        if sym:
            return sym
        if self.parent:
            return self.parent.lookup(name)
        return None

    def exists_in_current_scope(self, name: str) -> bool:
        return name in self.symbols

    def is_global_scope(self) -> bool:
        return self.scope_level == 0

    def get_field_info(self, qualified: str) -> Tuple[int, str]:
        try:
            rec_name, field_name = qualified.split(".", 1)
        except ValueError:
            raise KeyError(f"Bad qualifier '{qualified}'.")

        rec_sym = self.lookup(rec_name)
        if not isinstance(rec_sym, ClassSymbol):
            raise KeyError(f"Type '{rec_name}' is not a valid structure.")

        for idx, (fname, (ftype, offset)) in enumerate(rec_sym.fields.items()):  # type: ignore
            if fname == field_name:
                return idx, ftype

        raise KeyError(f"Field '{field_name}' not found on type '{rec_name}'.")

    def __repr__(self):
        return (
            f"<ScopedSymbolTable {self.scope_name} symbols={list(self.symbols.keys())}>"
        )
