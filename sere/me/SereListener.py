from __future__ import annotations
from typing import Optional, Dict, List, Any
from antlr4 import ParseTreeWalker
from sere.fe.gram.SereLexer import SereLexer
from sere.fe.gram.SereParser import SereParser
from sere.fe.gram.SereParserListener import SereParserListener
from sere.me.Error import SemanticError
from sere.me.Symbols import ScopedSymbolTable, Symbol


class SemanticAnalyzer(SereParserListener):
    """
    Listener for walking the parse tree and performing semantic analysis
    """

    def __init__(self, scope: ScopedSymbolTable | None = None):
        self.current_scope: ScopedSymbolTable = scope or ScopedSymbolTable("global")
        self.errors: List[SemanticError] = []
        self.scope_depth = 0

    def push_scope(self, name: str) -> None:
        new_scope = ScopedSymbolTable(scope_name=name, parent=self.current_scope)
        self.current_scope = new_scope
        self.scope_depth += 1

    def pop_scope(self) -> None:
        self.current_scope = self.current_scope.parent or self.current_scope
        self.scope_depth -= 1

    def register_symbol(self, name: str, _type: str | None = None) -> None:
        symbol = Symbol(name, sym_type=_type)
        try:
            self.current_scope.insert(symbol)
        except SemanticError as e:
            self.errors.append(e)

    def infer_type(self, ctx):
        text = ctx.getText()
        if text.isdigit():
            return "int"
        elif text.startswith('"') or text.startswith("'"):
            return "str"
        elif text.replace(".", "", 1).isdigit():
            return "float"
        elif text in ("True", "False"):
            return "bool"
        return "Any"

    def enterFuncdef(self, ctx: SereParser.FuncdefContext):
        func_name = ctx.name().getText()
        symbol = Symbol(func_name, "function")

        self.push_scope(func_name)

        params_ctx = ctx.parameters()
        if params_ctx and params_ctx.typedargslist():
            typed_args = params_ctx.typedargslist()
            tfpdefs = typed_args.tfpdef()
            for tfpdef in tfpdefs:
                param_name = tfpdef.name().getText()

                self.register_symbol(param_name, "parameter")
                symbol.params.append(param_name)

        if self.current_scope.parent:
            self.current_scope.parent.insert(symbol)

        return super().enterFuncdef(ctx)

    def exitFuncdef(self, ctx: SereParser.FuncdefContext):
        self.pop_scope()
        return super().exitFuncdef(ctx)

    def enterName(self, ctx: SereParser.NameContext):
        name = ctx.getText()
        symbol = self.current_scope.lookup(name)
        if not symbol:
            self.errors.append(SemanticError(f"Undefined identifier '{name}'"))

        return super().enterName(ctx)

    def enterExpr_stmt(self, ctx: SereParser.Expr_stmtContext):
        lhs = ctx.testlist_star_expr(0)

        if ctx.annassign():
            name = lhs.getText()
            annotation = ctx.annassign().test(0).getText()
            self.register_symbol(name, _type=annotation)

        elif ctx.augassign():
            name = lhs.getText()
            symbol = self.current_scope.lookup(name)
            if not symbol:
                self.register_symbol(name, _type="auto")

        elif ctx.getChildCount() >= 3 and ctx.getChild(1).getText() == "=":
            rhs = ctx.testlist_star_expr()[-1]
            inferred_type = self.infer_type(rhs)

            for target in ctx.testlist_star_expr():
                name = target.getText()
                existing = self.current_scope.lookup(name)
                if not existing:
                    self.register_symbol(name, _type=inferred_type)
        return super().enterExpr_stmt(ctx)
