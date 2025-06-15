from collections import defaultdict
from typing import Any, Dict, List, Optional
from dataclasses import dataclass, field

from llvmlite import ir, binding
from sere.fe.gram.SereParserVisitor import SereParserVisitor
from sere.fe.gram.SereParser import SereParser
from sere.me.Symbols import Symbol, ScopedSymbolTable

from antlr4.ParserRuleContext import ParserRuleContext
from antlr4.tree.Tree import TerminalNode


class IRBuilderVisitor(SereParserVisitor):

    def __init__(
        self,
        target: Optional[binding.Target] = None,
        target_machine: Optional[binding.TargetMachine] = None,
        scope: Optional[ScopedSymbolTable] = None,
    ):

        self.target = target or binding.Target.from_default_triple()
        self.target_machine = target_machine or self.target.create_target_machine()

        self.current_scope: ScopedSymbolTable = scope or ScopedSymbolTable("global")
        self.scope_depth = 0

        self.module = ir.Module(name="__module__")
        self.module.triple = binding.get_default_triple()  # type: ignore
        self.module.data_layout = str(self.target_machine.target_data)  # type: ignore

        self.global_func = ir.Function(
            self.module, ir.FunctionType(ir.VoidType(), []), name="__global__"
        )
        self.global_block = self.global_func.append_basic_block("entry")
        self.builder = ir.IRBuilder(self.global_block)

        self.main_defined = False
        self.main_func = None

        self.global_return_added = False

        self.default_type_name = "int"

        self.temp_index = 0

        self.reference_counts = defaultdict(int)

    def defi(self, ty: str, id_: str = ""):
        base = f"_{ty}_{id_}"
        index = self.reference_counts[base]
        self.reference_counts[base] += 1
        return f"{base}{index}"

    def ref(self):
        base = f"_tmp"
        index = self.reference_counts[base]
        self.reference_counts[base] += 1
        return f"{base}{index}"

    def tmp(self, name: str = ""):
        base = f"_tmp_{name}"
        index = self.reference_counts[base]
        self.reference_counts[base] += 1
        return f"{base}{index}"

    def resolve_type(self, type_name: str) -> str:
        norm = type_name.strip()
        if self.current_scope.lookup(norm):
            return norm
        raise TypeError(f"Unknown type '{type_name}'")

    def as_bool(self, val: ir.Value) -> ir.Value:
        if isinstance(val.type, ir.IntType) and val.type.width != 1:  # type: ignore
            return self.builder.icmp_signed("!=", val, ir.Constant(val.type, 0))  # type: ignore
        return val

    def to_llvm_type(self, type_name: str | ir.Type) -> ir.Type:
        if isinstance(type_name, ir.Type):
            return type_name

        norm = self.resolve_type(type_name)
        return self.current_scope.lookup(norm).as_llvm_type()  # type: ignore

    def default_type(self) -> str:
        return self.default_type_name

    def get_default_value(self, type_name: str) -> ir.Constant | None:
        llvm_ty = self.to_llvm_type(type_name)
        if isinstance(llvm_ty, ir.IntType):
            return ir.Constant(llvm_ty, 0)
        elif isinstance(llvm_ty, ir.FloatType):
            return ir.Constant(llvm_ty, 0.0)
        elif isinstance(llvm_ty, ir.PointerType):
            return ir.Constant(llvm_ty, None)
        elif isinstance(llvm_ty, ir.VoidType):
            return None
        else:
            raise TypeError(f"No default type implemented")

    def push_scope(self, name: str) -> None:
        new_scope = ScopedSymbolTable(scope_name=name, parent=self.current_scope)
        self.current_scope = new_scope
        self.scope_depth += 1

    def pop_scope(self) -> None:
        self.current_scope = self.current_scope.parent or self.current_scope
        self.scope_depth -= 1

    def visitFile_input(self, ctx: SereParser.File_inputContext):
        for stmt in ctx.children:
            self.visit(stmt)

        if not self.global_return_added:
            self.builder.ret_void()
            self.global_return_added = True

        return self.module

    def visitStmt(self, ctx: SereParser.StmtContext):
        if ctx.compound_stmt():
            return self.visit(ctx.compound_stmt())
        if ctx.simple_stmts():
            return self.visit(ctx.simple_stmts())

    def visitSimple_stmt(self, ctx: SereParser.Simple_stmtContext):
        if ctx.expr_stmt():
            return self.visit(ctx.expr_stmt())
        elif ctx.del_stmt():
            return self.visit(ctx.del_stmt())
        elif ctx.pass_stmt():
            return self.visit(ctx.pass_stmt())
        elif ctx.flow_stmt():
            return self.visit(ctx.flow_stmt())
        elif ctx.import_stmt():
            return self.visit(ctx.import_stmt())
        elif ctx.global_stmt():
            return self.visit(ctx.global_stmt())
        elif ctx.nonlocal_stmt():
            return self.visit(ctx.nonlocal_stmt())
        elif ctx.assert_stmt():
            return self.visit(ctx.assert_stmt())
        else:
            raise NotImplementedError("Unhandled simple_stmt case")

    def visitCompound_stmt(self, ctx: SereParser.Compound_stmtContext):
        if ctx.if_stmt():
            return self.visit(ctx.if_stmt())
        if ctx.while_stmt():
            return self.visit(ctx.while_stmt())
        if ctx.for_stmt():
            return self.visit(ctx.for_stmt())
        if ctx.try_stmt():
            return self.visit(ctx.try_stmt())
        if ctx.with_stmt():
            return self.visit(ctx.with_stmt())
        if ctx.funcdef():
            return self.visit(ctx.funcdef())
        if ctx.classdef():
            return self.visit(ctx.classdef())
        if ctx.decorated():
            return self.visit(ctx.decorated())
        if ctx.async_stmt():
            return self.visit(ctx.async_stmt())
        if ctx.match_stmt():
            return self.visit(ctx.match_stmt())
        else:
            raise Exception(f"Invalid statement: {ctx.getText()}")

    def visitExpr_stmt(self, ctx: SereParser.Expr_stmtContext):
        # 1) Annotated assignment:  x: T = expr
        if ctx.annassign():
            ann = ctx.annassign()
            lhs_list = ctx.testlist_star_expr(0)
            # We only allow single‐target annotated assigns for now
            var_name = lhs_list.getText()
            # annotation is the first test()
            type_name = ann.test(0).getText()
            llvm_ty = self.to_llvm_type(type_name)

            # value is optional second test()
            if ann.getChildCount() == 3:
                value_ir = self.visit(ann.test(1))
            else:
                value_ir = self.get_default_value(type_name)

            ptr = self.builder.alloca(llvm_ty, name=self.defi("alloc", var_name))
            self.builder.store(value_ir, ptr)
            self.current_scope.insert(Symbol(var_name, type_name, llvm_ref=ptr))
            return value_ir

        # 2) Augmented assignment:  x += expr, x -= expr, etc.
        if ctx.augassign():
            lhs_list = ctx.testlist_star_expr(0)
            var_name = lhs_list.getText()
            op_token = ctx.augassign().getText()  # e.g. "+="
            sym = self.current_scope.lookup(var_name)
            if sym is None or sym.llvm_ref is None:
                raise NameError(f"Undefined variable '{var_name}'")

            # load current
            current = self.builder.load(sym.llvm_ref, name=self.tmp(f"load_{var_name}"))

            # RHS is either a yield_expr or a testlist
            rhs_ctx = ctx.yield_expr() or ctx.testlist()
            # antlr4 returns lists for these accessors
            rhs_ctx = rhs_ctx[0] if isinstance(rhs_ctx, list) else rhs_ctx
            rhs_val = self.visit(rhs_ctx)

            # dispatch the op
            if op_token == "+=":
                result = self.builder.add(current, rhs_val, name=self.tmp("add"))
            elif op_token == "-=":
                result = self.builder.sub(current, rhs_val, name=self.tmp("sub"))
            elif op_token == "*=":
                result = self.builder.mul(current, rhs_val, name=self.tmp("mul"))
            elif op_token == "/=":
                result = self.builder.sdiv(current, rhs_val, name=self.tmp("div"))
            elif op_token == "%=":
                result = self.builder.srem(current, rhs_val, name=self.tmp("rem"))
            else:
                raise NotImplementedError(f"Unsupported augassign '{op_token}'")

            self.builder.store(result, sym.llvm_ref)
            return result

        # 3) Plain (possibly chained) assignment:  a = b = c
        lists = ctx.testlist_star_expr()
        if len(lists) >= 2 and ctx.getChild(1).getText() == "=":
            # evaluate RHS once
            rhs_ctx = lists[-1]
            rhs_val = self.visit(rhs_ctx)
            final_val = rhs_val

            # for each LHS list from left to right
            for lhs_list in lists[:-1]:
                # flatten commas
                targets = [
                    c
                    for c in lhs_list.getChildren()
                    if isinstance(c, ParserRuleContext) and c.getText() != ","
                ]
                for t in targets:
                    name = t.getText()
                    sym = self.current_scope.lookup(name)
                    if sym is None:
                        # declare new
                        llvm_ty = final_val.type  # now an ir.Type
                        ptr = self.builder.alloca(
                            llvm_ty, name=self.defi("alloc", name)
                        )
                        self.builder.store(final_val, ptr)
                        self.current_scope.insert(
                            Symbol(name, str(llvm_ty), llvm_ref=ptr)
                        )
                    else:
                        ptr = sym.llvm_ref  # type: ignore
                        self.builder.store(final_val, ptr)
                    # reload for next in chain
                    final_val = self.builder.load(ptr, name=self.tmp(f"load_{name}"))

            return final_val

        # 4) Expression statement: just evaluate and discard
        #    (`x` alone, or function call, etc.)
        first_list = ctx.testlist_star_expr(0)
        return self.visit(first_list)

    def visitTestlist_star_expr(self, ctx: SereParser.Testlist_star_exprContext):
        values: List[ir.Value] = []

        for child in ctx.getChildren():
            text = child.getText()
            if text == ",":
                continue

            val = self.visit(child)
            if isinstance(val, list):
                values.extend(val)
            else:
                values.append(val)
        if len(values) == 1:
            return values[0]
        return values

    def visitTestlist(self, ctx: SereParser.TestlistContext):
        exprs = []
        for child in ctx.children:
            if isinstance(child, SereParser.TestContext):
                exprs.append(self.visit(child))
        return exprs

    def visitTest(self, ctx: SereParser.TestContext):
        if ctx.getChildCount() == 5 and ctx.getChild(1).getText() == "if":
            then_val = self.visit(ctx.or_test(0))
            cond_val = self.visit(ctx.or_test(1))
            else_val = self.visit(ctx.test())

            bool_cond = self.as_bool(cond_val)

            then_bb = self.builder.append_basic_block("or_test_tmp.then")
            else_bb = self.builder.append_basic_block("or_test_tmp.else")
            cont_bb = self.builder.append_basic_block("or_test_tmp.cont")

            self.builder.cbranch(bool_cond, then_bb, else_bb)

            self.builder.position_at_start(then_bb)
            then_result = then_val
            self.builder.branch(cont_bb)
            then_bb = self.builder.block

            self.builder.position_at_start(else_bb)
            else_result = else_val
            self.builder.branch(cont_bb)
            else_bb = self.builder.block

            self.builder.position_at_start(cont_bb)
            phi = self.builder.phi(then_result.type)
            phi.add_incoming(then_result, then_bb)
            phi.add_incoming(else_result, else_bb)

            return phi

        if ctx.or_test():
            return self.visit(ctx.or_test(0))

        if ctx.lambdef():
            raise NotImplementedError("Lambda expressions are WIP")
        raise NotImplementedError(f"Unhandled test node: {ctx.getText()}")

    def visitOr_test(self, ctx: SereParser.Or_testContext):
        result = self.visit(ctx.and_test(0))

        if len(ctx.and_test()) == 1:
            return result

        end_block = self.builder.append_basic_block("or_test_tmp.end")
        phi = self.builder.phi(result.type)

        for i in range(1, len(ctx.and_test())):
            cond = self.as_bool(result)

            # Blocks for branching
            next_block = self.builder.append_basic_block(f"or_test_tmp.next_{i}")
            or_true_block = self.builder.append_basic_block(f"or_test_tmp.true_{i}")

            # Branch on condition
            self.builder.cbranch(cond, or_true_block, next_block)

            # True block
            self.builder.position_at_start(or_true_block)
            phi.add_incoming(ir.Constant(result.type, 1), self.builder.block)
            self.builder.branch(end_block)

            # Next block (eval next and_test)
            self.builder.position_at_start(next_block)
            next_val = self.visit(ctx.and_test(i))
            result = next_val

            # Add incoming from previous block
            phi.add_incoming(result, next_block)

        # Finish by branching to end
        self.builder.branch(end_block)
        self.builder.position_at_start(end_block)

        return phi

    def visitAnd_test(self, ctx: SereParser.And_testContext):
        left = self.visit(ctx.not_test(0))

        for i in range(1, len(ctx.not_test())):
            right_block = self.builder.append_basic_block(f"and_test_tmp.rblock")
            end_block = self.builder.append_basic_block(f"and_test_tmp.end")

            # if left is false then skip right
            result = self.builder.alloca(ir.IntType(1))
            self.builder.store(left, result)
            cond = self.builder.icmp_signed("!=", left, ir.Constant(ir.IntType(1), 0))
            self.builder.cbranch(cond, right_block, end_block)

            self.builder.position_at_start(right_block)
            right = self.visit(ctx.not_test(i))
            self.builder.store(right, result)
            self.builder.branch(end_block)

            self.builder.position_at_start(end_block)
            left = self.builder.load(result)

        return left

    def visitNot_test(self, ctx: SereParser.Not_testContext):
        if ctx.NOT():
            operand_res = self.visit(ctx.not_test())
            operand_val = operand_res
            if operand_val is None:
                raise ValueError("Operand of 'not' did not produce an LLVM value")

            i1 = ir.IntType(1)
            bool_val = operand_val
            if not isinstance(bool_val.type, ir.IntType) or bool_val.type.width != 1:
                bool_val = self.builder.icmp_signed(
                    "!=", bool_val, ir.Constant(operand_val.type, 0)
                )

            flipped = self.builder.xor(
                bool_val, ir.Constant(i1, 1), name=self.tmp("not")
            )
            return flipped
        else:
            return self.visit(ctx.comparison())

    def visitComparison(self, ctx: SereParser.ComparisonContext):
        left_val = self.visit(ctx.expr(0))
        if left_val is None:
            raise ValueError(
                f"Comparison left side did not produce an LLVM value.\n\tGot -> '{ctx.expr(0).getText()}'"
            )
        cmp_vals: list[ir.Value] = []

        for idx, op_ctx in enumerate(ctx.comp_op()):
            right_val = self.visit(ctx.expr(idx + 1))
            if right_val is None:
                raise ValueError("Comparison right side did not produce an LLVM value.")

            op = op_ctx.getText()

            if op == "<":
                pred = "slt"
            elif op == ">":
                pred = "sgt"
            elif op == "<=":
                pred = "sle"
            elif op == ">=":
                pred = "sge"
            elif op == "==":
                pred = "eq"
            elif op == "!=" or op == "<>":
                pred = "ne"
            else:
                # TODO: implement 'in', 'is', etc.
                raise NotImplementedError(
                    f"Comparison operator not yet defined '{op}'."
                )

            cmp_ir = self.builder.icmp_signed(
                pred, left_val, right_val, name=self.tmp("cmp")
            )
            cmp_vals.append(cmp_ir)

            left_val = right_val

        if not cmp_vals:

            return left_val

        if len(cmp_vals) == 1:
            return cmp_vals[0]

        chain = cmp_vals[0]
        for extra in cmp_vals[1:]:
            chain = self.builder.and_(chain, extra, name=self.tmp("and"))
        return chain

    def visitFuncdef(self, ctx: SereParser.FuncdefContext):
        func_name = ctx.name().getText()
        params_ctx = ctx.parameters().typedargslist()
        return_type = self.to_llvm_type(
            ctx.test().getText() if ctx.test() else "NoneType"
        )

        param_types = []
        param_names = []
        if params_ctx:
            for tfpdef in params_ctx.tfpdef():
                if tfpdef.test():
                    type_name = tfpdef.test().getText()
                else:
                    type_name = self.default_type()

                llvm_ty = self.to_llvm_type(type_name)
                param_types.append(llvm_ty)
                param_names.append(tfpdef.name().getText())

        func_type = ir.FunctionType(return_type, param_types)
        llvm_func = ir.Function(self.module, func_type, name=func_name)
        self.current_scope.insert(Symbol(func_name, "function", llvm_ref=llvm_func))

        self.push_scope(func_name)
        block = llvm_func.append_basic_block("entry")
        saved_builder = self.builder
        self.builder = ir.IRBuilder(block)

        for i, arg in enumerate(llvm_func.args):  # type: ignore
            arg.name = param_names[i]
            param_type = param_types[i]

            ptr = self.builder.alloca(param_type, name=self.defi("arg", arg.name))
            self.builder.store(arg, ptr)
            self.current_scope.insert(Symbol(arg.name, "argument", llvm_ref=ptr))

        self.visit(ctx.block())

        if not self.builder.block.is_terminated:
            self.builder.ret_void()

        self.builder = saved_builder
        self.pop_scope()

        return llvm_func

    def visitBlock(self, ctx: SereParser.BlockContext):
        if ctx.simple_stmts():
            return self.visit(ctx.simple_stmts())

        if ctx.stmt():
            for stmt in ctx.stmt():
                self.visit(stmt)
        return

    def visitReturn_stmt(self, ctx: SereParser.Return_stmtContext):
        if ctx.testlist():
            ret_val = self.visit(ctx.testlist())

            return self.builder.ret(ret_val[0])
        else:
            return self.builder.ret_void()

    def visitExpr(self, ctx: SereParser.ExprContext):
        lhs = self.visit(ctx.xor_expr(0))
        for i in range(1, len(ctx.xor_expr())):
            rhs = self.visit(ctx.xor_expr(i))
            lhs = self.builder.or_(lhs, rhs, name=self.tmp("bor"))
        return lhs

    def visitXor_expr(self, ctx: SereParser.Xor_exprContext):
        lhs = self.visit(ctx.and_expr(0))
        for i in range(1, len(ctx.and_expr())):
            rhs = self.visit(ctx.and_expr(i))
            lhs = self.builder.xor(lhs, rhs, name=self.tmp("bxor"))
        return lhs

    def visitAnd_expr(self, ctx: SereParser.And_exprContext):
        lhs = self.visit(ctx.shift_expr(0))
        for i in range(1, len(ctx.shift_expr())):
            rhs = self.visit(ctx.shift_expr(i))
            lhs = self.builder.and_(lhs, rhs, name=self.tmp("band"))
        return lhs

    def visitShift_expr(self, ctx: SereParser.Shift_exprContext):
        lhs = self.visit(ctx.arith_expr(0))
        for i in range(1, len(ctx.arith_expr())):
            rhs = self.visit(ctx.arith_expr(i))
            op = ctx.getChild(2 * i - 1).getText()
            if op == "<<":
                lhs = self.builder.shl(lhs, rhs, name=self.tmp("shl"))
            elif op == ">>":
                lhs = self.builder.ashr(lhs, rhs, name=self.tmp("ashr"))
            else:
                raise NotImplementedError(f"Unsupported shift op: {op}")
        return lhs

    def visitArith_expr(self, ctx: SereParser.Arith_exprContext):
        lhs = self.visit(ctx.term(0))
        for i in range(1, len(ctx.term())):
            rhs = self.visit(ctx.term(i))
            op = ctx.getChild(2 * i - 1).getText()
            if op == "+":
                lhs = self.builder.add(lhs, rhs, name=self.tmp("add"))
            elif op == "-":
                lhs = self.builder.sub(lhs, rhs, name=self.tmp("sub"))
            else:
                raise NotImplementedError(f"Unsupported arith op: {op}")
        return lhs

    def visitTerm(self, ctx: SereParser.TermContext):
        lhs = self.visit(ctx.factor(0))
        for i in range(1, len(ctx.factor())):
            rhs = self.visit(ctx.factor(i))
            op = ctx.getChild(2 * i - 1).getText()
            if op == "*":
                lhs = self.builder.mul(lhs, rhs, name=self.tmp("mul"))
            elif op == "@":
                lhs = self.builder.mul(lhs, rhs, name=self.tmp("mat"))
            elif op == "/":
                lhs = self.builder.sdiv(lhs, rhs, name=self.tmp("div"))
            elif op == "%":
                lhs = self.builder.srem(lhs, rhs, name=self.tmp("mod"))
            elif op == "//":
                lhs = self.builder.sdiv(lhs, rhs, name=self.tmp("flrdiv"))
            else:
                raise NotImplementedError(f"Error op type '{op}'")
        return lhs

    def visitFactor(self, ctx: SereParser.FactorContext):
        if ctx.getChildCount() == 1:
            return self.visit(ctx.power())

        op = ctx.getChild(0).getText()
        val = self.visit(ctx.factor())
        if op == "+":
            return val
        elif op == "-":
            return self.builder.neg(val, name=self.tmp("neg"))
        elif op == "~":
            self.builder.not_(val, name=self.tmp("not"))
        else:
            raise NotImplementedError(f"Error op type {op}")

    def visitPower(self, ctx: SereParser.PowerContext):
        base = self.visit(ctx.atom_expr())
        if ctx.factor():
            raise NotImplementedError("Power op not implemented yet.")
        return base

    def _unpack_trailer(self, tr: SereParser.TrailerContext):
        txt = tr.getChild(0).getText()
        if txt == "(":
            args = []
            if tr.arglist():
                for arg in tr.arglist().argument():
                    args.append(self.visit(arg))
            return "call", args

        if txt == "[":
            return "subscript", self.visit(tr.subscriptlist().subscript_(0))

        if txt == ".":
            name = tr.name().getText()
            idx, ty = self.current_scope.get_field_info(name)
            return "attribute", (idx, ty)

        raise NotImplementedError("Unknown trailer")

    def _unpack_tuple(self, elements: list[ir.Value]):
        elem_types = [e.type for e in elements]  # type: ignore
        struct_ty = ir.LiteralStructType(elem_types)

        const_vals = [getattr(e, "constant", e) for e in elements]
        return ir.Constant(struct_ty, const_vals)

    def visitAtom_expr(self, ctx: SereParser.Atom_exprContext):
        value = self.visit(ctx.atom())

        for tr in ctx.trailer():
            kind, data = self._unpack_trailer(tr)
            if kind == "call":
                # data: List[ir.Value]
                value = self.builder.call(value, data, name=f"{value.name}_call")
            elif kind == "attribute":
                # data: (field_index: int, field_type: str)
                idx, field_type = data
                ptr = self.builder.gep(
                    value,
                    [ir.Constant(ir.IntType(32), 0), ir.Constant(ir.IntType(32), idx)],
                    inbounds=True,
                )
                value = self.builder.load(
                    ptr, name=f"{value.name}_field{idx}_{field_type}"
                )
            elif kind == "subscript":
                idx_val = data
                ptr = self.builder.gep(
                    value, [ir.Constant(ir.IntType(32), 0), idx_val], inbounds=True
                )
                value = self.builder.load(ptr, name=f"{value.name}_idx")
            else:
                raise NotImplementedError(f"Unknown trailer kind {kind}")
        return value

    def visitAtom(self, ctx: SereParser.AtomContext):
        text0 = ctx.getChild(0).getText()

        if text0 == "(":
            if ctx.testlist_comp():
                values = [self.visit(t) for t in ctx.testlist_comp().test()]
                return values[0] if len(values) == 1 else self._unpack_tuple(values)

            return ir.Constant(ir.VoidType(), None)

        if text0 == "[":
            vals = []
            if ctx.testlist_comp():
                vals = [self.visit(t) for t in ctx.testlist_comp().test()]
            elem_ty = vals[0].type if vals else ir.IntType(32)
            arr_ty = ir.ArrayType(elem_ty, len(vals))
            return ir.Constant(arr_ty, [v.constant for v in vals])

        if text0 == "{":
            if ctx.dictorsetmaker():
                elems = ctx.dictorsetmaker().test()
                pairs = [
                    (self.visit(elems[i]), self.visit(elems[i + 1]))
                    for i in range(0, len(elems), 2)
                ]
                types = [p[0].type for p in pairs] + [p[1].type for p in pairs]
                consts = [p[0].constant for p in pairs] + [p[1].constant for p in pairs]
                struct_ty = ir.LiteralStructType(types)
                return ir.Constant(struct_ty, consts)

            return ir.Constant(ir.LiteralStructType([]), [])

        if ctx.name():
            var = ctx.name().getText()
            sym = self.current_scope.lookup(var)

            if not sym or not hasattr(sym, "llvm_ref"):
                raise NameError(f"Undefined reference to '{var}'")
            return self.builder.load(sym.llvm_ref, name=self.tmp(f"load_{var}"))

        if token := ctx.getToken(SereParser.NUMBER, 0):
            val = int(token.getText())
            return ir.Constant(self.to_llvm_type("int"), val)

        if ctx.STRING():
            parts = [s.getText().strip('"') for s in ctx.STRING()]
            joined = "".join(parts)
            arr = ir.Constant(
                ir.ArrayType(ir.IntType(8), len(joined) + 1),
                bytearray(joined.encode("utf8") + b"\0"),
            )
            gv = ir.GlobalVariable(self.module, arr.type, name=f"strtmp_{len(self.module.global_values)}")  # type: ignore
            gv.global_constant = True  # type: ignore
            gv.initializer = arr  # type: ignore
            return self.builder.bitcast(gv, ir.IntType(8).as_pointer())

        elif text0 == "...":
            return ir.Constant(ir.IntType(32), 0)

        raise NotImplementedError(f"Unhandled atom: '{text0}'.")

    def _create_string_literal(self, string):
        str_val = bytearray(string.encode("utf8")) + b"\00"
        const_array = ir.Constant(ir.ArrayType(ir.IntType(8), len(str_val)), str_val)
        global_var = ir.GlobalVariable(self.module, const_array.type, name="strtmp")  # type: ignore
        global_var.linkage = "internal"  # type: ignore
        global_var.global_constant = True  # type: ignore
        global_var.initializer = const_array  # type: ignore
        ptr = self.builder.bitcast(
            global_var, ir.IntType(8).as_pointer(), name="tmpcast"
        )
        return ptr
