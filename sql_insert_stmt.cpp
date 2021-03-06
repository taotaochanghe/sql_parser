#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "parse_malloc.h"
#include "sql_logical_plan.h"
#include "utility.h"
#include "sql_insert_stmt.h"

using namespace std;
using namespace jdbd::common;
using namespace jdbd::sql;

ObInsertStmt::ObInsertStmt()
: ObStmt(T_INSERT)
{
    sub_query_id_   = OB_INVALID_ID;
    is_replace_     = false;
    has_auto_incr   = false;

    for (int i = 0; i < AUTO_INCR_MAX_INDEX; i++)
    {
        auto_incr_ids[i] = 0;
    }
}

ObInsertStmt::~ObInsertStmt()
{
    for (uint32_t i = 0; i < value_vectors_.size(); i++)
    {
        vector<uint64_t>& value_row = value_vectors_.at(i);
        value_row.clear();
    }
}

void ObInsertStmt::print(FILE* fp, int32_t level, int32_t index)
{
    print_indentation(fp, level);
    fprintf(fp, "<ObInsertStmt %d Begin>\n", index);
    ObStmt::print(fp, level + 1);
    print_indentation(fp, level + 1);
    fprintf(fp, "INTO ::= <%ld>\n", table_id_);
    if (sub_query_id_ == OB_INVALID_ID)
    {
        print_indentation(fp, level + 1);
        fprintf(fp, "VALUES ::= ");
        for (uint32_t i = 0; i < value_vectors_.size(); i++)
        {
            if (i == 0)
                fprintf(fp, "<");
            else
                fprintf(fp, ", <");
            vector<uint64_t>& value_row = value_vectors_.at(i);
            for (uint32_t j = 0; j < value_row.size(); j++)
            {
                if (j == 0)
                    fprintf(fp, "%ld", value_row.at(j));
                else
                    fprintf(fp, ", %ld", value_row.at(j));
            }
            fprintf(fp, ">");
        }
        fprintf(fp, "\n");
    }
    else
    {
        print_indentation(fp, level + 1);
        fprintf(fp, "SUBQUERY ::= <%ld>\n", sub_query_id_);
    }
    print_indentation(fp, level);
    fprintf(fp, "<ObInsertStmt %d End>\n", index);
}

/**************************************************
Funtion     :   make_stmt_string
Author      :   qinbo
Date        :   2013.9.10
Description :   make select sql
Input       :   ResultPlan& result_plan,
                string &assembled_sql
Output      :   
 **************************************************/
int64_t ObInsertStmt::make_stmt_string(ResultPlan& result_plan, string &assembled_sql)
{
    int& ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    ObSqlRawExpr* sql_expr = NULL;

    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);

    if (is_replace_)
    {
        assembled_sql.append("REPLACE INTO ");
    }
    else
    {
        assembled_sql.append("INSERT INTO ");
    }

    assembled_sql.append(ObStmt::get_table_item_by_id(table_id_)->table_name_);
        
    for (int64_t i = 0; i < ObStmt::get_column_size(); i++)
    {
        if (0 == i)
        {
            assembled_sql.append(" (");
        }

        assembled_sql.append(ObStmt::get_column_item(i)->column_name_);
            
        if (i != ObStmt::get_column_size() - 1)
        {
            assembled_sql.append(", ");
        }
        else
        {
            assembled_sql.append(") ");
        }
    }

    if (sub_query_id_ == OB_INVALID_ID)
    {
        assembled_sql.append("VALUES ");
            
        for (uint32_t i = 0; i < value_vectors_.size(); i++)
        {
            vector<uint64_t>& value_row = value_vectors_.at(i);
            for (uint32_t j = 0; j < value_row.size(); j++)
            {
                string assembled_sql_tmp;
                if (j == 0)
                {
                    assembled_sql.append("(");
                }

                sql_expr = logical_plan->get_expr_by_id(value_row.at(j));

                if (NULL == sql_expr)
                {
                    ret = JD_ERR_LOGICAL_PLAN_FAILD;
                    jlog(WARNING, "insert value expr error!!!");
                    return ret;
                }

                sql_expr->to_string(result_plan, assembled_sql_tmp);
                assembled_sql.append(assembled_sql_tmp);
                    
                if (j == value_row.size() - 1)
                {
                    assembled_sql.append(")");
                }
                else
                {
                    assembled_sql.append(",");
                }
            }

            if (i != value_vectors_.size() - 1)
            {
                assembled_sql.append(",");
            }
        }
    }
    else
    {
        string assembled_sql_tmp;
        ObBasicStmt* query_stmt = logical_plan->get_query(sub_query_id_);

        if (NULL == query_stmt)
        {
            ret = JD_ERR_LOGICAL_PLAN_FAILD;
            jlog(WARNING, "can not get query stmt by query id!!!");
            return ret;
        }

        query_stmt->make_stmt_string(result_plan, assembled_sql_tmp);
        assembled_sql.append(assembled_sql_tmp);
    }
    
    return ret;
}

