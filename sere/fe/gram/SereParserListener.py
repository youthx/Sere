# Generated from sere/fe/gram/SereParser.g4 by ANTLR 4.13.2
from antlr4 import *

if "." in __name__:
    from .SereParser import SereParser
else:
    from SereParser import SereParser


# This class defines a complete listener for a parse tree produced by SereParser.
class SereParserListener(ParseTreeListener):
    # Enter a parse tree produced by SereParser#single_input.
    def enterSingle_input(self, ctx: SereParser.Single_inputContext):
        pass

    # Exit a parse tree produced by SereParser#single_input.
    def exitSingle_input(self, ctx: SereParser.Single_inputContext):
        pass

    # Enter a parse tree produced by SereParser#file_input.
    def enterFile_input(self, ctx: SereParser.File_inputContext):
        pass

    # Exit a parse tree produced by SereParser#file_input.
    def exitFile_input(self, ctx: SereParser.File_inputContext):
        pass

    # Enter a parse tree produced by SereParser#eval_input.
    def enterEval_input(self, ctx: SereParser.Eval_inputContext):
        pass

    # Exit a parse tree produced by SereParser#eval_input.
    def exitEval_input(self, ctx: SereParser.Eval_inputContext):
        pass

    # Enter a parse tree produced by SereParser#decorator.
    def enterDecorator(self, ctx: SereParser.DecoratorContext):
        pass

    # Exit a parse tree produced by SereParser#decorator.
    def exitDecorator(self, ctx: SereParser.DecoratorContext):
        pass

    # Enter a parse tree produced by SereParser#decorators.
    def enterDecorators(self, ctx: SereParser.DecoratorsContext):
        pass

    # Exit a parse tree produced by SereParser#decorators.
    def exitDecorators(self, ctx: SereParser.DecoratorsContext):
        pass

    # Enter a parse tree produced by SereParser#decorated.
    def enterDecorated(self, ctx: SereParser.DecoratedContext):
        pass

    # Exit a parse tree produced by SereParser#decorated.
    def exitDecorated(self, ctx: SereParser.DecoratedContext):
        pass

    # Enter a parse tree produced by SereParser#async_funcdef.
    def enterAsync_funcdef(self, ctx: SereParser.Async_funcdefContext):
        pass

    # Exit a parse tree produced by SereParser#async_funcdef.
    def exitAsync_funcdef(self, ctx: SereParser.Async_funcdefContext):
        pass

    # Enter a parse tree produced by SereParser#funcdef.
    def enterFuncdef(self, ctx: SereParser.FuncdefContext):
        pass

    # Exit a parse tree produced by SereParser#funcdef.
    def exitFuncdef(self, ctx: SereParser.FuncdefContext):
        pass

    # Enter a parse tree produced by SereParser#parameters.
    def enterParameters(self, ctx: SereParser.ParametersContext):
        pass

    # Exit a parse tree produced by SereParser#parameters.
    def exitParameters(self, ctx: SereParser.ParametersContext):
        pass

    # Enter a parse tree produced by SereParser#typedargslist.
    def enterTypedargslist(self, ctx: SereParser.TypedargslistContext):
        pass

    # Exit a parse tree produced by SereParser#typedargslist.
    def exitTypedargslist(self, ctx: SereParser.TypedargslistContext):
        pass

    # Enter a parse tree produced by SereParser#tfpdef.
    def enterTfpdef(self, ctx: SereParser.TfpdefContext):
        pass

    # Exit a parse tree produced by SereParser#tfpdef.
    def exitTfpdef(self, ctx: SereParser.TfpdefContext):
        pass

    # Enter a parse tree produced by SereParser#varargslist.
    def enterVarargslist(self, ctx: SereParser.VarargslistContext):
        pass

    # Exit a parse tree produced by SereParser#varargslist.
    def exitVarargslist(self, ctx: SereParser.VarargslistContext):
        pass

    # Enter a parse tree produced by SereParser#vfpdef.
    def enterVfpdef(self, ctx: SereParser.VfpdefContext):
        pass

    # Exit a parse tree produced by SereParser#vfpdef.
    def exitVfpdef(self, ctx: SereParser.VfpdefContext):
        pass

    # Enter a parse tree produced by SereParser#stmt.
    def enterStmt(self, ctx: SereParser.StmtContext):
        pass

    # Exit a parse tree produced by SereParser#stmt.
    def exitStmt(self, ctx: SereParser.StmtContext):
        pass

    # Enter a parse tree produced by SereParser#simple_stmts.
    def enterSimple_stmts(self, ctx: SereParser.Simple_stmtsContext):
        pass

    # Exit a parse tree produced by SereParser#simple_stmts.
    def exitSimple_stmts(self, ctx: SereParser.Simple_stmtsContext):
        pass

    # Enter a parse tree produced by SereParser#simple_stmt.
    def enterSimple_stmt(self, ctx: SereParser.Simple_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#simple_stmt.
    def exitSimple_stmt(self, ctx: SereParser.Simple_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#expr_stmt.
    def enterExpr_stmt(self, ctx: SereParser.Expr_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#expr_stmt.
    def exitExpr_stmt(self, ctx: SereParser.Expr_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#annassign.
    def enterAnnassign(self, ctx: SereParser.AnnassignContext):
        pass

    # Exit a parse tree produced by SereParser#annassign.
    def exitAnnassign(self, ctx: SereParser.AnnassignContext):
        pass

    # Enter a parse tree produced by SereParser#testlist_star_expr.
    def enterTestlist_star_expr(self, ctx: SereParser.Testlist_star_exprContext):
        pass

    # Exit a parse tree produced by SereParser#testlist_star_expr.
    def exitTestlist_star_expr(self, ctx: SereParser.Testlist_star_exprContext):
        pass

    # Enter a parse tree produced by SereParser#augassign.
    def enterAugassign(self, ctx: SereParser.AugassignContext):
        pass

    # Exit a parse tree produced by SereParser#augassign.
    def exitAugassign(self, ctx: SereParser.AugassignContext):
        pass

    # Enter a parse tree produced by SereParser#del_stmt.
    def enterDel_stmt(self, ctx: SereParser.Del_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#del_stmt.
    def exitDel_stmt(self, ctx: SereParser.Del_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#pass_stmt.
    def enterPass_stmt(self, ctx: SereParser.Pass_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#pass_stmt.
    def exitPass_stmt(self, ctx: SereParser.Pass_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#flow_stmt.
    def enterFlow_stmt(self, ctx: SereParser.Flow_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#flow_stmt.
    def exitFlow_stmt(self, ctx: SereParser.Flow_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#break_stmt.
    def enterBreak_stmt(self, ctx: SereParser.Break_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#break_stmt.
    def exitBreak_stmt(self, ctx: SereParser.Break_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#continue_stmt.
    def enterContinue_stmt(self, ctx: SereParser.Continue_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#continue_stmt.
    def exitContinue_stmt(self, ctx: SereParser.Continue_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#return_stmt.
    def enterReturn_stmt(self, ctx: SereParser.Return_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#return_stmt.
    def exitReturn_stmt(self, ctx: SereParser.Return_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#yield_stmt.
    def enterYield_stmt(self, ctx: SereParser.Yield_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#yield_stmt.
    def exitYield_stmt(self, ctx: SereParser.Yield_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#raise_stmt.
    def enterRaise_stmt(self, ctx: SereParser.Raise_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#raise_stmt.
    def exitRaise_stmt(self, ctx: SereParser.Raise_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#import_stmt.
    def enterImport_stmt(self, ctx: SereParser.Import_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#import_stmt.
    def exitImport_stmt(self, ctx: SereParser.Import_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#import_name.
    def enterImport_name(self, ctx: SereParser.Import_nameContext):
        pass

    # Exit a parse tree produced by SereParser#import_name.
    def exitImport_name(self, ctx: SereParser.Import_nameContext):
        pass

    # Enter a parse tree produced by SereParser#import_from.
    def enterImport_from(self, ctx: SereParser.Import_fromContext):
        pass

    # Exit a parse tree produced by SereParser#import_from.
    def exitImport_from(self, ctx: SereParser.Import_fromContext):
        pass

    # Enter a parse tree produced by SereParser#import_as_name.
    def enterImport_as_name(self, ctx: SereParser.Import_as_nameContext):
        pass

    # Exit a parse tree produced by SereParser#import_as_name.
    def exitImport_as_name(self, ctx: SereParser.Import_as_nameContext):
        pass

    # Enter a parse tree produced by SereParser#dotted_as_name.
    def enterDotted_as_name(self, ctx: SereParser.Dotted_as_nameContext):
        pass

    # Exit a parse tree produced by SereParser#dotted_as_name.
    def exitDotted_as_name(self, ctx: SereParser.Dotted_as_nameContext):
        pass

    # Enter a parse tree produced by SereParser#import_as_names.
    def enterImport_as_names(self, ctx: SereParser.Import_as_namesContext):
        pass

    # Exit a parse tree produced by SereParser#import_as_names.
    def exitImport_as_names(self, ctx: SereParser.Import_as_namesContext):
        pass

    # Enter a parse tree produced by SereParser#dotted_as_names.
    def enterDotted_as_names(self, ctx: SereParser.Dotted_as_namesContext):
        pass

    # Exit a parse tree produced by SereParser#dotted_as_names.
    def exitDotted_as_names(self, ctx: SereParser.Dotted_as_namesContext):
        pass

    # Enter a parse tree produced by SereParser#dotted_name.
    def enterDotted_name(self, ctx: SereParser.Dotted_nameContext):
        pass

    # Exit a parse tree produced by SereParser#dotted_name.
    def exitDotted_name(self, ctx: SereParser.Dotted_nameContext):
        pass

    # Enter a parse tree produced by SereParser#global_stmt.
    def enterGlobal_stmt(self, ctx: SereParser.Global_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#global_stmt.
    def exitGlobal_stmt(self, ctx: SereParser.Global_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#nonlocal_stmt.
    def enterNonlocal_stmt(self, ctx: SereParser.Nonlocal_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#nonlocal_stmt.
    def exitNonlocal_stmt(self, ctx: SereParser.Nonlocal_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#assert_stmt.
    def enterAssert_stmt(self, ctx: SereParser.Assert_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#assert_stmt.
    def exitAssert_stmt(self, ctx: SereParser.Assert_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#compound_stmt.
    def enterCompound_stmt(self, ctx: SereParser.Compound_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#compound_stmt.
    def exitCompound_stmt(self, ctx: SereParser.Compound_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#async_stmt.
    def enterAsync_stmt(self, ctx: SereParser.Async_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#async_stmt.
    def exitAsync_stmt(self, ctx: SereParser.Async_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#if_stmt.
    def enterIf_stmt(self, ctx: SereParser.If_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#if_stmt.
    def exitIf_stmt(self, ctx: SereParser.If_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#while_stmt.
    def enterWhile_stmt(self, ctx: SereParser.While_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#while_stmt.
    def exitWhile_stmt(self, ctx: SereParser.While_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#for_stmt.
    def enterFor_stmt(self, ctx: SereParser.For_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#for_stmt.
    def exitFor_stmt(self, ctx: SereParser.For_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#try_stmt.
    def enterTry_stmt(self, ctx: SereParser.Try_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#try_stmt.
    def exitTry_stmt(self, ctx: SereParser.Try_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#with_stmt.
    def enterWith_stmt(self, ctx: SereParser.With_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#with_stmt.
    def exitWith_stmt(self, ctx: SereParser.With_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#with_item.
    def enterWith_item(self, ctx: SereParser.With_itemContext):
        pass

    # Exit a parse tree produced by SereParser#with_item.
    def exitWith_item(self, ctx: SereParser.With_itemContext):
        pass

    # Enter a parse tree produced by SereParser#except_clause.
    def enterExcept_clause(self, ctx: SereParser.Except_clauseContext):
        pass

    # Exit a parse tree produced by SereParser#except_clause.
    def exitExcept_clause(self, ctx: SereParser.Except_clauseContext):
        pass

    # Enter a parse tree produced by SereParser#block.
    def enterBlock(self, ctx: SereParser.BlockContext):
        pass

    # Exit a parse tree produced by SereParser#block.
    def exitBlock(self, ctx: SereParser.BlockContext):
        pass

    # Enter a parse tree produced by SereParser#match_stmt.
    def enterMatch_stmt(self, ctx: SereParser.Match_stmtContext):
        pass

    # Exit a parse tree produced by SereParser#match_stmt.
    def exitMatch_stmt(self, ctx: SereParser.Match_stmtContext):
        pass

    # Enter a parse tree produced by SereParser#subject_expr.
    def enterSubject_expr(self, ctx: SereParser.Subject_exprContext):
        pass

    # Exit a parse tree produced by SereParser#subject_expr.
    def exitSubject_expr(self, ctx: SereParser.Subject_exprContext):
        pass

    # Enter a parse tree produced by SereParser#star_named_expressions.
    def enterStar_named_expressions(
        self, ctx: SereParser.Star_named_expressionsContext
    ):
        pass

    # Exit a parse tree produced by SereParser#star_named_expressions.
    def exitStar_named_expressions(self, ctx: SereParser.Star_named_expressionsContext):
        pass

    # Enter a parse tree produced by SereParser#star_named_expression.
    def enterStar_named_expression(self, ctx: SereParser.Star_named_expressionContext):
        pass

    # Exit a parse tree produced by SereParser#star_named_expression.
    def exitStar_named_expression(self, ctx: SereParser.Star_named_expressionContext):
        pass

    # Enter a parse tree produced by SereParser#case_block.
    def enterCase_block(self, ctx: SereParser.Case_blockContext):
        pass

    # Exit a parse tree produced by SereParser#case_block.
    def exitCase_block(self, ctx: SereParser.Case_blockContext):
        pass

    # Enter a parse tree produced by SereParser#guard.
    def enterGuard(self, ctx: SereParser.GuardContext):
        pass

    # Exit a parse tree produced by SereParser#guard.
    def exitGuard(self, ctx: SereParser.GuardContext):
        pass

    # Enter a parse tree produced by SereParser#patterns.
    def enterPatterns(self, ctx: SereParser.PatternsContext):
        pass

    # Exit a parse tree produced by SereParser#patterns.
    def exitPatterns(self, ctx: SereParser.PatternsContext):
        pass

    # Enter a parse tree produced by SereParser#pattern.
    def enterPattern(self, ctx: SereParser.PatternContext):
        pass

    # Exit a parse tree produced by SereParser#pattern.
    def exitPattern(self, ctx: SereParser.PatternContext):
        pass

    # Enter a parse tree produced by SereParser#as_pattern.
    def enterAs_pattern(self, ctx: SereParser.As_patternContext):
        pass

    # Exit a parse tree produced by SereParser#as_pattern.
    def exitAs_pattern(self, ctx: SereParser.As_patternContext):
        pass

    # Enter a parse tree produced by SereParser#or_pattern.
    def enterOr_pattern(self, ctx: SereParser.Or_patternContext):
        pass

    # Exit a parse tree produced by SereParser#or_pattern.
    def exitOr_pattern(self, ctx: SereParser.Or_patternContext):
        pass

    # Enter a parse tree produced by SereParser#closed_pattern.
    def enterClosed_pattern(self, ctx: SereParser.Closed_patternContext):
        pass

    # Exit a parse tree produced by SereParser#closed_pattern.
    def exitClosed_pattern(self, ctx: SereParser.Closed_patternContext):
        pass

    # Enter a parse tree produced by SereParser#literal_pattern.
    def enterLiteral_pattern(self, ctx: SereParser.Literal_patternContext):
        pass

    # Exit a parse tree produced by SereParser#literal_pattern.
    def exitLiteral_pattern(self, ctx: SereParser.Literal_patternContext):
        pass

    # Enter a parse tree produced by SereParser#literal_expr.
    def enterLiteral_expr(self, ctx: SereParser.Literal_exprContext):
        pass

    # Exit a parse tree produced by SereParser#literal_expr.
    def exitLiteral_expr(self, ctx: SereParser.Literal_exprContext):
        pass

    # Enter a parse tree produced by SereParser#complex_number.
    def enterComplex_number(self, ctx: SereParser.Complex_numberContext):
        pass

    # Exit a parse tree produced by SereParser#complex_number.
    def exitComplex_number(self, ctx: SereParser.Complex_numberContext):
        pass

    # Enter a parse tree produced by SereParser#signed_number.
    def enterSigned_number(self, ctx: SereParser.Signed_numberContext):
        pass

    # Exit a parse tree produced by SereParser#signed_number.
    def exitSigned_number(self, ctx: SereParser.Signed_numberContext):
        pass

    # Enter a parse tree produced by SereParser#signed_real_number.
    def enterSigned_real_number(self, ctx: SereParser.Signed_real_numberContext):
        pass

    # Exit a parse tree produced by SereParser#signed_real_number.
    def exitSigned_real_number(self, ctx: SereParser.Signed_real_numberContext):
        pass

    # Enter a parse tree produced by SereParser#real_number.
    def enterReal_number(self, ctx: SereParser.Real_numberContext):
        pass

    # Exit a parse tree produced by SereParser#real_number.
    def exitReal_number(self, ctx: SereParser.Real_numberContext):
        pass

    # Enter a parse tree produced by SereParser#imaginary_number.
    def enterImaginary_number(self, ctx: SereParser.Imaginary_numberContext):
        pass

    # Exit a parse tree produced by SereParser#imaginary_number.
    def exitImaginary_number(self, ctx: SereParser.Imaginary_numberContext):
        pass

    # Enter a parse tree produced by SereParser#capture_pattern.
    def enterCapture_pattern(self, ctx: SereParser.Capture_patternContext):
        pass

    # Exit a parse tree produced by SereParser#capture_pattern.
    def exitCapture_pattern(self, ctx: SereParser.Capture_patternContext):
        pass

    # Enter a parse tree produced by SereParser#pattern_capture_target.
    def enterPattern_capture_target(
        self, ctx: SereParser.Pattern_capture_targetContext
    ):
        pass

    # Exit a parse tree produced by SereParser#pattern_capture_target.
    def exitPattern_capture_target(self, ctx: SereParser.Pattern_capture_targetContext):
        pass

    # Enter a parse tree produced by SereParser#wildcard_pattern.
    def enterWildcard_pattern(self, ctx: SereParser.Wildcard_patternContext):
        pass

    # Exit a parse tree produced by SereParser#wildcard_pattern.
    def exitWildcard_pattern(self, ctx: SereParser.Wildcard_patternContext):
        pass

    # Enter a parse tree produced by SereParser#value_pattern.
    def enterValue_pattern(self, ctx: SereParser.Value_patternContext):
        pass

    # Exit a parse tree produced by SereParser#value_pattern.
    def exitValue_pattern(self, ctx: SereParser.Value_patternContext):
        pass

    # Enter a parse tree produced by SereParser#attr.
    def enterAttr(self, ctx: SereParser.AttrContext):
        pass

    # Exit a parse tree produced by SereParser#attr.
    def exitAttr(self, ctx: SereParser.AttrContext):
        pass

    # Enter a parse tree produced by SereParser#name_or_attr.
    def enterName_or_attr(self, ctx: SereParser.Name_or_attrContext):
        pass

    # Exit a parse tree produced by SereParser#name_or_attr.
    def exitName_or_attr(self, ctx: SereParser.Name_or_attrContext):
        pass

    # Enter a parse tree produced by SereParser#group_pattern.
    def enterGroup_pattern(self, ctx: SereParser.Group_patternContext):
        pass

    # Exit a parse tree produced by SereParser#group_pattern.
    def exitGroup_pattern(self, ctx: SereParser.Group_patternContext):
        pass

    # Enter a parse tree produced by SereParser#sequence_pattern.
    def enterSequence_pattern(self, ctx: SereParser.Sequence_patternContext):
        pass

    # Exit a parse tree produced by SereParser#sequence_pattern.
    def exitSequence_pattern(self, ctx: SereParser.Sequence_patternContext):
        pass

    # Enter a parse tree produced by SereParser#open_sequence_pattern.
    def enterOpen_sequence_pattern(self, ctx: SereParser.Open_sequence_patternContext):
        pass

    # Exit a parse tree produced by SereParser#open_sequence_pattern.
    def exitOpen_sequence_pattern(self, ctx: SereParser.Open_sequence_patternContext):
        pass

    # Enter a parse tree produced by SereParser#maybe_sequence_pattern.
    def enterMaybe_sequence_pattern(
        self, ctx: SereParser.Maybe_sequence_patternContext
    ):
        pass

    # Exit a parse tree produced by SereParser#maybe_sequence_pattern.
    def exitMaybe_sequence_pattern(self, ctx: SereParser.Maybe_sequence_patternContext):
        pass

    # Enter a parse tree produced by SereParser#maybe_star_pattern.
    def enterMaybe_star_pattern(self, ctx: SereParser.Maybe_star_patternContext):
        pass

    # Exit a parse tree produced by SereParser#maybe_star_pattern.
    def exitMaybe_star_pattern(self, ctx: SereParser.Maybe_star_patternContext):
        pass

    # Enter a parse tree produced by SereParser#star_pattern.
    def enterStar_pattern(self, ctx: SereParser.Star_patternContext):
        pass

    # Exit a parse tree produced by SereParser#star_pattern.
    def exitStar_pattern(self, ctx: SereParser.Star_patternContext):
        pass

    # Enter a parse tree produced by SereParser#mapping_pattern.
    def enterMapping_pattern(self, ctx: SereParser.Mapping_patternContext):
        pass

    # Exit a parse tree produced by SereParser#mapping_pattern.
    def exitMapping_pattern(self, ctx: SereParser.Mapping_patternContext):
        pass

    # Enter a parse tree produced by SereParser#items_pattern.
    def enterItems_pattern(self, ctx: SereParser.Items_patternContext):
        pass

    # Exit a parse tree produced by SereParser#items_pattern.
    def exitItems_pattern(self, ctx: SereParser.Items_patternContext):
        pass

    # Enter a parse tree produced by SereParser#key_value_pattern.
    def enterKey_value_pattern(self, ctx: SereParser.Key_value_patternContext):
        pass

    # Exit a parse tree produced by SereParser#key_value_pattern.
    def exitKey_value_pattern(self, ctx: SereParser.Key_value_patternContext):
        pass

    # Enter a parse tree produced by SereParser#double_star_pattern.
    def enterDouble_star_pattern(self, ctx: SereParser.Double_star_patternContext):
        pass

    # Exit a parse tree produced by SereParser#double_star_pattern.
    def exitDouble_star_pattern(self, ctx: SereParser.Double_star_patternContext):
        pass

    # Enter a parse tree produced by SereParser#class_pattern.
    def enterClass_pattern(self, ctx: SereParser.Class_patternContext):
        pass

    # Exit a parse tree produced by SereParser#class_pattern.
    def exitClass_pattern(self, ctx: SereParser.Class_patternContext):
        pass

    # Enter a parse tree produced by SereParser#positional_patterns.
    def enterPositional_patterns(self, ctx: SereParser.Positional_patternsContext):
        pass

    # Exit a parse tree produced by SereParser#positional_patterns.
    def exitPositional_patterns(self, ctx: SereParser.Positional_patternsContext):
        pass

    # Enter a parse tree produced by SereParser#keyword_patterns.
    def enterKeyword_patterns(self, ctx: SereParser.Keyword_patternsContext):
        pass

    # Exit a parse tree produced by SereParser#keyword_patterns.
    def exitKeyword_patterns(self, ctx: SereParser.Keyword_patternsContext):
        pass

    # Enter a parse tree produced by SereParser#keyword_pattern.
    def enterKeyword_pattern(self, ctx: SereParser.Keyword_patternContext):
        pass

    # Exit a parse tree produced by SereParser#keyword_pattern.
    def exitKeyword_pattern(self, ctx: SereParser.Keyword_patternContext):
        pass

    # Enter a parse tree produced by SereParser#test.
    def enterTest(self, ctx: SereParser.TestContext):
        pass

    # Exit a parse tree produced by SereParser#test.
    def exitTest(self, ctx: SereParser.TestContext):
        pass

    # Enter a parse tree produced by SereParser#test_nocond.
    def enterTest_nocond(self, ctx: SereParser.Test_nocondContext):
        pass

    # Exit a parse tree produced by SereParser#test_nocond.
    def exitTest_nocond(self, ctx: SereParser.Test_nocondContext):
        pass

    # Enter a parse tree produced by SereParser#lambdef.
    def enterLambdef(self, ctx: SereParser.LambdefContext):
        pass

    # Exit a parse tree produced by SereParser#lambdef.
    def exitLambdef(self, ctx: SereParser.LambdefContext):
        pass

    # Enter a parse tree produced by SereParser#lambdef_nocond.
    def enterLambdef_nocond(self, ctx: SereParser.Lambdef_nocondContext):
        pass

    # Exit a parse tree produced by SereParser#lambdef_nocond.
    def exitLambdef_nocond(self, ctx: SereParser.Lambdef_nocondContext):
        pass

    # Enter a parse tree produced by SereParser#or_test.
    def enterOr_test(self, ctx: SereParser.Or_testContext):
        pass

    # Exit a parse tree produced by SereParser#or_test.
    def exitOr_test(self, ctx: SereParser.Or_testContext):
        pass

    # Enter a parse tree produced by SereParser#and_test.
    def enterAnd_test(self, ctx: SereParser.And_testContext):
        pass

    # Exit a parse tree produced by SereParser#and_test.
    def exitAnd_test(self, ctx: SereParser.And_testContext):
        pass

    # Enter a parse tree produced by SereParser#not_test.
    def enterNot_test(self, ctx: SereParser.Not_testContext):
        pass

    # Exit a parse tree produced by SereParser#not_test.
    def exitNot_test(self, ctx: SereParser.Not_testContext):
        pass

    # Enter a parse tree produced by SereParser#comparison.
    def enterComparison(self, ctx: SereParser.ComparisonContext):
        pass

    # Exit a parse tree produced by SereParser#comparison.
    def exitComparison(self, ctx: SereParser.ComparisonContext):
        pass

    # Enter a parse tree produced by SereParser#comp_op.
    def enterComp_op(self, ctx: SereParser.Comp_opContext):
        pass

    # Exit a parse tree produced by SereParser#comp_op.
    def exitComp_op(self, ctx: SereParser.Comp_opContext):
        pass

    # Enter a parse tree produced by SereParser#star_expr.
    def enterStar_expr(self, ctx: SereParser.Star_exprContext):
        pass

    # Exit a parse tree produced by SereParser#star_expr.
    def exitStar_expr(self, ctx: SereParser.Star_exprContext):
        pass

    # Enter a parse tree produced by SereParser#expr.
    def enterExpr(self, ctx: SereParser.ExprContext):
        pass

    # Exit a parse tree produced by SereParser#expr.
    def exitExpr(self, ctx: SereParser.ExprContext):
        pass

    # Enter a parse tree produced by SereParser#xor_expr.
    def enterXor_expr(self, ctx: SereParser.Xor_exprContext):
        pass

    # Exit a parse tree produced by SereParser#xor_expr.
    def exitXor_expr(self, ctx: SereParser.Xor_exprContext):
        pass

    # Enter a parse tree produced by SereParser#and_expr.
    def enterAnd_expr(self, ctx: SereParser.And_exprContext):
        pass

    # Exit a parse tree produced by SereParser#and_expr.
    def exitAnd_expr(self, ctx: SereParser.And_exprContext):
        pass

    # Enter a parse tree produced by SereParser#shift_expr.
    def enterShift_expr(self, ctx: SereParser.Shift_exprContext):
        pass

    # Exit a parse tree produced by SereParser#shift_expr.
    def exitShift_expr(self, ctx: SereParser.Shift_exprContext):
        pass

    # Enter a parse tree produced by SereParser#arith_expr.
    def enterArith_expr(self, ctx: SereParser.Arith_exprContext):
        pass

    # Exit a parse tree produced by SereParser#arith_expr.
    def exitArith_expr(self, ctx: SereParser.Arith_exprContext):
        pass

    # Enter a parse tree produced by SereParser#term.
    def enterTerm(self, ctx: SereParser.TermContext):
        pass

    # Exit a parse tree produced by SereParser#term.
    def exitTerm(self, ctx: SereParser.TermContext):
        pass

    # Enter a parse tree produced by SereParser#factor.
    def enterFactor(self, ctx: SereParser.FactorContext):
        pass

    # Exit a parse tree produced by SereParser#factor.
    def exitFactor(self, ctx: SereParser.FactorContext):
        pass

    # Enter a parse tree produced by SereParser#power.
    def enterPower(self, ctx: SereParser.PowerContext):
        pass

    # Exit a parse tree produced by SereParser#power.
    def exitPower(self, ctx: SereParser.PowerContext):
        pass

    # Enter a parse tree produced by SereParser#atom_expr.
    def enterAtom_expr(self, ctx: SereParser.Atom_exprContext):
        pass

    # Exit a parse tree produced by SereParser#atom_expr.
    def exitAtom_expr(self, ctx: SereParser.Atom_exprContext):
        pass

    # Enter a parse tree produced by SereParser#atom.
    def enterAtom(self, ctx: SereParser.AtomContext):
        pass

    # Exit a parse tree produced by SereParser#atom.
    def exitAtom(self, ctx: SereParser.AtomContext):
        pass

    # Enter a parse tree produced by SereParser#name.
    def enterName(self, ctx: SereParser.NameContext):
        pass

    # Exit a parse tree produced by SereParser#name.
    def exitName(self, ctx: SereParser.NameContext):
        pass

    # Enter a parse tree produced by SereParser#testlist_comp.
    def enterTestlist_comp(self, ctx: SereParser.Testlist_compContext):
        pass

    # Exit a parse tree produced by SereParser#testlist_comp.
    def exitTestlist_comp(self, ctx: SereParser.Testlist_compContext):
        pass

    # Enter a parse tree produced by SereParser#trailer.
    def enterTrailer(self, ctx: SereParser.TrailerContext):
        pass

    # Exit a parse tree produced by SereParser#trailer.
    def exitTrailer(self, ctx: SereParser.TrailerContext):
        pass

    # Enter a parse tree produced by SereParser#subscriptlist.
    def enterSubscriptlist(self, ctx: SereParser.SubscriptlistContext):
        pass

    # Exit a parse tree produced by SereParser#subscriptlist.
    def exitSubscriptlist(self, ctx: SereParser.SubscriptlistContext):
        pass

    # Enter a parse tree produced by SereParser#subscript_.
    def enterSubscript_(self, ctx: SereParser.Subscript_Context):
        pass

    # Exit a parse tree produced by SereParser#subscript_.
    def exitSubscript_(self, ctx: SereParser.Subscript_Context):
        pass

    # Enter a parse tree produced by SereParser#sliceop.
    def enterSliceop(self, ctx: SereParser.SliceopContext):
        pass

    # Exit a parse tree produced by SereParser#sliceop.
    def exitSliceop(self, ctx: SereParser.SliceopContext):
        pass

    # Enter a parse tree produced by SereParser#exprlist.
    def enterExprlist(self, ctx: SereParser.ExprlistContext):
        pass

    # Exit a parse tree produced by SereParser#exprlist.
    def exitExprlist(self, ctx: SereParser.ExprlistContext):
        pass

    # Enter a parse tree produced by SereParser#testlist.
    def enterTestlist(self, ctx: SereParser.TestlistContext):
        pass

    # Exit a parse tree produced by SereParser#testlist.
    def exitTestlist(self, ctx: SereParser.TestlistContext):
        pass

    # Enter a parse tree produced by SereParser#dictorsetmaker.
    def enterDictorsetmaker(self, ctx: SereParser.DictorsetmakerContext):
        pass

    # Exit a parse tree produced by SereParser#dictorsetmaker.
    def exitDictorsetmaker(self, ctx: SereParser.DictorsetmakerContext):
        pass

    # Enter a parse tree produced by SereParser#classdef.
    def enterClassdef(self, ctx: SereParser.ClassdefContext):
        pass

    # Exit a parse tree produced by SereParser#classdef.
    def exitClassdef(self, ctx: SereParser.ClassdefContext):
        pass

    # Enter a parse tree produced by SereParser#arglist.
    def enterArglist(self, ctx: SereParser.ArglistContext):
        pass

    # Exit a parse tree produced by SereParser#arglist.
    def exitArglist(self, ctx: SereParser.ArglistContext):
        pass

    # Enter a parse tree produced by SereParser#argument.
    def enterArgument(self, ctx: SereParser.ArgumentContext):
        pass

    # Exit a parse tree produced by SereParser#argument.
    def exitArgument(self, ctx: SereParser.ArgumentContext):
        pass

    # Enter a parse tree produced by SereParser#comp_iter.
    def enterComp_iter(self, ctx: SereParser.Comp_iterContext):
        pass

    # Exit a parse tree produced by SereParser#comp_iter.
    def exitComp_iter(self, ctx: SereParser.Comp_iterContext):
        pass

    # Enter a parse tree produced by SereParser#comp_for.
    def enterComp_for(self, ctx: SereParser.Comp_forContext):
        pass

    # Exit a parse tree produced by SereParser#comp_for.
    def exitComp_for(self, ctx: SereParser.Comp_forContext):
        pass

    # Enter a parse tree produced by SereParser#comp_if.
    def enterComp_if(self, ctx: SereParser.Comp_ifContext):
        pass

    # Exit a parse tree produced by SereParser#comp_if.
    def exitComp_if(self, ctx: SereParser.Comp_ifContext):
        pass

    # Enter a parse tree produced by SereParser#encoding_decl.
    def enterEncoding_decl(self, ctx: SereParser.Encoding_declContext):
        pass

    # Exit a parse tree produced by SereParser#encoding_decl.
    def exitEncoding_decl(self, ctx: SereParser.Encoding_declContext):
        pass

    # Enter a parse tree produced by SereParser#yield_expr.
    def enterYield_expr(self, ctx: SereParser.Yield_exprContext):
        pass

    # Exit a parse tree produced by SereParser#yield_expr.
    def exitYield_expr(self, ctx: SereParser.Yield_exprContext):
        pass

    # Enter a parse tree produced by SereParser#yield_arg.
    def enterYield_arg(self, ctx: SereParser.Yield_argContext):
        pass

    # Exit a parse tree produced by SereParser#yield_arg.
    def exitYield_arg(self, ctx: SereParser.Yield_argContext):
        pass

    # Enter a parse tree produced by SereParser#strings.
    def enterStrings(self, ctx: SereParser.StringsContext):
        pass

    # Exit a parse tree produced by SereParser#strings.
    def exitStrings(self, ctx: SereParser.StringsContext):
        pass


del SereParser
