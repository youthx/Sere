// Generated from /workspaces/Sere/sere/fe/gram/SereParser.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link SereParser}.
 */
public interface SereParserListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link SereParser#single_input}.
	 * @param ctx the parse tree
	 */
	void enterSingle_input(SereParser.Single_inputContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#single_input}.
	 * @param ctx the parse tree
	 */
	void exitSingle_input(SereParser.Single_inputContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#file_input}.
	 * @param ctx the parse tree
	 */
	void enterFile_input(SereParser.File_inputContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#file_input}.
	 * @param ctx the parse tree
	 */
	void exitFile_input(SereParser.File_inputContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#eval_input}.
	 * @param ctx the parse tree
	 */
	void enterEval_input(SereParser.Eval_inputContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#eval_input}.
	 * @param ctx the parse tree
	 */
	void exitEval_input(SereParser.Eval_inputContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#decorator}.
	 * @param ctx the parse tree
	 */
	void enterDecorator(SereParser.DecoratorContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#decorator}.
	 * @param ctx the parse tree
	 */
	void exitDecorator(SereParser.DecoratorContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#decorators}.
	 * @param ctx the parse tree
	 */
	void enterDecorators(SereParser.DecoratorsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#decorators}.
	 * @param ctx the parse tree
	 */
	void exitDecorators(SereParser.DecoratorsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#decorated}.
	 * @param ctx the parse tree
	 */
	void enterDecorated(SereParser.DecoratedContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#decorated}.
	 * @param ctx the parse tree
	 */
	void exitDecorated(SereParser.DecoratedContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#async_funcdef}.
	 * @param ctx the parse tree
	 */
	void enterAsync_funcdef(SereParser.Async_funcdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#async_funcdef}.
	 * @param ctx the parse tree
	 */
	void exitAsync_funcdef(SereParser.Async_funcdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#funcdef}.
	 * @param ctx the parse tree
	 */
	void enterFuncdef(SereParser.FuncdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#funcdef}.
	 * @param ctx the parse tree
	 */
	void exitFuncdef(SereParser.FuncdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#parameters}.
	 * @param ctx the parse tree
	 */
	void enterParameters(SereParser.ParametersContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#parameters}.
	 * @param ctx the parse tree
	 */
	void exitParameters(SereParser.ParametersContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#typedargslist}.
	 * @param ctx the parse tree
	 */
	void enterTypedargslist(SereParser.TypedargslistContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#typedargslist}.
	 * @param ctx the parse tree
	 */
	void exitTypedargslist(SereParser.TypedargslistContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#tfpdef}.
	 * @param ctx the parse tree
	 */
	void enterTfpdef(SereParser.TfpdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#tfpdef}.
	 * @param ctx the parse tree
	 */
	void exitTfpdef(SereParser.TfpdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#varargslist}.
	 * @param ctx the parse tree
	 */
	void enterVarargslist(SereParser.VarargslistContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#varargslist}.
	 * @param ctx the parse tree
	 */
	void exitVarargslist(SereParser.VarargslistContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#vfpdef}.
	 * @param ctx the parse tree
	 */
	void enterVfpdef(SereParser.VfpdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#vfpdef}.
	 * @param ctx the parse tree
	 */
	void exitVfpdef(SereParser.VfpdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterStmt(SereParser.StmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitStmt(SereParser.StmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#simple_stmts}.
	 * @param ctx the parse tree
	 */
	void enterSimple_stmts(SereParser.Simple_stmtsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#simple_stmts}.
	 * @param ctx the parse tree
	 */
	void exitSimple_stmts(SereParser.Simple_stmtsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#simple_stmt}.
	 * @param ctx the parse tree
	 */
	void enterSimple_stmt(SereParser.Simple_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#simple_stmt}.
	 * @param ctx the parse tree
	 */
	void exitSimple_stmt(SereParser.Simple_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#expr_stmt}.
	 * @param ctx the parse tree
	 */
	void enterExpr_stmt(SereParser.Expr_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#expr_stmt}.
	 * @param ctx the parse tree
	 */
	void exitExpr_stmt(SereParser.Expr_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#annassign}.
	 * @param ctx the parse tree
	 */
	void enterAnnassign(SereParser.AnnassignContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#annassign}.
	 * @param ctx the parse tree
	 */
	void exitAnnassign(SereParser.AnnassignContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#testlist_star_expr}.
	 * @param ctx the parse tree
	 */
	void enterTestlist_star_expr(SereParser.Testlist_star_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#testlist_star_expr}.
	 * @param ctx the parse tree
	 */
	void exitTestlist_star_expr(SereParser.Testlist_star_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#augassign}.
	 * @param ctx the parse tree
	 */
	void enterAugassign(SereParser.AugassignContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#augassign}.
	 * @param ctx the parse tree
	 */
	void exitAugassign(SereParser.AugassignContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#del_stmt}.
	 * @param ctx the parse tree
	 */
	void enterDel_stmt(SereParser.Del_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#del_stmt}.
	 * @param ctx the parse tree
	 */
	void exitDel_stmt(SereParser.Del_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#pass_stmt}.
	 * @param ctx the parse tree
	 */
	void enterPass_stmt(SereParser.Pass_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#pass_stmt}.
	 * @param ctx the parse tree
	 */
	void exitPass_stmt(SereParser.Pass_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#flow_stmt}.
	 * @param ctx the parse tree
	 */
	void enterFlow_stmt(SereParser.Flow_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#flow_stmt}.
	 * @param ctx the parse tree
	 */
	void exitFlow_stmt(SereParser.Flow_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#break_stmt}.
	 * @param ctx the parse tree
	 */
	void enterBreak_stmt(SereParser.Break_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#break_stmt}.
	 * @param ctx the parse tree
	 */
	void exitBreak_stmt(SereParser.Break_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#continue_stmt}.
	 * @param ctx the parse tree
	 */
	void enterContinue_stmt(SereParser.Continue_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#continue_stmt}.
	 * @param ctx the parse tree
	 */
	void exitContinue_stmt(SereParser.Continue_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void enterReturn_stmt(SereParser.Return_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void exitReturn_stmt(SereParser.Return_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#yield_stmt}.
	 * @param ctx the parse tree
	 */
	void enterYield_stmt(SereParser.Yield_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#yield_stmt}.
	 * @param ctx the parse tree
	 */
	void exitYield_stmt(SereParser.Yield_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#raise_stmt}.
	 * @param ctx the parse tree
	 */
	void enterRaise_stmt(SereParser.Raise_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#raise_stmt}.
	 * @param ctx the parse tree
	 */
	void exitRaise_stmt(SereParser.Raise_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#import_stmt}.
	 * @param ctx the parse tree
	 */
	void enterImport_stmt(SereParser.Import_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#import_stmt}.
	 * @param ctx the parse tree
	 */
	void exitImport_stmt(SereParser.Import_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#import_name}.
	 * @param ctx the parse tree
	 */
	void enterImport_name(SereParser.Import_nameContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#import_name}.
	 * @param ctx the parse tree
	 */
	void exitImport_name(SereParser.Import_nameContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#import_from}.
	 * @param ctx the parse tree
	 */
	void enterImport_from(SereParser.Import_fromContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#import_from}.
	 * @param ctx the parse tree
	 */
	void exitImport_from(SereParser.Import_fromContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#import_as_name}.
	 * @param ctx the parse tree
	 */
	void enterImport_as_name(SereParser.Import_as_nameContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#import_as_name}.
	 * @param ctx the parse tree
	 */
	void exitImport_as_name(SereParser.Import_as_nameContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#dotted_as_name}.
	 * @param ctx the parse tree
	 */
	void enterDotted_as_name(SereParser.Dotted_as_nameContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#dotted_as_name}.
	 * @param ctx the parse tree
	 */
	void exitDotted_as_name(SereParser.Dotted_as_nameContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#import_as_names}.
	 * @param ctx the parse tree
	 */
	void enterImport_as_names(SereParser.Import_as_namesContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#import_as_names}.
	 * @param ctx the parse tree
	 */
	void exitImport_as_names(SereParser.Import_as_namesContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#dotted_as_names}.
	 * @param ctx the parse tree
	 */
	void enterDotted_as_names(SereParser.Dotted_as_namesContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#dotted_as_names}.
	 * @param ctx the parse tree
	 */
	void exitDotted_as_names(SereParser.Dotted_as_namesContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#dotted_name}.
	 * @param ctx the parse tree
	 */
	void enterDotted_name(SereParser.Dotted_nameContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#dotted_name}.
	 * @param ctx the parse tree
	 */
	void exitDotted_name(SereParser.Dotted_nameContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#global_stmt}.
	 * @param ctx the parse tree
	 */
	void enterGlobal_stmt(SereParser.Global_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#global_stmt}.
	 * @param ctx the parse tree
	 */
	void exitGlobal_stmt(SereParser.Global_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#nonlocal_stmt}.
	 * @param ctx the parse tree
	 */
	void enterNonlocal_stmt(SereParser.Nonlocal_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#nonlocal_stmt}.
	 * @param ctx the parse tree
	 */
	void exitNonlocal_stmt(SereParser.Nonlocal_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#assert_stmt}.
	 * @param ctx the parse tree
	 */
	void enterAssert_stmt(SereParser.Assert_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#assert_stmt}.
	 * @param ctx the parse tree
	 */
	void exitAssert_stmt(SereParser.Assert_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#compound_stmt}.
	 * @param ctx the parse tree
	 */
	void enterCompound_stmt(SereParser.Compound_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#compound_stmt}.
	 * @param ctx the parse tree
	 */
	void exitCompound_stmt(SereParser.Compound_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#async_stmt}.
	 * @param ctx the parse tree
	 */
	void enterAsync_stmt(SereParser.Async_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#async_stmt}.
	 * @param ctx the parse tree
	 */
	void exitAsync_stmt(SereParser.Async_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#if_stmt}.
	 * @param ctx the parse tree
	 */
	void enterIf_stmt(SereParser.If_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#if_stmt}.
	 * @param ctx the parse tree
	 */
	void exitIf_stmt(SereParser.If_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#while_stmt}.
	 * @param ctx the parse tree
	 */
	void enterWhile_stmt(SereParser.While_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#while_stmt}.
	 * @param ctx the parse tree
	 */
	void exitWhile_stmt(SereParser.While_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#for_stmt}.
	 * @param ctx the parse tree
	 */
	void enterFor_stmt(SereParser.For_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#for_stmt}.
	 * @param ctx the parse tree
	 */
	void exitFor_stmt(SereParser.For_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#try_stmt}.
	 * @param ctx the parse tree
	 */
	void enterTry_stmt(SereParser.Try_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#try_stmt}.
	 * @param ctx the parse tree
	 */
	void exitTry_stmt(SereParser.Try_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#with_stmt}.
	 * @param ctx the parse tree
	 */
	void enterWith_stmt(SereParser.With_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#with_stmt}.
	 * @param ctx the parse tree
	 */
	void exitWith_stmt(SereParser.With_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#with_item}.
	 * @param ctx the parse tree
	 */
	void enterWith_item(SereParser.With_itemContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#with_item}.
	 * @param ctx the parse tree
	 */
	void exitWith_item(SereParser.With_itemContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#except_clause}.
	 * @param ctx the parse tree
	 */
	void enterExcept_clause(SereParser.Except_clauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#except_clause}.
	 * @param ctx the parse tree
	 */
	void exitExcept_clause(SereParser.Except_clauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(SereParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(SereParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#match_stmt}.
	 * @param ctx the parse tree
	 */
	void enterMatch_stmt(SereParser.Match_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#match_stmt}.
	 * @param ctx the parse tree
	 */
	void exitMatch_stmt(SereParser.Match_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#subject_expr}.
	 * @param ctx the parse tree
	 */
	void enterSubject_expr(SereParser.Subject_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#subject_expr}.
	 * @param ctx the parse tree
	 */
	void exitSubject_expr(SereParser.Subject_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#star_named_expressions}.
	 * @param ctx the parse tree
	 */
	void enterStar_named_expressions(SereParser.Star_named_expressionsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#star_named_expressions}.
	 * @param ctx the parse tree
	 */
	void exitStar_named_expressions(SereParser.Star_named_expressionsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#star_named_expression}.
	 * @param ctx the parse tree
	 */
	void enterStar_named_expression(SereParser.Star_named_expressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#star_named_expression}.
	 * @param ctx the parse tree
	 */
	void exitStar_named_expression(SereParser.Star_named_expressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#case_block}.
	 * @param ctx the parse tree
	 */
	void enterCase_block(SereParser.Case_blockContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#case_block}.
	 * @param ctx the parse tree
	 */
	void exitCase_block(SereParser.Case_blockContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#guard}.
	 * @param ctx the parse tree
	 */
	void enterGuard(SereParser.GuardContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#guard}.
	 * @param ctx the parse tree
	 */
	void exitGuard(SereParser.GuardContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#patterns}.
	 * @param ctx the parse tree
	 */
	void enterPatterns(SereParser.PatternsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#patterns}.
	 * @param ctx the parse tree
	 */
	void exitPatterns(SereParser.PatternsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#pattern}.
	 * @param ctx the parse tree
	 */
	void enterPattern(SereParser.PatternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#pattern}.
	 * @param ctx the parse tree
	 */
	void exitPattern(SereParser.PatternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#as_pattern}.
	 * @param ctx the parse tree
	 */
	void enterAs_pattern(SereParser.As_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#as_pattern}.
	 * @param ctx the parse tree
	 */
	void exitAs_pattern(SereParser.As_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#or_pattern}.
	 * @param ctx the parse tree
	 */
	void enterOr_pattern(SereParser.Or_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#or_pattern}.
	 * @param ctx the parse tree
	 */
	void exitOr_pattern(SereParser.Or_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#closed_pattern}.
	 * @param ctx the parse tree
	 */
	void enterClosed_pattern(SereParser.Closed_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#closed_pattern}.
	 * @param ctx the parse tree
	 */
	void exitClosed_pattern(SereParser.Closed_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#literal_pattern}.
	 * @param ctx the parse tree
	 */
	void enterLiteral_pattern(SereParser.Literal_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#literal_pattern}.
	 * @param ctx the parse tree
	 */
	void exitLiteral_pattern(SereParser.Literal_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#literal_expr}.
	 * @param ctx the parse tree
	 */
	void enterLiteral_expr(SereParser.Literal_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#literal_expr}.
	 * @param ctx the parse tree
	 */
	void exitLiteral_expr(SereParser.Literal_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#complex_number}.
	 * @param ctx the parse tree
	 */
	void enterComplex_number(SereParser.Complex_numberContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#complex_number}.
	 * @param ctx the parse tree
	 */
	void exitComplex_number(SereParser.Complex_numberContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#signed_number}.
	 * @param ctx the parse tree
	 */
	void enterSigned_number(SereParser.Signed_numberContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#signed_number}.
	 * @param ctx the parse tree
	 */
	void exitSigned_number(SereParser.Signed_numberContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#signed_real_number}.
	 * @param ctx the parse tree
	 */
	void enterSigned_real_number(SereParser.Signed_real_numberContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#signed_real_number}.
	 * @param ctx the parse tree
	 */
	void exitSigned_real_number(SereParser.Signed_real_numberContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#real_number}.
	 * @param ctx the parse tree
	 */
	void enterReal_number(SereParser.Real_numberContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#real_number}.
	 * @param ctx the parse tree
	 */
	void exitReal_number(SereParser.Real_numberContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#imaginary_number}.
	 * @param ctx the parse tree
	 */
	void enterImaginary_number(SereParser.Imaginary_numberContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#imaginary_number}.
	 * @param ctx the parse tree
	 */
	void exitImaginary_number(SereParser.Imaginary_numberContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#capture_pattern}.
	 * @param ctx the parse tree
	 */
	void enterCapture_pattern(SereParser.Capture_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#capture_pattern}.
	 * @param ctx the parse tree
	 */
	void exitCapture_pattern(SereParser.Capture_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#pattern_capture_target}.
	 * @param ctx the parse tree
	 */
	void enterPattern_capture_target(SereParser.Pattern_capture_targetContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#pattern_capture_target}.
	 * @param ctx the parse tree
	 */
	void exitPattern_capture_target(SereParser.Pattern_capture_targetContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#wildcard_pattern}.
	 * @param ctx the parse tree
	 */
	void enterWildcard_pattern(SereParser.Wildcard_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#wildcard_pattern}.
	 * @param ctx the parse tree
	 */
	void exitWildcard_pattern(SereParser.Wildcard_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#value_pattern}.
	 * @param ctx the parse tree
	 */
	void enterValue_pattern(SereParser.Value_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#value_pattern}.
	 * @param ctx the parse tree
	 */
	void exitValue_pattern(SereParser.Value_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#attr}.
	 * @param ctx the parse tree
	 */
	void enterAttr(SereParser.AttrContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#attr}.
	 * @param ctx the parse tree
	 */
	void exitAttr(SereParser.AttrContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#name_or_attr}.
	 * @param ctx the parse tree
	 */
	void enterName_or_attr(SereParser.Name_or_attrContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#name_or_attr}.
	 * @param ctx the parse tree
	 */
	void exitName_or_attr(SereParser.Name_or_attrContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#group_pattern}.
	 * @param ctx the parse tree
	 */
	void enterGroup_pattern(SereParser.Group_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#group_pattern}.
	 * @param ctx the parse tree
	 */
	void exitGroup_pattern(SereParser.Group_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#sequence_pattern}.
	 * @param ctx the parse tree
	 */
	void enterSequence_pattern(SereParser.Sequence_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#sequence_pattern}.
	 * @param ctx the parse tree
	 */
	void exitSequence_pattern(SereParser.Sequence_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#open_sequence_pattern}.
	 * @param ctx the parse tree
	 */
	void enterOpen_sequence_pattern(SereParser.Open_sequence_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#open_sequence_pattern}.
	 * @param ctx the parse tree
	 */
	void exitOpen_sequence_pattern(SereParser.Open_sequence_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#maybe_sequence_pattern}.
	 * @param ctx the parse tree
	 */
	void enterMaybe_sequence_pattern(SereParser.Maybe_sequence_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#maybe_sequence_pattern}.
	 * @param ctx the parse tree
	 */
	void exitMaybe_sequence_pattern(SereParser.Maybe_sequence_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#maybe_star_pattern}.
	 * @param ctx the parse tree
	 */
	void enterMaybe_star_pattern(SereParser.Maybe_star_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#maybe_star_pattern}.
	 * @param ctx the parse tree
	 */
	void exitMaybe_star_pattern(SereParser.Maybe_star_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#star_pattern}.
	 * @param ctx the parse tree
	 */
	void enterStar_pattern(SereParser.Star_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#star_pattern}.
	 * @param ctx the parse tree
	 */
	void exitStar_pattern(SereParser.Star_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#mapping_pattern}.
	 * @param ctx the parse tree
	 */
	void enterMapping_pattern(SereParser.Mapping_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#mapping_pattern}.
	 * @param ctx the parse tree
	 */
	void exitMapping_pattern(SereParser.Mapping_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#items_pattern}.
	 * @param ctx the parse tree
	 */
	void enterItems_pattern(SereParser.Items_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#items_pattern}.
	 * @param ctx the parse tree
	 */
	void exitItems_pattern(SereParser.Items_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#key_value_pattern}.
	 * @param ctx the parse tree
	 */
	void enterKey_value_pattern(SereParser.Key_value_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#key_value_pattern}.
	 * @param ctx the parse tree
	 */
	void exitKey_value_pattern(SereParser.Key_value_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#double_star_pattern}.
	 * @param ctx the parse tree
	 */
	void enterDouble_star_pattern(SereParser.Double_star_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#double_star_pattern}.
	 * @param ctx the parse tree
	 */
	void exitDouble_star_pattern(SereParser.Double_star_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#class_pattern}.
	 * @param ctx the parse tree
	 */
	void enterClass_pattern(SereParser.Class_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#class_pattern}.
	 * @param ctx the parse tree
	 */
	void exitClass_pattern(SereParser.Class_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#positional_patterns}.
	 * @param ctx the parse tree
	 */
	void enterPositional_patterns(SereParser.Positional_patternsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#positional_patterns}.
	 * @param ctx the parse tree
	 */
	void exitPositional_patterns(SereParser.Positional_patternsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#keyword_patterns}.
	 * @param ctx the parse tree
	 */
	void enterKeyword_patterns(SereParser.Keyword_patternsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#keyword_patterns}.
	 * @param ctx the parse tree
	 */
	void exitKeyword_patterns(SereParser.Keyword_patternsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#keyword_pattern}.
	 * @param ctx the parse tree
	 */
	void enterKeyword_pattern(SereParser.Keyword_patternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#keyword_pattern}.
	 * @param ctx the parse tree
	 */
	void exitKeyword_pattern(SereParser.Keyword_patternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#test}.
	 * @param ctx the parse tree
	 */
	void enterTest(SereParser.TestContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#test}.
	 * @param ctx the parse tree
	 */
	void exitTest(SereParser.TestContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#test_nocond}.
	 * @param ctx the parse tree
	 */
	void enterTest_nocond(SereParser.Test_nocondContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#test_nocond}.
	 * @param ctx the parse tree
	 */
	void exitTest_nocond(SereParser.Test_nocondContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#lambdef}.
	 * @param ctx the parse tree
	 */
	void enterLambdef(SereParser.LambdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#lambdef}.
	 * @param ctx the parse tree
	 */
	void exitLambdef(SereParser.LambdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#lambdef_nocond}.
	 * @param ctx the parse tree
	 */
	void enterLambdef_nocond(SereParser.Lambdef_nocondContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#lambdef_nocond}.
	 * @param ctx the parse tree
	 */
	void exitLambdef_nocond(SereParser.Lambdef_nocondContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#or_test}.
	 * @param ctx the parse tree
	 */
	void enterOr_test(SereParser.Or_testContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#or_test}.
	 * @param ctx the parse tree
	 */
	void exitOr_test(SereParser.Or_testContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#and_test}.
	 * @param ctx the parse tree
	 */
	void enterAnd_test(SereParser.And_testContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#and_test}.
	 * @param ctx the parse tree
	 */
	void exitAnd_test(SereParser.And_testContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#not_test}.
	 * @param ctx the parse tree
	 */
	void enterNot_test(SereParser.Not_testContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#not_test}.
	 * @param ctx the parse tree
	 */
	void exitNot_test(SereParser.Not_testContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#comparison}.
	 * @param ctx the parse tree
	 */
	void enterComparison(SereParser.ComparisonContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#comparison}.
	 * @param ctx the parse tree
	 */
	void exitComparison(SereParser.ComparisonContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#comp_op}.
	 * @param ctx the parse tree
	 */
	void enterComp_op(SereParser.Comp_opContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#comp_op}.
	 * @param ctx the parse tree
	 */
	void exitComp_op(SereParser.Comp_opContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#star_expr}.
	 * @param ctx the parse tree
	 */
	void enterStar_expr(SereParser.Star_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#star_expr}.
	 * @param ctx the parse tree
	 */
	void exitStar_expr(SereParser.Star_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterExpr(SereParser.ExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitExpr(SereParser.ExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#atom_expr}.
	 * @param ctx the parse tree
	 */
	void enterAtom_expr(SereParser.Atom_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#atom_expr}.
	 * @param ctx the parse tree
	 */
	void exitAtom_expr(SereParser.Atom_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#atom}.
	 * @param ctx the parse tree
	 */
	void enterAtom(SereParser.AtomContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#atom}.
	 * @param ctx the parse tree
	 */
	void exitAtom(SereParser.AtomContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#name}.
	 * @param ctx the parse tree
	 */
	void enterName(SereParser.NameContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#name}.
	 * @param ctx the parse tree
	 */
	void exitName(SereParser.NameContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#testlist_comp}.
	 * @param ctx the parse tree
	 */
	void enterTestlist_comp(SereParser.Testlist_compContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#testlist_comp}.
	 * @param ctx the parse tree
	 */
	void exitTestlist_comp(SereParser.Testlist_compContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#trailer}.
	 * @param ctx the parse tree
	 */
	void enterTrailer(SereParser.TrailerContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#trailer}.
	 * @param ctx the parse tree
	 */
	void exitTrailer(SereParser.TrailerContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#subscriptlist}.
	 * @param ctx the parse tree
	 */
	void enterSubscriptlist(SereParser.SubscriptlistContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#subscriptlist}.
	 * @param ctx the parse tree
	 */
	void exitSubscriptlist(SereParser.SubscriptlistContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#subscript_}.
	 * @param ctx the parse tree
	 */
	void enterSubscript_(SereParser.Subscript_Context ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#subscript_}.
	 * @param ctx the parse tree
	 */
	void exitSubscript_(SereParser.Subscript_Context ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#sliceop}.
	 * @param ctx the parse tree
	 */
	void enterSliceop(SereParser.SliceopContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#sliceop}.
	 * @param ctx the parse tree
	 */
	void exitSliceop(SereParser.SliceopContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#exprlist}.
	 * @param ctx the parse tree
	 */
	void enterExprlist(SereParser.ExprlistContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#exprlist}.
	 * @param ctx the parse tree
	 */
	void exitExprlist(SereParser.ExprlistContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#testlist}.
	 * @param ctx the parse tree
	 */
	void enterTestlist(SereParser.TestlistContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#testlist}.
	 * @param ctx the parse tree
	 */
	void exitTestlist(SereParser.TestlistContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#dictorsetmaker}.
	 * @param ctx the parse tree
	 */
	void enterDictorsetmaker(SereParser.DictorsetmakerContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#dictorsetmaker}.
	 * @param ctx the parse tree
	 */
	void exitDictorsetmaker(SereParser.DictorsetmakerContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#classdef}.
	 * @param ctx the parse tree
	 */
	void enterClassdef(SereParser.ClassdefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#classdef}.
	 * @param ctx the parse tree
	 */
	void exitClassdef(SereParser.ClassdefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#arglist}.
	 * @param ctx the parse tree
	 */
	void enterArglist(SereParser.ArglistContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#arglist}.
	 * @param ctx the parse tree
	 */
	void exitArglist(SereParser.ArglistContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#argument}.
	 * @param ctx the parse tree
	 */
	void enterArgument(SereParser.ArgumentContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#argument}.
	 * @param ctx the parse tree
	 */
	void exitArgument(SereParser.ArgumentContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#comp_iter}.
	 * @param ctx the parse tree
	 */
	void enterComp_iter(SereParser.Comp_iterContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#comp_iter}.
	 * @param ctx the parse tree
	 */
	void exitComp_iter(SereParser.Comp_iterContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#comp_for}.
	 * @param ctx the parse tree
	 */
	void enterComp_for(SereParser.Comp_forContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#comp_for}.
	 * @param ctx the parse tree
	 */
	void exitComp_for(SereParser.Comp_forContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#comp_if}.
	 * @param ctx the parse tree
	 */
	void enterComp_if(SereParser.Comp_ifContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#comp_if}.
	 * @param ctx the parse tree
	 */
	void exitComp_if(SereParser.Comp_ifContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#encoding_decl}.
	 * @param ctx the parse tree
	 */
	void enterEncoding_decl(SereParser.Encoding_declContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#encoding_decl}.
	 * @param ctx the parse tree
	 */
	void exitEncoding_decl(SereParser.Encoding_declContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#yield_expr}.
	 * @param ctx the parse tree
	 */
	void enterYield_expr(SereParser.Yield_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#yield_expr}.
	 * @param ctx the parse tree
	 */
	void exitYield_expr(SereParser.Yield_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#yield_arg}.
	 * @param ctx the parse tree
	 */
	void enterYield_arg(SereParser.Yield_argContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#yield_arg}.
	 * @param ctx the parse tree
	 */
	void exitYield_arg(SereParser.Yield_argContext ctx);
	/**
	 * Enter a parse tree produced by {@link SereParser#strings}.
	 * @param ctx the parse tree
	 */
	void enterStrings(SereParser.StringsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SereParser#strings}.
	 * @param ctx the parse tree
	 */
	void exitStrings(SereParser.StringsContext ctx);
}