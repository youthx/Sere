# Generated from sere/fe/gram/SereParser.g4 by ANTLR 4.13.2
from antlr4 import *
if "." in __name__:
    from .SereParser import SereParser
else:
    from SereParser import SereParser

# This class defines a complete generic visitor for a parse tree produced by SereParser.

class SereParserVisitor(ParseTreeVisitor):

    # Visit a parse tree produced by SereParser#single_input.
    def visitSingle_input(self, ctx:SereParser.Single_inputContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#file_input.
    def visitFile_input(self, ctx:SereParser.File_inputContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#eval_input.
    def visitEval_input(self, ctx:SereParser.Eval_inputContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#decorator.
    def visitDecorator(self, ctx:SereParser.DecoratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#decorators.
    def visitDecorators(self, ctx:SereParser.DecoratorsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#decorated.
    def visitDecorated(self, ctx:SereParser.DecoratedContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#async_funcdef.
    def visitAsync_funcdef(self, ctx:SereParser.Async_funcdefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#funcdef.
    def visitFuncdef(self, ctx:SereParser.FuncdefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#parameters.
    def visitParameters(self, ctx:SereParser.ParametersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#typedargslist.
    def visitTypedargslist(self, ctx:SereParser.TypedargslistContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#tfpdef.
    def visitTfpdef(self, ctx:SereParser.TfpdefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#varargslist.
    def visitVarargslist(self, ctx:SereParser.VarargslistContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#vfpdef.
    def visitVfpdef(self, ctx:SereParser.VfpdefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#stmt.
    def visitStmt(self, ctx:SereParser.StmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#simple_stmts.
    def visitSimple_stmts(self, ctx:SereParser.Simple_stmtsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#simple_stmt.
    def visitSimple_stmt(self, ctx:SereParser.Simple_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#expr_stmt.
    def visitExpr_stmt(self, ctx:SereParser.Expr_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#annassign.
    def visitAnnassign(self, ctx:SereParser.AnnassignContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#testlist_star_expr.
    def visitTestlist_star_expr(self, ctx:SereParser.Testlist_star_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#augassign.
    def visitAugassign(self, ctx:SereParser.AugassignContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#del_stmt.
    def visitDel_stmt(self, ctx:SereParser.Del_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#pass_stmt.
    def visitPass_stmt(self, ctx:SereParser.Pass_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#flow_stmt.
    def visitFlow_stmt(self, ctx:SereParser.Flow_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#break_stmt.
    def visitBreak_stmt(self, ctx:SereParser.Break_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#continue_stmt.
    def visitContinue_stmt(self, ctx:SereParser.Continue_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#return_stmt.
    def visitReturn_stmt(self, ctx:SereParser.Return_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#yield_stmt.
    def visitYield_stmt(self, ctx:SereParser.Yield_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#raise_stmt.
    def visitRaise_stmt(self, ctx:SereParser.Raise_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#import_stmt.
    def visitImport_stmt(self, ctx:SereParser.Import_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#import_name.
    def visitImport_name(self, ctx:SereParser.Import_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#import_from.
    def visitImport_from(self, ctx:SereParser.Import_fromContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#import_as_name.
    def visitImport_as_name(self, ctx:SereParser.Import_as_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#dotted_as_name.
    def visitDotted_as_name(self, ctx:SereParser.Dotted_as_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#import_as_names.
    def visitImport_as_names(self, ctx:SereParser.Import_as_namesContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#dotted_as_names.
    def visitDotted_as_names(self, ctx:SereParser.Dotted_as_namesContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#dotted_name.
    def visitDotted_name(self, ctx:SereParser.Dotted_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#global_stmt.
    def visitGlobal_stmt(self, ctx:SereParser.Global_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#nonlocal_stmt.
    def visitNonlocal_stmt(self, ctx:SereParser.Nonlocal_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#assert_stmt.
    def visitAssert_stmt(self, ctx:SereParser.Assert_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#compound_stmt.
    def visitCompound_stmt(self, ctx:SereParser.Compound_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#async_stmt.
    def visitAsync_stmt(self, ctx:SereParser.Async_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#if_stmt.
    def visitIf_stmt(self, ctx:SereParser.If_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#while_stmt.
    def visitWhile_stmt(self, ctx:SereParser.While_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#for_stmt.
    def visitFor_stmt(self, ctx:SereParser.For_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#try_stmt.
    def visitTry_stmt(self, ctx:SereParser.Try_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#with_stmt.
    def visitWith_stmt(self, ctx:SereParser.With_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#with_item.
    def visitWith_item(self, ctx:SereParser.With_itemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#except_clause.
    def visitExcept_clause(self, ctx:SereParser.Except_clauseContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#block.
    def visitBlock(self, ctx:SereParser.BlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#match_stmt.
    def visitMatch_stmt(self, ctx:SereParser.Match_stmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#subject_expr.
    def visitSubject_expr(self, ctx:SereParser.Subject_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#star_named_expressions.
    def visitStar_named_expressions(self, ctx:SereParser.Star_named_expressionsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#star_named_expression.
    def visitStar_named_expression(self, ctx:SereParser.Star_named_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#case_block.
    def visitCase_block(self, ctx:SereParser.Case_blockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#guard.
    def visitGuard(self, ctx:SereParser.GuardContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#patterns.
    def visitPatterns(self, ctx:SereParser.PatternsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#pattern.
    def visitPattern(self, ctx:SereParser.PatternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#as_pattern.
    def visitAs_pattern(self, ctx:SereParser.As_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#or_pattern.
    def visitOr_pattern(self, ctx:SereParser.Or_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#closed_pattern.
    def visitClosed_pattern(self, ctx:SereParser.Closed_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#literal_pattern.
    def visitLiteral_pattern(self, ctx:SereParser.Literal_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#literal_expr.
    def visitLiteral_expr(self, ctx:SereParser.Literal_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#complex_number.
    def visitComplex_number(self, ctx:SereParser.Complex_numberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#signed_number.
    def visitSigned_number(self, ctx:SereParser.Signed_numberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#signed_real_number.
    def visitSigned_real_number(self, ctx:SereParser.Signed_real_numberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#real_number.
    def visitReal_number(self, ctx:SereParser.Real_numberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#imaginary_number.
    def visitImaginary_number(self, ctx:SereParser.Imaginary_numberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#capture_pattern.
    def visitCapture_pattern(self, ctx:SereParser.Capture_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#pattern_capture_target.
    def visitPattern_capture_target(self, ctx:SereParser.Pattern_capture_targetContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#wildcard_pattern.
    def visitWildcard_pattern(self, ctx:SereParser.Wildcard_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#value_pattern.
    def visitValue_pattern(self, ctx:SereParser.Value_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#attr.
    def visitAttr(self, ctx:SereParser.AttrContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#name_or_attr.
    def visitName_or_attr(self, ctx:SereParser.Name_or_attrContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#group_pattern.
    def visitGroup_pattern(self, ctx:SereParser.Group_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#sequence_pattern.
    def visitSequence_pattern(self, ctx:SereParser.Sequence_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#open_sequence_pattern.
    def visitOpen_sequence_pattern(self, ctx:SereParser.Open_sequence_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#maybe_sequence_pattern.
    def visitMaybe_sequence_pattern(self, ctx:SereParser.Maybe_sequence_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#maybe_star_pattern.
    def visitMaybe_star_pattern(self, ctx:SereParser.Maybe_star_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#star_pattern.
    def visitStar_pattern(self, ctx:SereParser.Star_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#mapping_pattern.
    def visitMapping_pattern(self, ctx:SereParser.Mapping_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#items_pattern.
    def visitItems_pattern(self, ctx:SereParser.Items_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#key_value_pattern.
    def visitKey_value_pattern(self, ctx:SereParser.Key_value_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#double_star_pattern.
    def visitDouble_star_pattern(self, ctx:SereParser.Double_star_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#class_pattern.
    def visitClass_pattern(self, ctx:SereParser.Class_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#positional_patterns.
    def visitPositional_patterns(self, ctx:SereParser.Positional_patternsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#keyword_patterns.
    def visitKeyword_patterns(self, ctx:SereParser.Keyword_patternsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#keyword_pattern.
    def visitKeyword_pattern(self, ctx:SereParser.Keyword_patternContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#test.
    def visitTest(self, ctx:SereParser.TestContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#test_nocond.
    def visitTest_nocond(self, ctx:SereParser.Test_nocondContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#lambdef.
    def visitLambdef(self, ctx:SereParser.LambdefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#lambdef_nocond.
    def visitLambdef_nocond(self, ctx:SereParser.Lambdef_nocondContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#or_test.
    def visitOr_test(self, ctx:SereParser.Or_testContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#and_test.
    def visitAnd_test(self, ctx:SereParser.And_testContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#not_test.
    def visitNot_test(self, ctx:SereParser.Not_testContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#comparison.
    def visitComparison(self, ctx:SereParser.ComparisonContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#comp_op.
    def visitComp_op(self, ctx:SereParser.Comp_opContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#star_expr.
    def visitStar_expr(self, ctx:SereParser.Star_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#expr.
    def visitExpr(self, ctx:SereParser.ExprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#xor_expr.
    def visitXor_expr(self, ctx:SereParser.Xor_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#and_expr.
    def visitAnd_expr(self, ctx:SereParser.And_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#shift_expr.
    def visitShift_expr(self, ctx:SereParser.Shift_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#arith_expr.
    def visitArith_expr(self, ctx:SereParser.Arith_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#term.
    def visitTerm(self, ctx:SereParser.TermContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#factor.
    def visitFactor(self, ctx:SereParser.FactorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#power.
    def visitPower(self, ctx:SereParser.PowerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#atom_expr.
    def visitAtom_expr(self, ctx:SereParser.Atom_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#atom.
    def visitAtom(self, ctx:SereParser.AtomContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#name.
    def visitName(self, ctx:SereParser.NameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#testlist_comp.
    def visitTestlist_comp(self, ctx:SereParser.Testlist_compContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#trailer.
    def visitTrailer(self, ctx:SereParser.TrailerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#subscriptlist.
    def visitSubscriptlist(self, ctx:SereParser.SubscriptlistContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#subscript_.
    def visitSubscript_(self, ctx:SereParser.Subscript_Context):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#sliceop.
    def visitSliceop(self, ctx:SereParser.SliceopContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#exprlist.
    def visitExprlist(self, ctx:SereParser.ExprlistContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#testlist.
    def visitTestlist(self, ctx:SereParser.TestlistContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#dictorsetmaker.
    def visitDictorsetmaker(self, ctx:SereParser.DictorsetmakerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#classdef.
    def visitClassdef(self, ctx:SereParser.ClassdefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#arglist.
    def visitArglist(self, ctx:SereParser.ArglistContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#argument.
    def visitArgument(self, ctx:SereParser.ArgumentContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#comp_iter.
    def visitComp_iter(self, ctx:SereParser.Comp_iterContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#comp_for.
    def visitComp_for(self, ctx:SereParser.Comp_forContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#comp_if.
    def visitComp_if(self, ctx:SereParser.Comp_ifContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#encoding_decl.
    def visitEncoding_decl(self, ctx:SereParser.Encoding_declContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#yield_expr.
    def visitYield_expr(self, ctx:SereParser.Yield_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#yield_arg.
    def visitYield_arg(self, ctx:SereParser.Yield_argContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SereParser#strings.
    def visitStrings(self, ctx:SereParser.StringsContext):
        return self.visitChildren(ctx)



del SereParser