/**************************************************
Funtion     :   make_exec_plan_unit_string
Author      :   qinbo
Date        :   2013.12.9
Description :   make select sql
Input       :   ResultPlan& result_plan, 
                string insert_rows, 
                vector<schema_shard*> binding_shard_info,
                string &assembled_sql
Output      :   
 **************************************************/
int64_t ObInsertStmt::make_exec_plan_unit_string(ResultPlan& result_plan, 
                                                string insert_rows, 
                                                vector<schema_shard*> &binding_shard_info,
                                                string &assembled_sql)
{
    int& ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    ObSqlRawExpr* sql_expr = NULL;

    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);

    if (is_replace_)
    {
        assembled_sql.append("REPLACE INTO ");
    }
    else
    {
        assembled_sql.append("INSERT INTO ");
    }

    string table_name = ObStmt::get_table_item_by_id(table_id_)->table_name_;
    if (table_name != binding_shard_info.at(0)->get_table_name())
    {
        assembled_sql.append(binding_shard_info.at(0)->get_shard_name());
    }
    else
    {
        assembled_sql.append(table_name);
    }
        
    for (int64_t i = 0; i < ObStmt::get_column_size(); i++)
    {
        if (0 == i)
        {
            assembled_sql.append(" (");
            if (has_auto_incr_sharding_key())
            {
                assembled_sql.append(get_auto_incr_column_name());
                assembled_sql.append(",");
            }
        }
        
        assembled_sql.append(ObStmt::get_column_item(i)->column_name_);
            
        if (i != ObStmt::get_column_size() - 1)
        {
            assembled_sql.append(", ");
        }
        else
        {
            assembled_sql.append(") ");
        }
    }

    if (sub_query_id_ == OB_INVALID_ID)
    {
        if (insert_rows.empty())
        {
            assembled_sql.append("VALUES ");
                
            for (uint32_t i = 0; i < value_vectors_.size(); i++)
            {
                vector<uint64_t>& value_row = value_vectors_.at(i);
                for (uint32_t j = 0; j < value_row.size(); j++)
                {
                    string assembled_sql_tmp;
                    if (j == 0)
                    {
                        assembled_sql.append("(");
                    }
            
                    sql_expr = logical_plan->get_expr_by_id(value_row.at(j));
            
                    if (NULL == sql_expr)
                    {
                        ret = JD_ERR_LOGICAL_PLAN_FAILD;
                        jlog(WARNING, "insert value expr error!!!");
                        return ret;
                    }
            
                    //BEGIN: Added by qinbo: sql value type is same with column meta info
                    const ColumnItem* column_item = ObStmt::get_column_item(j);
                    
                    schema_table* table_schema = meta_reader::get_instance().get_table_schema_by_id_with_lock(result_plan.db_name, column_item->table_id_);
                    if (NULL == table_schema)
                    {
                        ret = JD_ERR_CONFIG_ROUTE_ERR;
                        jlog(WARNING, "Table with id %d should not be empty in table schema", column_item->table_id_);
                        return ret;
                    }
                    schema_column* column_schema =  table_schema->get_column_from_table(column_item->column_name_);
                    if (NULL == column_schema)
                    {
                        ret = JD_ERR_LOGICAL_PLAN_FAILD;
                        jlog(WARNING, "Column %s should not be empty in table schema", column_item->column_name_.data());
                        return ret;
                    }

                    if (column_schema->get_column_type() != sql_expr->get_expr()->get_expr_type())
                    {
                        if (((T_INT == column_schema->get_column_type())
                            ||(T_FLOAT == column_schema->get_column_type())
                            ||(T_DOUBLE == column_schema->get_column_type())
                            ||(T_BOOL == column_schema->get_column_type()))
                            &&(T_STRING == sql_expr->get_expr()->get_expr_type()))
                        {
                            ret = JD_ERR_SQL_VALUE_TYPE_ERR;
                            jlog(WARNING, "Insert column type is %d while insert value type is %d", 
                                        column_schema->get_column_type(),
                                        sql_expr->get_expr()->get_expr_type());
                            return ret;
                        }
                    }
                    //END: Added by qinbo: sql value type is same with column meta info
                    sql_expr->to_string(result_plan, assembled_sql_tmp);
                    assembled_sql.append(assembled_sql_tmp);
                        
                    if (j == value_row.size() - 1)
                    {
                        assembled_sql.append(")");
                    }
                    else
                    {
                        assembled_sql.append(",");
                    }
                }
            
                if (i != value_vectors_.size() - 1)
                {
                    assembled_sql.append(",");
                }
            }
        }
        else
        {
            assembled_sql.append(insert_rows);
        }
    }
    else
    {
        jlog(INFO, "We do not support sub query now!!");
    #if 0
        string assembled_sql_tmp;
        ObBasicStmt* query_stmt = logical_plan->get_query(sub_query_id_);

        if (NULL == query_stmt)
        {
            ret = JD_ERR_LOGICAL_PLAN_FAILD;
            jlog(WARNING, "can not get query stmt by query id!!!");
            return ret;
        }

        query_stmt->make_stmt_string(result_plan, assembled_sql_tmp);
        assembled_sql.append(assembled_sql_tmp);
    #endif    
    }
    
    return ret;
}


