# Generated from RSSQLParser.g4 by ANTLR 4.7.2
from antlr4 import *
if __name__ is not None and "." in __name__:
    from .RSSQLParser import RSSQLParser
else:
    from RSSQLParser import RSSQLParser

# This class defines a complete listener for a parse tree produced by RSSQLParser.
class RSSQLParserListener(ParseTreeListener):

    # Enter a parse tree produced by RSSQLParser#parse.
    def enterParse(self, ctx:RSSQLParser.ParseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#parse.
    def exitParse(self, ctx:RSSQLParser.ParseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#sql_stmt_list.
    def enterSql_stmt_list(self, ctx:RSSQLParser.Sql_stmt_listContext):
        pass

    # Exit a parse tree produced by RSSQLParser#sql_stmt_list.
    def exitSql_stmt_list(self, ctx:RSSQLParser.Sql_stmt_listContext):
        pass


    # Enter a parse tree produced by RSSQLParser#sql_stmt.
    def enterSql_stmt(self, ctx:RSSQLParser.Sql_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#sql_stmt.
    def exitSql_stmt(self, ctx:RSSQLParser.Sql_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#alter_table_stmt.
    def enterAlter_table_stmt(self, ctx:RSSQLParser.Alter_table_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#alter_table_stmt.
    def exitAlter_table_stmt(self, ctx:RSSQLParser.Alter_table_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#analyze_stmt.
    def enterAnalyze_stmt(self, ctx:RSSQLParser.Analyze_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#analyze_stmt.
    def exitAnalyze_stmt(self, ctx:RSSQLParser.Analyze_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#attach_stmt.
    def enterAttach_stmt(self, ctx:RSSQLParser.Attach_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#attach_stmt.
    def exitAttach_stmt(self, ctx:RSSQLParser.Attach_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#begin_stmt.
    def enterBegin_stmt(self, ctx:RSSQLParser.Begin_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#begin_stmt.
    def exitBegin_stmt(self, ctx:RSSQLParser.Begin_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#commit_stmt.
    def enterCommit_stmt(self, ctx:RSSQLParser.Commit_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#commit_stmt.
    def exitCommit_stmt(self, ctx:RSSQLParser.Commit_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#rollback_stmt.
    def enterRollback_stmt(self, ctx:RSSQLParser.Rollback_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#rollback_stmt.
    def exitRollback_stmt(self, ctx:RSSQLParser.Rollback_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#savepoint_stmt.
    def enterSavepoint_stmt(self, ctx:RSSQLParser.Savepoint_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#savepoint_stmt.
    def exitSavepoint_stmt(self, ctx:RSSQLParser.Savepoint_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#release_stmt.
    def enterRelease_stmt(self, ctx:RSSQLParser.Release_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#release_stmt.
    def exitRelease_stmt(self, ctx:RSSQLParser.Release_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#create_index_stmt.
    def enterCreate_index_stmt(self, ctx:RSSQLParser.Create_index_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#create_index_stmt.
    def exitCreate_index_stmt(self, ctx:RSSQLParser.Create_index_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#indexed_column.
    def enterIndexed_column(self, ctx:RSSQLParser.Indexed_columnContext):
        pass

    # Exit a parse tree produced by RSSQLParser#indexed_column.
    def exitIndexed_column(self, ctx:RSSQLParser.Indexed_columnContext):
        pass


    # Enter a parse tree produced by RSSQLParser#create_table_stmt.
    def enterCreate_table_stmt(self, ctx:RSSQLParser.Create_table_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#create_table_stmt.
    def exitCreate_table_stmt(self, ctx:RSSQLParser.Create_table_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#column_def.
    def enterColumn_def(self, ctx:RSSQLParser.Column_defContext):
        pass

    # Exit a parse tree produced by RSSQLParser#column_def.
    def exitColumn_def(self, ctx:RSSQLParser.Column_defContext):
        pass


    # Enter a parse tree produced by RSSQLParser#type_name.
    def enterType_name(self, ctx:RSSQLParser.Type_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#type_name.
    def exitType_name(self, ctx:RSSQLParser.Type_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#column_constraint.
    def enterColumn_constraint(self, ctx:RSSQLParser.Column_constraintContext):
        pass

    # Exit a parse tree produced by RSSQLParser#column_constraint.
    def exitColumn_constraint(self, ctx:RSSQLParser.Column_constraintContext):
        pass


    # Enter a parse tree produced by RSSQLParser#signed_number.
    def enterSigned_number(self, ctx:RSSQLParser.Signed_numberContext):
        pass

    # Exit a parse tree produced by RSSQLParser#signed_number.
    def exitSigned_number(self, ctx:RSSQLParser.Signed_numberContext):
        pass


    # Enter a parse tree produced by RSSQLParser#table_constraint.
    def enterTable_constraint(self, ctx:RSSQLParser.Table_constraintContext):
        pass

    # Exit a parse tree produced by RSSQLParser#table_constraint.
    def exitTable_constraint(self, ctx:RSSQLParser.Table_constraintContext):
        pass


    # Enter a parse tree produced by RSSQLParser#foreign_key_clause.
    def enterForeign_key_clause(self, ctx:RSSQLParser.Foreign_key_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#foreign_key_clause.
    def exitForeign_key_clause(self, ctx:RSSQLParser.Foreign_key_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#conflict_clause.
    def enterConflict_clause(self, ctx:RSSQLParser.Conflict_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#conflict_clause.
    def exitConflict_clause(self, ctx:RSSQLParser.Conflict_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#create_trigger_stmt.
    def enterCreate_trigger_stmt(self, ctx:RSSQLParser.Create_trigger_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#create_trigger_stmt.
    def exitCreate_trigger_stmt(self, ctx:RSSQLParser.Create_trigger_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#create_view_stmt.
    def enterCreate_view_stmt(self, ctx:RSSQLParser.Create_view_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#create_view_stmt.
    def exitCreate_view_stmt(self, ctx:RSSQLParser.Create_view_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#create_virtual_table_stmt.
    def enterCreate_virtual_table_stmt(self, ctx:RSSQLParser.Create_virtual_table_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#create_virtual_table_stmt.
    def exitCreate_virtual_table_stmt(self, ctx:RSSQLParser.Create_virtual_table_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#with_clause.
    def enterWith_clause(self, ctx:RSSQLParser.With_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#with_clause.
    def exitWith_clause(self, ctx:RSSQLParser.With_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#cte_table_name.
    def enterCte_table_name(self, ctx:RSSQLParser.Cte_table_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#cte_table_name.
    def exitCte_table_name(self, ctx:RSSQLParser.Cte_table_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#recursive_cte.
    def enterRecursive_cte(self, ctx:RSSQLParser.Recursive_cteContext):
        pass

    # Exit a parse tree produced by RSSQLParser#recursive_cte.
    def exitRecursive_cte(self, ctx:RSSQLParser.Recursive_cteContext):
        pass


    # Enter a parse tree produced by RSSQLParser#common_table_expression.
    def enterCommon_table_expression(self, ctx:RSSQLParser.Common_table_expressionContext):
        pass

    # Exit a parse tree produced by RSSQLParser#common_table_expression.
    def exitCommon_table_expression(self, ctx:RSSQLParser.Common_table_expressionContext):
        pass


    # Enter a parse tree produced by RSSQLParser#delete_stmt.
    def enterDelete_stmt(self, ctx:RSSQLParser.Delete_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#delete_stmt.
    def exitDelete_stmt(self, ctx:RSSQLParser.Delete_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#delete_stmt_limited.
    def enterDelete_stmt_limited(self, ctx:RSSQLParser.Delete_stmt_limitedContext):
        pass

    # Exit a parse tree produced by RSSQLParser#delete_stmt_limited.
    def exitDelete_stmt_limited(self, ctx:RSSQLParser.Delete_stmt_limitedContext):
        pass


    # Enter a parse tree produced by RSSQLParser#detach_stmt.
    def enterDetach_stmt(self, ctx:RSSQLParser.Detach_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#detach_stmt.
    def exitDetach_stmt(self, ctx:RSSQLParser.Detach_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#drop_stmt.
    def enterDrop_stmt(self, ctx:RSSQLParser.Drop_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#drop_stmt.
    def exitDrop_stmt(self, ctx:RSSQLParser.Drop_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#expr.
    def enterExpr(self, ctx:RSSQLParser.ExprContext):
        pass

    # Exit a parse tree produced by RSSQLParser#expr.
    def exitExpr(self, ctx:RSSQLParser.ExprContext):
        pass


    # Enter a parse tree produced by RSSQLParser#raise_function.
    def enterRaise_function(self, ctx:RSSQLParser.Raise_functionContext):
        pass

    # Exit a parse tree produced by RSSQLParser#raise_function.
    def exitRaise_function(self, ctx:RSSQLParser.Raise_functionContext):
        pass


    # Enter a parse tree produced by RSSQLParser#literal_value.
    def enterLiteral_value(self, ctx:RSSQLParser.Literal_valueContext):
        pass

    # Exit a parse tree produced by RSSQLParser#literal_value.
    def exitLiteral_value(self, ctx:RSSQLParser.Literal_valueContext):
        pass


    # Enter a parse tree produced by RSSQLParser#value_row.
    def enterValue_row(self, ctx:RSSQLParser.Value_rowContext):
        pass

    # Exit a parse tree produced by RSSQLParser#value_row.
    def exitValue_row(self, ctx:RSSQLParser.Value_rowContext):
        pass


    # Enter a parse tree produced by RSSQLParser#values_clause.
    def enterValues_clause(self, ctx:RSSQLParser.Values_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#values_clause.
    def exitValues_clause(self, ctx:RSSQLParser.Values_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#insert_stmt.
    def enterInsert_stmt(self, ctx:RSSQLParser.Insert_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#insert_stmt.
    def exitInsert_stmt(self, ctx:RSSQLParser.Insert_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#returning_clause.
    def enterReturning_clause(self, ctx:RSSQLParser.Returning_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#returning_clause.
    def exitReturning_clause(self, ctx:RSSQLParser.Returning_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#upsert_clause.
    def enterUpsert_clause(self, ctx:RSSQLParser.Upsert_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#upsert_clause.
    def exitUpsert_clause(self, ctx:RSSQLParser.Upsert_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#pragma_stmt.
    def enterPragma_stmt(self, ctx:RSSQLParser.Pragma_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#pragma_stmt.
    def exitPragma_stmt(self, ctx:RSSQLParser.Pragma_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#pragma_value.
    def enterPragma_value(self, ctx:RSSQLParser.Pragma_valueContext):
        pass

    # Exit a parse tree produced by RSSQLParser#pragma_value.
    def exitPragma_value(self, ctx:RSSQLParser.Pragma_valueContext):
        pass


    # Enter a parse tree produced by RSSQLParser#reindex_stmt.
    def enterReindex_stmt(self, ctx:RSSQLParser.Reindex_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#reindex_stmt.
    def exitReindex_stmt(self, ctx:RSSQLParser.Reindex_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#select_stmt.
    def enterSelect_stmt(self, ctx:RSSQLParser.Select_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#select_stmt.
    def exitSelect_stmt(self, ctx:RSSQLParser.Select_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#join_clause.
    def enterJoin_clause(self, ctx:RSSQLParser.Join_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#join_clause.
    def exitJoin_clause(self, ctx:RSSQLParser.Join_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#select_core.
    def enterSelect_core(self, ctx:RSSQLParser.Select_coreContext):
        pass

    # Exit a parse tree produced by RSSQLParser#select_core.
    def exitSelect_core(self, ctx:RSSQLParser.Select_coreContext):
        pass


    # Enter a parse tree produced by RSSQLParser#factored_select_stmt.
    def enterFactored_select_stmt(self, ctx:RSSQLParser.Factored_select_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#factored_select_stmt.
    def exitFactored_select_stmt(self, ctx:RSSQLParser.Factored_select_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#simple_select_stmt.
    def enterSimple_select_stmt(self, ctx:RSSQLParser.Simple_select_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#simple_select_stmt.
    def exitSimple_select_stmt(self, ctx:RSSQLParser.Simple_select_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#compound_select_stmt.
    def enterCompound_select_stmt(self, ctx:RSSQLParser.Compound_select_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#compound_select_stmt.
    def exitCompound_select_stmt(self, ctx:RSSQLParser.Compound_select_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#table_or_subquery.
    def enterTable_or_subquery(self, ctx:RSSQLParser.Table_or_subqueryContext):
        pass

    # Exit a parse tree produced by RSSQLParser#table_or_subquery.
    def exitTable_or_subquery(self, ctx:RSSQLParser.Table_or_subqueryContext):
        pass


    # Enter a parse tree produced by RSSQLParser#result_column.
    def enterResult_column(self, ctx:RSSQLParser.Result_columnContext):
        pass

    # Exit a parse tree produced by RSSQLParser#result_column.
    def exitResult_column(self, ctx:RSSQLParser.Result_columnContext):
        pass


    # Enter a parse tree produced by RSSQLParser#join_operator.
    def enterJoin_operator(self, ctx:RSSQLParser.Join_operatorContext):
        pass

    # Exit a parse tree produced by RSSQLParser#join_operator.
    def exitJoin_operator(self, ctx:RSSQLParser.Join_operatorContext):
        pass


    # Enter a parse tree produced by RSSQLParser#join_constraint.
    def enterJoin_constraint(self, ctx:RSSQLParser.Join_constraintContext):
        pass

    # Exit a parse tree produced by RSSQLParser#join_constraint.
    def exitJoin_constraint(self, ctx:RSSQLParser.Join_constraintContext):
        pass


    # Enter a parse tree produced by RSSQLParser#compound_operator.
    def enterCompound_operator(self, ctx:RSSQLParser.Compound_operatorContext):
        pass

    # Exit a parse tree produced by RSSQLParser#compound_operator.
    def exitCompound_operator(self, ctx:RSSQLParser.Compound_operatorContext):
        pass


    # Enter a parse tree produced by RSSQLParser#update_stmt.
    def enterUpdate_stmt(self, ctx:RSSQLParser.Update_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#update_stmt.
    def exitUpdate_stmt(self, ctx:RSSQLParser.Update_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#column_name_list.
    def enterColumn_name_list(self, ctx:RSSQLParser.Column_name_listContext):
        pass

    # Exit a parse tree produced by RSSQLParser#column_name_list.
    def exitColumn_name_list(self, ctx:RSSQLParser.Column_name_listContext):
        pass


    # Enter a parse tree produced by RSSQLParser#update_stmt_limited.
    def enterUpdate_stmt_limited(self, ctx:RSSQLParser.Update_stmt_limitedContext):
        pass

    # Exit a parse tree produced by RSSQLParser#update_stmt_limited.
    def exitUpdate_stmt_limited(self, ctx:RSSQLParser.Update_stmt_limitedContext):
        pass


    # Enter a parse tree produced by RSSQLParser#qualified_table_name.
    def enterQualified_table_name(self, ctx:RSSQLParser.Qualified_table_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#qualified_table_name.
    def exitQualified_table_name(self, ctx:RSSQLParser.Qualified_table_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#vacuum_stmt.
    def enterVacuum_stmt(self, ctx:RSSQLParser.Vacuum_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#vacuum_stmt.
    def exitVacuum_stmt(self, ctx:RSSQLParser.Vacuum_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#filter_clause.
    def enterFilter_clause(self, ctx:RSSQLParser.Filter_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#filter_clause.
    def exitFilter_clause(self, ctx:RSSQLParser.Filter_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#window_defn.
    def enterWindow_defn(self, ctx:RSSQLParser.Window_defnContext):
        pass

    # Exit a parse tree produced by RSSQLParser#window_defn.
    def exitWindow_defn(self, ctx:RSSQLParser.Window_defnContext):
        pass


    # Enter a parse tree produced by RSSQLParser#over_clause.
    def enterOver_clause(self, ctx:RSSQLParser.Over_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#over_clause.
    def exitOver_clause(self, ctx:RSSQLParser.Over_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#frame_spec.
    def enterFrame_spec(self, ctx:RSSQLParser.Frame_specContext):
        pass

    # Exit a parse tree produced by RSSQLParser#frame_spec.
    def exitFrame_spec(self, ctx:RSSQLParser.Frame_specContext):
        pass


    # Enter a parse tree produced by RSSQLParser#frame_clause.
    def enterFrame_clause(self, ctx:RSSQLParser.Frame_clauseContext):
        pass

    # Exit a parse tree produced by RSSQLParser#frame_clause.
    def exitFrame_clause(self, ctx:RSSQLParser.Frame_clauseContext):
        pass


    # Enter a parse tree produced by RSSQLParser#simple_function_invocation.
    def enterSimple_function_invocation(self, ctx:RSSQLParser.Simple_function_invocationContext):
        pass

    # Exit a parse tree produced by RSSQLParser#simple_function_invocation.
    def exitSimple_function_invocation(self, ctx:RSSQLParser.Simple_function_invocationContext):
        pass


    # Enter a parse tree produced by RSSQLParser#aggregate_function_invocation.
    def enterAggregate_function_invocation(self, ctx:RSSQLParser.Aggregate_function_invocationContext):
        pass

    # Exit a parse tree produced by RSSQLParser#aggregate_function_invocation.
    def exitAggregate_function_invocation(self, ctx:RSSQLParser.Aggregate_function_invocationContext):
        pass


    # Enter a parse tree produced by RSSQLParser#window_function_invocation.
    def enterWindow_function_invocation(self, ctx:RSSQLParser.Window_function_invocationContext):
        pass

    # Exit a parse tree produced by RSSQLParser#window_function_invocation.
    def exitWindow_function_invocation(self, ctx:RSSQLParser.Window_function_invocationContext):
        pass


    # Enter a parse tree produced by RSSQLParser#common_table_stmt.
    def enterCommon_table_stmt(self, ctx:RSSQLParser.Common_table_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#common_table_stmt.
    def exitCommon_table_stmt(self, ctx:RSSQLParser.Common_table_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#order_by_stmt.
    def enterOrder_by_stmt(self, ctx:RSSQLParser.Order_by_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#order_by_stmt.
    def exitOrder_by_stmt(self, ctx:RSSQLParser.Order_by_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#limit_stmt.
    def enterLimit_stmt(self, ctx:RSSQLParser.Limit_stmtContext):
        pass

    # Exit a parse tree produced by RSSQLParser#limit_stmt.
    def exitLimit_stmt(self, ctx:RSSQLParser.Limit_stmtContext):
        pass


    # Enter a parse tree produced by RSSQLParser#ordering_term.
    def enterOrdering_term(self, ctx:RSSQLParser.Ordering_termContext):
        pass

    # Exit a parse tree produced by RSSQLParser#ordering_term.
    def exitOrdering_term(self, ctx:RSSQLParser.Ordering_termContext):
        pass


    # Enter a parse tree produced by RSSQLParser#asc_desc.
    def enterAsc_desc(self, ctx:RSSQLParser.Asc_descContext):
        pass

    # Exit a parse tree produced by RSSQLParser#asc_desc.
    def exitAsc_desc(self, ctx:RSSQLParser.Asc_descContext):
        pass


    # Enter a parse tree produced by RSSQLParser#frame_left.
    def enterFrame_left(self, ctx:RSSQLParser.Frame_leftContext):
        pass

    # Exit a parse tree produced by RSSQLParser#frame_left.
    def exitFrame_left(self, ctx:RSSQLParser.Frame_leftContext):
        pass


    # Enter a parse tree produced by RSSQLParser#frame_right.
    def enterFrame_right(self, ctx:RSSQLParser.Frame_rightContext):
        pass

    # Exit a parse tree produced by RSSQLParser#frame_right.
    def exitFrame_right(self, ctx:RSSQLParser.Frame_rightContext):
        pass


    # Enter a parse tree produced by RSSQLParser#frame_single.
    def enterFrame_single(self, ctx:RSSQLParser.Frame_singleContext):
        pass

    # Exit a parse tree produced by RSSQLParser#frame_single.
    def exitFrame_single(self, ctx:RSSQLParser.Frame_singleContext):
        pass


    # Enter a parse tree produced by RSSQLParser#window_function.
    def enterWindow_function(self, ctx:RSSQLParser.Window_functionContext):
        pass

    # Exit a parse tree produced by RSSQLParser#window_function.
    def exitWindow_function(self, ctx:RSSQLParser.Window_functionContext):
        pass


    # Enter a parse tree produced by RSSQLParser#offset.
    def enterOffset(self, ctx:RSSQLParser.OffsetContext):
        pass

    # Exit a parse tree produced by RSSQLParser#offset.
    def exitOffset(self, ctx:RSSQLParser.OffsetContext):
        pass


    # Enter a parse tree produced by RSSQLParser#default_value.
    def enterDefault_value(self, ctx:RSSQLParser.Default_valueContext):
        pass

    # Exit a parse tree produced by RSSQLParser#default_value.
    def exitDefault_value(self, ctx:RSSQLParser.Default_valueContext):
        pass


    # Enter a parse tree produced by RSSQLParser#partition_by.
    def enterPartition_by(self, ctx:RSSQLParser.Partition_byContext):
        pass

    # Exit a parse tree produced by RSSQLParser#partition_by.
    def exitPartition_by(self, ctx:RSSQLParser.Partition_byContext):
        pass


    # Enter a parse tree produced by RSSQLParser#order_by_expr.
    def enterOrder_by_expr(self, ctx:RSSQLParser.Order_by_exprContext):
        pass

    # Exit a parse tree produced by RSSQLParser#order_by_expr.
    def exitOrder_by_expr(self, ctx:RSSQLParser.Order_by_exprContext):
        pass


    # Enter a parse tree produced by RSSQLParser#order_by_expr_asc_desc.
    def enterOrder_by_expr_asc_desc(self, ctx:RSSQLParser.Order_by_expr_asc_descContext):
        pass

    # Exit a parse tree produced by RSSQLParser#order_by_expr_asc_desc.
    def exitOrder_by_expr_asc_desc(self, ctx:RSSQLParser.Order_by_expr_asc_descContext):
        pass


    # Enter a parse tree produced by RSSQLParser#expr_asc_desc.
    def enterExpr_asc_desc(self, ctx:RSSQLParser.Expr_asc_descContext):
        pass

    # Exit a parse tree produced by RSSQLParser#expr_asc_desc.
    def exitExpr_asc_desc(self, ctx:RSSQLParser.Expr_asc_descContext):
        pass


    # Enter a parse tree produced by RSSQLParser#initial_select.
    def enterInitial_select(self, ctx:RSSQLParser.Initial_selectContext):
        pass

    # Exit a parse tree produced by RSSQLParser#initial_select.
    def exitInitial_select(self, ctx:RSSQLParser.Initial_selectContext):
        pass


    # Enter a parse tree produced by RSSQLParser#recursive_select.
    def enterRecursive_select(self, ctx:RSSQLParser.Recursive_selectContext):
        pass

    # Exit a parse tree produced by RSSQLParser#recursive_select.
    def exitRecursive_select(self, ctx:RSSQLParser.Recursive_selectContext):
        pass


    # Enter a parse tree produced by RSSQLParser#unary_operator.
    def enterUnary_operator(self, ctx:RSSQLParser.Unary_operatorContext):
        pass

    # Exit a parse tree produced by RSSQLParser#unary_operator.
    def exitUnary_operator(self, ctx:RSSQLParser.Unary_operatorContext):
        pass


    # Enter a parse tree produced by RSSQLParser#error_message.
    def enterError_message(self, ctx:RSSQLParser.Error_messageContext):
        pass

    # Exit a parse tree produced by RSSQLParser#error_message.
    def exitError_message(self, ctx:RSSQLParser.Error_messageContext):
        pass


    # Enter a parse tree produced by RSSQLParser#module_argument.
    def enterModule_argument(self, ctx:RSSQLParser.Module_argumentContext):
        pass

    # Exit a parse tree produced by RSSQLParser#module_argument.
    def exitModule_argument(self, ctx:RSSQLParser.Module_argumentContext):
        pass


    # Enter a parse tree produced by RSSQLParser#column_alias.
    def enterColumn_alias(self, ctx:RSSQLParser.Column_aliasContext):
        pass

    # Exit a parse tree produced by RSSQLParser#column_alias.
    def exitColumn_alias(self, ctx:RSSQLParser.Column_aliasContext):
        pass


    # Enter a parse tree produced by RSSQLParser#keyword.
    def enterKeyword(self, ctx:RSSQLParser.KeywordContext):
        pass

    # Exit a parse tree produced by RSSQLParser#keyword.
    def exitKeyword(self, ctx:RSSQLParser.KeywordContext):
        pass


    # Enter a parse tree produced by RSSQLParser#name.
    def enterName(self, ctx:RSSQLParser.NameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#name.
    def exitName(self, ctx:RSSQLParser.NameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#function_name.
    def enterFunction_name(self, ctx:RSSQLParser.Function_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#function_name.
    def exitFunction_name(self, ctx:RSSQLParser.Function_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#schema_name.
    def enterSchema_name(self, ctx:RSSQLParser.Schema_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#schema_name.
    def exitSchema_name(self, ctx:RSSQLParser.Schema_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#table_name.
    def enterTable_name(self, ctx:RSSQLParser.Table_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#table_name.
    def exitTable_name(self, ctx:RSSQLParser.Table_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#table_or_index_name.
    def enterTable_or_index_name(self, ctx:RSSQLParser.Table_or_index_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#table_or_index_name.
    def exitTable_or_index_name(self, ctx:RSSQLParser.Table_or_index_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#column_name.
    def enterColumn_name(self, ctx:RSSQLParser.Column_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#column_name.
    def exitColumn_name(self, ctx:RSSQLParser.Column_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#collation_name.
    def enterCollation_name(self, ctx:RSSQLParser.Collation_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#collation_name.
    def exitCollation_name(self, ctx:RSSQLParser.Collation_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#foreign_table.
    def enterForeign_table(self, ctx:RSSQLParser.Foreign_tableContext):
        pass

    # Exit a parse tree produced by RSSQLParser#foreign_table.
    def exitForeign_table(self, ctx:RSSQLParser.Foreign_tableContext):
        pass


    # Enter a parse tree produced by RSSQLParser#index_name.
    def enterIndex_name(self, ctx:RSSQLParser.Index_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#index_name.
    def exitIndex_name(self, ctx:RSSQLParser.Index_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#trigger_name.
    def enterTrigger_name(self, ctx:RSSQLParser.Trigger_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#trigger_name.
    def exitTrigger_name(self, ctx:RSSQLParser.Trigger_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#view_name.
    def enterView_name(self, ctx:RSSQLParser.View_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#view_name.
    def exitView_name(self, ctx:RSSQLParser.View_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#module_name.
    def enterModule_name(self, ctx:RSSQLParser.Module_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#module_name.
    def exitModule_name(self, ctx:RSSQLParser.Module_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#pragma_name.
    def enterPragma_name(self, ctx:RSSQLParser.Pragma_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#pragma_name.
    def exitPragma_name(self, ctx:RSSQLParser.Pragma_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#savepoint_name.
    def enterSavepoint_name(self, ctx:RSSQLParser.Savepoint_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#savepoint_name.
    def exitSavepoint_name(self, ctx:RSSQLParser.Savepoint_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#table_alias.
    def enterTable_alias(self, ctx:RSSQLParser.Table_aliasContext):
        pass

    # Exit a parse tree produced by RSSQLParser#table_alias.
    def exitTable_alias(self, ctx:RSSQLParser.Table_aliasContext):
        pass


    # Enter a parse tree produced by RSSQLParser#transaction_name.
    def enterTransaction_name(self, ctx:RSSQLParser.Transaction_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#transaction_name.
    def exitTransaction_name(self, ctx:RSSQLParser.Transaction_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#window_name.
    def enterWindow_name(self, ctx:RSSQLParser.Window_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#window_name.
    def exitWindow_name(self, ctx:RSSQLParser.Window_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#alias.
    def enterAlias(self, ctx:RSSQLParser.AliasContext):
        pass

    # Exit a parse tree produced by RSSQLParser#alias.
    def exitAlias(self, ctx:RSSQLParser.AliasContext):
        pass


    # Enter a parse tree produced by RSSQLParser#filename.
    def enterFilename(self, ctx:RSSQLParser.FilenameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#filename.
    def exitFilename(self, ctx:RSSQLParser.FilenameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#base_window_name.
    def enterBase_window_name(self, ctx:RSSQLParser.Base_window_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#base_window_name.
    def exitBase_window_name(self, ctx:RSSQLParser.Base_window_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#simple_func.
    def enterSimple_func(self, ctx:RSSQLParser.Simple_funcContext):
        pass

    # Exit a parse tree produced by RSSQLParser#simple_func.
    def exitSimple_func(self, ctx:RSSQLParser.Simple_funcContext):
        pass


    # Enter a parse tree produced by RSSQLParser#aggregate_func.
    def enterAggregate_func(self, ctx:RSSQLParser.Aggregate_funcContext):
        pass

    # Exit a parse tree produced by RSSQLParser#aggregate_func.
    def exitAggregate_func(self, ctx:RSSQLParser.Aggregate_funcContext):
        pass


    # Enter a parse tree produced by RSSQLParser#table_function_name.
    def enterTable_function_name(self, ctx:RSSQLParser.Table_function_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#table_function_name.
    def exitTable_function_name(self, ctx:RSSQLParser.Table_function_nameContext):
        pass


    # Enter a parse tree produced by RSSQLParser#any_name.
    def enterAny_name(self, ctx:RSSQLParser.Any_nameContext):
        pass

    # Exit a parse tree produced by RSSQLParser#any_name.
    def exitAny_name(self, ctx:RSSQLParser.Any_nameContext):
        pass