/**************************************************
Funtion     :   append_distributed_insert_items
Author      :   qinbo
Date        :   2014.1.6
Description :   append insert rows
Input       :   ResultPlan& result_plan,
                vector<uint32_t> &insert_rows_index,
                string &assembled_sql
Output      :   
 **************************************************/
int ObInsertStmt::append_distributed_insert_items(  ResultPlan& result_plan,
                                                    vector<uint32_t> &insert_rows_index,
                                                    string &assembled_sql)
{
    int& ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    ObSqlRawExpr* sql_expr = NULL;

    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);

    assembled_sql.append("VALUES ");
    
    for (uint32_t i = 0; i < insert_rows_index.size(); i++)
    {
        vector<uint64_t>& value_row = value_vectors_.at(insert_rows_index.at(i));
        for (uint32_t j = 0; j < value_row.size(); j++)
        {
            string assembled_sql_tmp;
            if (j == 0)
            {
                assembled_sql.append("(");
                
                if (has_auto_incr_sharding_key())
                {
                    uint64_t auto_id = get_auto_incr_id_value(insert_rows_index.at(i));
                    stringstream ss;
                    ss<< auto_id;
                    assembled_sql.append(ss.str());
                    assembled_sql.append(",");
                }
            }
            
            sql_expr = logical_plan->get_expr_by_id(value_row.at(j));
            
            if (NULL == sql_expr)
            {
                ret = JD_ERR_LOGICAL_PLAN_FAILD;
                jlog(WARNING, "insert value expr error!!!");
                return ret;
            }
            
            //BEGIN: Added by qinbo: sql value type is same with column meta info
            const ColumnItem* column_item = ObStmt::get_column_item(j);
            
            schema_table* table_schema = meta_reader::get_instance().get_table_schema_by_id_with_lock(result_plan.db_name, column_item->table_id_);
            if (NULL == table_schema)
            {
                ret = JD_ERR_CONFIG_ROUTE_ERR;
                jlog(WARNING, "Table with id %d should not be empty in table schema", column_item->table_id_);
                return ret;
            }
            schema_column* column_schema =  table_schema->get_column_from_table(column_item->column_name_);
            if (NULL == column_schema)
            {
                ret = JD_ERR_LOGICAL_PLAN_FAILD;
                jlog(WARNING, "Column %s should not be empty in table schema", column_item->column_name_.data());
                return ret;
            }
            
            if (column_schema->get_column_type() != sql_expr->get_expr()->get_expr_type())
            {
                if (((T_INT == column_schema->get_column_type())
                    ||(T_FLOAT == column_schema->get_column_type())
                    ||(T_DOUBLE == column_schema->get_column_type())
                    ||(T_BOOL == column_schema->get_column_type()))
                    &&(T_STRING == sql_expr->get_expr()->get_expr_type()))
                {
                    ret = JD_ERR_SQL_VALUE_TYPE_ERR;
                    jlog(WARNING, "Insert column type is %d while insert value type is %d", 
                                column_schema->get_column_type(),
                                sql_expr->get_expr()->get_expr_type());
                    return ret;
                }
            }

            sql_expr->to_string(result_plan, assembled_sql_tmp);
            assembled_sql.append(assembled_sql_tmp);
                
            if (j == value_row.size() - 1)
            {
                assembled_sql.append(")");
            }
            else
            {
                assembled_sql.append(",");
            }
        }

        if (i != insert_rows_index.size() - 1)
        {
            assembled_sql.append(",");
        }
    }
    
    return ret;
}



