/************************************************************
  Copyright (C), 2013-2015
  FileName:     jd_exec_plan.cpp
  Author:       qinbo      
  Version :          
  Date:         
  Description:  exec plan generator
  History: 
      <author>  <time>   <version >     <desc>
      qinbo    2013/9/15     1.0       build this moudle  
***********************************************************/

#include <algorithm>
#include "jd_exec_plan.h"


using namespace jdbd::common;
using namespace jdbd::sql;
using namespace std;

/**************************************************
Funtion     :   ExecPlanUnit
Author      :   qinbo
Date        :   2013.12.11
Description :   
Input       :   
Output      :   
return      :
 **************************************************/
ExecPlanUnit::ExecPlanUnit()
{   
    shard_info      = NULL;
}

/**************************************************
Funtion     :   ~ExecPlanUnit
Author      :   qinbo
Date        :   2013.12.11
Description :   
Input       :   
Output      :   
return      :
 **************************************************/

ExecPlanUnit::~ExecPlanUnit()
{   
}


/**************************************************
Funtion     :   SameLevelExecPlan
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
 **************************************************/
SameLevelExecPlan::SameLevelExecPlan()
{
    query_post_reduce_info  = NULL;
    sql_sent_goal           = SEND_SQL_TO_DATA_NODE;
    //is_1st_plan;
    //parent_sql_type;            //if need to be reparsed
}

/**************************************************
Funtion     :   ~SameLevelExecPlan
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
**************************************************/
SameLevelExecPlan::~SameLevelExecPlan()
{
    for(uint32_t i = 0; i < exec_plan_units.size(); i++)
    {
        exec_plan_units[i]->~ExecPlanUnit();
        parse_free(exec_plan_units.at(i));
        exec_plan_units.at(i)= NULL;
    }
    exec_plan_units.clear();

    if (NULL != query_post_reduce_info)
    {
        query_post_reduce_info->~QueryPostReduce();
        parse_free(query_post_reduce_info);
        query_post_reduce_info = NULL;
    }
}

/**************************************************
Funtion     :   get_parent_sql_type
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
**************************************************/
bool SameLevelExecPlan::get_parent_sql_type()
{
    return false;
}

/**************************************************
Funtion     :   set_parent_sql_type
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
**************************************************/
void SameLevelExecPlan::set_parent_sql_type(uint8_t parent_sql_type)
{
}

/**************************************************
Funtion     :   set_first_plan_true
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
**************************************************/
void SameLevelExecPlan::set_first_plan_true()
{
}

/**************************************************
Funtion     :   is_first_plan
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
**************************************************/
bool SameLevelExecPlan::is_first_plan()
{
    return false;
}

/**************************************************
Funtion     :   add_exec_plan_unit
Author      :   qinbo
Date        :   2013.11.1
Description :   add exec_plan unit
Input       :   
Output      :   
return      :
**************************************************/
void SameLevelExecPlan::add_exec_plan_unit(ExecPlanUnit* exec_plan_unit)
{
    exec_plan_units.push_back(exec_plan_unit);
}

/**************************************************
Funtion     :   get_all_same_level_exec_plans
Author      :   qinbo
Date        :   2013.11.
Description :   get all same level exec plans
Input       :   
Output      :   
return      :
 **************************************************/
vector<ExecPlanUnit*> &SameLevelExecPlan::get_all_exec_plan_units()
{
    return exec_plan_units;
}

/**************************************************
Funtion     :   FinalExecPlan
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
 **************************************************/
FinalExecPlan::FinalExecPlan()
{
    set_union_is_distinct(false);
}

/**************************************************
Funtion     :   ~FinalExecPlan
Author      :   qinbo
Date        :   2013.11.1
Description :   
Input       :   
Output      :   
return      :
 **************************************************/
FinalExecPlan::~FinalExecPlan()
{
    for(uint32_t i = 0; i < same_level_exec_plans.size(); i++)
    {
        same_level_exec_plans[i]->~SameLevelExecPlan();
        parse_free(same_level_exec_plans.at(i));
        same_level_exec_plans.at(i) = NULL;
    }
    same_level_exec_plans.clear();
}

/**************************************************
Funtion     :   set_union_is_distinct
Author      :   qinbo
Date        :   2013.11.7
Description :   set union is distinct
Input       :   
Output      :   
return      :
 **************************************************/
void FinalExecPlan::set_union_is_distinct(bool union_is_distinct_)
{
    union_is_distinct = union_is_distinct_;
}


/**************************************************
Funtion     :   get_union_is_distinct
Author      :   qinbo
Date        :   2013.11.7
Description :   check whether union is distinct
Input       :   
Output      :   
return      :
 **************************************************/
bool FinalExecPlan::get_union_is_distinct()
{
    return union_is_distinct;
}



/**************************************************
Funtion     :   add_same_level_exec_plan
Author      :   qinbo
Date        :   2013.11.7
Description :   add same_level_exec_plan into fanal_exec_plan
Input       :   
Output      :   
return      :
 **************************************************/
void FinalExecPlan::add_same_level_exec_plan(SameLevelExecPlan* same_level_exec_plan)
{
    same_level_exec_plans.push_back(same_level_exec_plan);
}


/**************************************************
Funtion     :   get_all_same_level_exec_plans
Author      :   qinbo
Date        :   2013.11.7
Description :   get all same level exec plans
Input       :   
Output      :   
return      :
 **************************************************/
vector<SameLevelExecPlan*> &FinalExecPlan::get_all_same_level_exec_plans()
{
    return same_level_exec_plans;
}

/**************************************************
Funtion     :   get_stmt
Author      :   qinbo
Date        :   2013.11.7
Description :   get stmt
Input       :   
Output      :   
return      :
 **************************************************/
template <class T>
int QueryActuator::get_stmt(
        ObLogicalPlan *logical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        T *& stmt)
{
    int& ret = err_stat.err_code_ = OB_SUCCESS;
    /* get statement */
    if (query_id == OB_INVALID_ID)
        stmt = dynamic_cast<T*> (logical_plan->get_main_stmt());
    else
        stmt = dynamic_cast<T*> (logical_plan->get_query(query_id));
    if (stmt == NULL)
    {
        err_stat.err_code_ = JD_ERR_PARSER_SYNTAX;
    }
    return ret;
}

/**************************************************
Funtion     :   QueryActuator
Author      :   qinbo
Date        :   2013.11.7
Description :   
Input       :   
Output      :   
return      :
 **************************************************/
QueryActuator::QueryActuator(string current_db_name, string host, string user)
{
    init_exec_plan(current_db_name,host,user);
}

/**************************************************
Funtion     :   ~QueryActuator
Author      :   qinbo
Date        :   2013.11.7
Description :   
Input       :   
Output      :   
return      :
 **************************************************/
QueryActuator::~QueryActuator()
{
    release_exec_plan();
}

/**************************************************
Funtion     :   popActuator
Author      :   qinbo
Date        :   2013.11.7
Description :   get stmt
Input       :   
Output      :   
return      :
 **************************************************/
FinalExecPlan* QueryActuator::popActuator()
{
    return NULL;
}

/**************************************************
Funtion     :   get_stmt
Author      :   qinbo
Date        :   2013.11.7
Description :   get stmt
Input       :   
Output      :   
return      :
 **************************************************/
void QueryActuator::pushActuator(FinalExecPlan* exec_plan)
{
}

/**************************************************
Funtion     :   is_all_plan_done
Author      :   qinbo
Date        :   2013.11.7
Description :   is_all_plan_done
Input       :   
Output      :   
return      :
 **************************************************/
bool QueryActuator::is_all_plan_done()
{
    return false;;
}

/**************************************************
Funtion     :   set_all_plan_done
Author      :   qinbo
Date        :   2013.11.7
Description :   all plan is done
Input       :   
Output      :   
return      :
 **************************************************/
void QueryActuator::set_all_plan_done()
{
}

/**************************************************
Funtion     :   set_next_plan_reparsed
Author      :   qinbo
Date        :   2013.11.7
Description :   set_next_plan_reparsed
Input       :   
Output      :   
return      :
 **************************************************/
void QueryActuator::set_next_plan_reparsed()
{
}

/**************************************************
Funtion     :   get_next_plan_reparsed
Author      :   qinbo
Date        :   2013.11.7
Description :   get_next_plan_reparsed
Input       :   
Output      :   
return      :
 **************************************************/
bool QueryActuator::get_next_plan_reparsed()
{
    return false;
}

/**************************************************
Funtion     :   reparse_next_plan
Author      :   qinbo
Date        :   2013.11.7
Description :   reparse netx plan
Input       :   
Output      :   
return      :
 **************************************************/
void QueryActuator::reparse_next_plan()
{
}

/**************************************************
Funtion     :   init_exec_plan
Author      :   qinbo
Date        :   2013.10.28
Description :   generate exec plan
Input       :   string current_db_name
Output      :   
 **************************************************/
int QueryActuator::init_exec_plan(string current_db_name, string host, string user)
{
    memset(&result, 0, sizeof(ParseResult));
    result_plan.name_pool_ = NULL;
    result_plan.plan_tree_ = NULL;
    result_plan.db_name    = current_db_name;
    result_plan.host_name  = host;
    result_plan.user_name  = user;
    final_exec_plan        = NULL;
    result_plan.has_sub_query= false;
    result_plan.is_show_sys_var = false;
    //BEGIN: add by tangchao 20131225
    query_type = ObBasicStmt::T_NONE;
    //END: add by tangchao 20131225
    
    return 0;
}

/**************************************************
Funtion     :   release_exec_plan
Author      :   qinbo
Date        :   2013.10.28
Description :   generate exec plan
Input       :   
Output      :   
 **************************************************/
void QueryActuator::release_exec_plan()
{
    query_type = ObBasicStmt::T_NONE;
    
    if (result_plan.plan_tree_)
    {
        ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
        logical_plan->~ObLogicalPlan();
        parse_free(result_plan.plan_tree_);
        result_plan.plan_tree_ = NULL;
    }

    if (final_exec_plan)
    {
        final_exec_plan->~FinalExecPlan();
        parse_free(final_exec_plan);
        final_exec_plan = NULL;
    }
    
    result_plan.name_pool_ = NULL;
    result_plan.has_sub_query= false;
    result_plan.is_show_sys_var = false;
    result_plan.meta_db_name = "";
}

/**************************************************
Funtion     :   set_final_exec_plan
Author      :   qinbo
Date        :   2013.11.1
Description :   set QueryActuator exec plans
Input       :   
Output      :   
 **************************************************/
void QueryActuator::set_final_exec_plan(FinalExecPlan* final_exec_plan_)
{
    final_exec_plan = final_exec_plan_;
}


/**************************************************
Funtion     :   get_final_exec_plan
Author      :   qinbo
Date        :   2013.11.1
Description :   set QueryActuator exec plans
Input       :   
Output      :   
 **************************************************/
FinalExecPlan*  QueryActuator::get_final_exec_plan( )
{
    return final_exec_plan;
}


/**************************************************
Funtion     :   generate_exec_plan
Author      :   qinbo
Date        :   2013.9.24
Description :   generate exec plan
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   
 **************************************************/
int QueryActuator::generate_exec_plan(
        string sql,
        uint32_t* index)
{
    int&        ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    bool        new_generated = false;
    uint64_t    query_id = OB_INVALID_ID;
    string      assemble_sql;
    FinalExecPlan* final_exec_plan = NULL;
    result_plan.meta_db_name.clear();
    result_plan.is_show_sys_var = false;

    string sql_tmp = sql;
    std::transform(sql.begin(), sql.end(), sql.begin(), ::tolower);
    
    if (0 == sql.compare(0, 3, "set"))
    {
        query_type = ObBasicStmt::T_VARIABLE_SET;
        jlog(INFO, "now executed set command: %s", sql.data());
        return ret;
    }
    else if (0 == sql.compare(0, 4, "show"))
    {
        query_type = ObBasicStmt::T_SHOW_DATABASES;
        jlog(INFO, "now executed show command: %s", sql.data());
        return ret;
    }

    sql = sql_tmp;
    
    if (parse_init(&result))
    {
        ret = JD_ERR_SQL_PARSER_WRONG;
        jlog(ERROR, "parse_init error!!!");
        return ret;
    }
    
    jlog(INFO, "<<Part 1 : SQL STRING>> %s" ,sql.data());
    if ((parse_sql(&result, sql.c_str(), sql.size()) != 0)
      || (NULL == result.result_tree_))
    {
        ret = JD_ERR_SQL_PARSER_WRONG;
        jlog(WARNING, "parse: %p, %p, %p, msg=[%s], start_col_=[%d], end_col_[%d], line_[%d], yycolumn[%d], yylineno_[%d]",
            result.yyscan_info_,
            result.result_tree_,
            result.malloc_pool_,
            result.error_msg_,
            result.start_col_,
            result.end_col_,
            result.line_,
            result.yycolumn_,
            result.yylineno_);
        destroy_tree(result.result_tree_);
        parse_terminate(&result);
        return ret;
    }

#if DEBUG_ON
    jlog(INFO, "<<Part 2 : PARSE TREE>>");
    print_tree(result.result_tree_, 0);
#endif

    switch (result.result_tree_->children_[0]->type_)
    {
        case T_SELECT:
        {
            ret = resolve_select_stmt(&result_plan, result.result_tree_->children_[0], query_id);
            break;
        }
        case T_DELETE:
        {
            ret = resolve_delete_stmt(&result_plan, result.result_tree_->children_[0], query_id);
            break;
        }
        case T_INSERT:
        {
            ret = resolve_insert_stmt(&result_plan, result.result_tree_->children_[0], query_id);
            break;
        }
        case T_UPDATE:
        {
            ret = resolve_update_stmt(&result_plan, result.result_tree_->children_[0], query_id);
            break;
        }
        case T_SHOW_DATABASES:
        case T_SHOW_TABLES:
        case T_SHOW_VARIABLES:
        case T_SHOW_COLUMNS:
        case T_SHOW_SCHEMA:
        case T_SHOW_CREATE_TABLE:
        case T_SHOW_TABLE_STATUS:
        case T_SHOW_SERVER_STATUS:
        case T_SHOW_WARNINGS:
        case T_SHOW_GRANTS:
        case T_SHOW_PARAMETERS:
        case T_SHOW_PROCESSLIST :
        {
            ret = resolve_show_stmt(&result_plan, result.result_tree_->children_[0], query_id);
            break;
        }
        
        case T_VARIABLE_SET:
        {
            ret = resolve_variable_set_stmt(&result_plan, result.result_tree_->children_[0], query_id);
            break;
        }
        default:
            ret = JD_ERR_SQL_NOT_SUPPORT;
            break;
    }


    destroy_tree(result.result_tree_);
    result.result_tree_ = NULL;
    parse_terminate(&result);

    if (OB_SUCCESS != ret)
    {
        jlog(INFO, "sql process error!");
        return ret;
    }
    
    ObLogicalPlan* logic_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
        
    if (logic_plan)
    {
#if DEBUG_ON
        jlog(INFO, "\n=======================================\n");
        jlog(INFO, "\n<<Part 2 : LOGICAL PLAN>>\n");
        logic_plan->print();
        logic_plan->make_stmt_string(result_plan);
#endif
        if (result_plan.is_show_sys_var)
        {
            query_type = ObBasicStmt::T_SELECT;
            return ret;
        }
        
        if (OB_LIKELY(NULL == final_exec_plan))
        {
            if ((final_exec_plan = (FinalExecPlan*) parse_malloc(sizeof (FinalExecPlan), NULL)) == NULL)
            {
                ret = JD_ERR_PARSER_MALLOC_FAILED;
                jlog(WARNING, "Can not malloc space for FinalExecPlan");
            }
            else
            {
                final_exec_plan = new(final_exec_plan) FinalExecPlan();
                new_generated = true;
            }
        }

        set_final_exec_plan(final_exec_plan);
        
        ObBasicStmt *stmt = NULL;
        if (ret == OB_SUCCESS)
        {
            stmt = logic_plan->get_main_stmt();
            if (stmt == NULL)
            {
                ret = JD_ERR_ILLEGAL_ID;
                jlog(WARNING, "Wrong query id to find query statement");
            }
        }

        if (OB_LIKELY(ret == OB_SUCCESS))
        {
            //BEGIN: add by tangchao 20131225
            query_type = stmt->get_stmt_type();
            //END: add by tangchao 20131225
            switch (stmt->get_stmt_type())
            {
                case ObBasicStmt::T_SELECT:
                    ret = gen_exec_plan_select(result_plan, final_exec_plan, result_plan.err_stat_, query_id, index);
                    break;
                case ObBasicStmt::T_DELETE:
                    ret = gen_exec_plan_delete(result_plan, final_exec_plan, result_plan.err_stat_, query_id, index);
                    break;
                case ObBasicStmt::T_INSERT:
                case ObBasicStmt::T_REPLACE:
                    ret = gen_exec_plan_insert(result_plan, final_exec_plan, result_plan.err_stat_, query_id, index);
                    break;
                case ObBasicStmt::T_UPDATE:
                    ret = gen_exec_plan_update(result_plan, final_exec_plan, result_plan.err_stat_, query_id, index);
                    break;
                case ObBasicStmt::T_SHOW_DATABASES:
                case ObBasicStmt::T_SHOW_TABLES:
                case ObBasicStmt::T_SHOW_VARIABLES:
                case ObBasicStmt::T_SHOW_COLUMNS:
                case ObBasicStmt::T_SHOW_SCHEMA:
                case ObBasicStmt::T_SHOW_CREATE_TABLE:
                case ObBasicStmt::T_SHOW_TABLE_STATUS:
                case ObBasicStmt::T_SHOW_SERVER_STATUS:
                case ObBasicStmt::T_SHOW_WARNINGS:
                case ObBasicStmt::T_SHOW_GRANTS:
                case ObBasicStmt::T_SHOW_PARAMETERS:
                case ObBasicStmt::T_SHOW_PROCESSLIST:
                case ObBasicStmt::T_VARIABLE_SET:
                    //ret = send_sql_to_config_server(result_plan, final_exec_plan, sql);
                    break;
                default:
                    ret = JD_ERR_SQL_NOT_SUPPORT;
                    jlog(INFO, "Unknown logical plan, stmt_type=%d", stmt->get_stmt_type());
                    break;
            }
        }
#if 0
        if (ret != OB_SUCCESS && new_generated && final_exec_plan != NULL)
        {
            final_exec_plan->~FinalExecPlan();
            parse_free(final_exec_plan);
            final_exec_plan = NULL;
        }
#endif        
    }
    if (OB_SUCCESS != ret)
    {
        jlog(INFO, "sql process error!");
    }
    else
    {
        jlog(INFO, "sql process OK!");
    }
    return ret;
}

/**************************************************
Funtion     :   gen_exec_plan_select
Author      :   qinbo
Date        :   2013.9.24
Description :   generate exec plan
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   

 SELECT
     [ALL | DISTINCT | DISTINCTROW ]
       [HIGH_PRIORITY]
       [STRAIGHT_JOIN]
       [SQL_SMALL_RESULT] [SQL_BIG_RESULT] [SQL_BUFFER_RESULT]
       [SQL_CACHE | SQL_NO_CACHE] [SQL_CALC_FOUND_ROWS]
     select_expr [, select_expr ...]
     [FROM table_references
       [PARTITION partition_list]
     [WHERE where_condition]
     [GROUP BY {col_name | expr | position}
       [ASC | DESC], ... [WITH ROLLUP]]
     [HAVING where_condition]
     [ORDER BY {col_name | expr | position}
       [ASC | DESC], ...]
     [LIMIT {[offset,] row_count | row_count OFFSET offset}]
     [PROCEDURE procedure_name(argument_list)]
     [INTO OUTFILE 'file_name'
         [CHARACTER SET charset_name]
         export_options
       | INTO DUMPFILE 'file_name'
       | INTO var_name [, var_name]]
     [FOR UPDATE | LOCK IN SHARE MODE]]
 **************************************************/
int QueryActuator::gen_exec_plan_select(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    int ret = err_stat.err_code_ = OB_SUCCESS;
    ObSelectStmt *select_stmt = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    //get statement
    if (OB_SUCCESS != (ret = get_stmt(logical_plan, err_stat, query_id, select_stmt)))
    {
        ret = JD_ERR_GEN_PLAN;
        jlog(WARNING, "Can not get stmt");
        return ret;
    }

    if (result_plan.has_sub_query)
    {
        ret = JD_ERR_SQL_NO_SUBQUERY;
        jlog(WARNING, "Now we DO NOT support sub-query");
        return ret;
    }

    if (ObSelectStmt::UNION == select_stmt->get_set_op())
    {
        if (select_stmt->is_set_distinct())
        {
            ret = JD_ERR_SQL_NOT_SUPPORT;
            jlog(WARNING, "Now we DO NOT support 'UNION DISTINCT'");
            return ret;
            //physical_plan->set_union_is_distinct(true);
        }
        
        ret = gen_exec_plan_select(result_plan, physical_plan, err_stat, select_stmt->get_left_query_id(), index);
        if (OB_SUCCESS == ret)
        {
            ret = gen_exec_plan_select(result_plan, physical_plan, err_stat, select_stmt->get_right_query_id(), index);
        }
    }
    else if (ObSelectStmt::NONE == select_stmt->get_set_op())
    {
    #if 0
        if (select_stmt->is_from_item_with_join())
        {
            ret = JD_ERR_GEN_PLAN;
            jlog(WARNING, "Can not support stmt with JOIN now");
        }
        else
    #endif
        {
            //related to 1 table
            if (1 == select_stmt->get_from_item_size())
            {
                ret = generate_select_plan_single_table(result_plan, physical_plan, err_stat, query_id, index);
            }
            //related to >1 tables
            else if (1 < select_stmt->get_from_item_size())
            {
                ret = generate_select_plan_multi_table(result_plan, physical_plan, err_stat, query_id, index);
            }
        }
    }
    else
    {
        ret = JD_ERR_SQL_NOT_SUPPORT;
        jlog(WARNING, "Now we DO NOT support 'union distinct' query");
    }

    return ret;
}


/**************************************************
Funtion     :   gen_exec_plan_update
Author      :   qinbo
Date        :   2013.9.24
Description :   generate exec plan
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   

 Single-table syntax:
 UPDATE [LOW_PRIORITY] [IGNORE] table_reference
 SET col_name1={expr1|DEFAULT} [, col_name2={expr2|DEFAULT}] ...
 [WHERE where_condition]
 [ORDER BY ...]
 
 [LIMIT row_count]
 Multiple-table syntax:
 UPDATE [LOW_PRIORITY] [IGNORE] table_references
 SET col_name1={expr1|DEFAULT} [, col_name2={expr2|DEFAULT}] ...
 [WHERE where_condition]
 **************************************************/
int QueryActuator::gen_exec_plan_update(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    int ret = err_stat.err_code_ = OB_SUCCESS;
    int where_ret = WHERE_IS_OR_AND;
    ObUpdateStmt *update_stmt = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    string table_name;
    string sql_exec_plan_unit;
    schema_column*  column_info = NULL;
    ObSqlRawExpr*   sql_expr = NULL;
    uint64_t        column_id= 0;
    uint64_t        i = 0;
    vector<string>  acl_checked_tables;
    
    // get statement
    if (OB_SUCCESS != (ret = get_stmt(logical_plan, err_stat, query_id, update_stmt)))
    {
        ret = JD_ERR_GEN_PLAN;
        jlog(WARNING, "Can not get stmt");
        return ret;
    }
    
    if (result_plan.has_sub_query)
    {
        ret = JD_ERR_SQL_NO_SUBQUERY;
        jlog(WARNING, "Now we DO NOT support sub-query");
        return ret;
    }

    table_name.assign(update_stmt->get_table_item_by_id(update_stmt->get_update_table_id())->table_name_);
    schema_table* table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, table_name);
    if (NULL == table_schema)
    {
        ret = JD_ERR_CONFIG_ROUTE_ERR;
        jlog(WARNING, "Table %s should not be empty in table schema", table_name.data());
        return ret;
    }

    SameLevelExecPlan* exec_plan = (SameLevelExecPlan*) parse_malloc(sizeof (SameLevelExecPlan), NULL);
    if (exec_plan == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for SameLevelExecPlan");
        return ret;
    }
    else
    {
        exec_plan = new(exec_plan) SameLevelExecPlan();
        physical_plan->add_same_level_exec_plan(exec_plan);
    }

    //BEGIN: set acl 
    acl_checked_tables.push_back(table_name);
    if (OB_SUCCESS != (ret = check_acl(result_plan.db_name, result_plan.host_name, result_plan.user_name, ObBasicStmt::T_UPDATE, acl_checked_tables)))
    {
        return ret;
    }
    //END: set acl

    //this table is not distributed table
    if (!table_schema->get_is_distributed_table())
    {
        if (1 != table_schema->get_all_shards().size())
        {
            ret = JD_ERR_SHARD_NUM_WRONG;
            jlog(WARNING, "shard manage wrong");
            return ret;
        }

        ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
        return ret;
    }
    //this table is distributed table
    else
    {
        if (0 == table_schema->get_all_shards().size())
        {
            ret = JD_ERR_SHARD_NUM_WRONG;
            jlog(WARNING, "shard manage wrong");
            return ret;
        }

        if (0 == update_stmt->get_update_column_count())
        {
            ret = JD_ERR_SQL_NOT_SUPPORT;
            jlog(WARNING, "MUST set column name in UPDATE OPERATION");
            return ret;
        }

        for (i = 0; i < update_stmt->get_update_column_count(); i++)
        {            
            if(JD_INVALID_ARGUMENT != update_stmt->get_update_column_id(i, column_id))
            {
                column_info = table_schema->get_column_from_table_by_id(column_id);
                if (column_info->is_sharding_key())
                {
                    ret = JD_ERR_SQL_NOT_SUPPORT;
                    jlog(WARNING, "Can not update partition key column");
                    return ret;
                }
            }
        }

        vector<schema_shard*>   &table_all_shards= table_schema->get_all_shards();
        vector<uint64_t>        &expr_ids        = update_stmt->get_where_exprs();
        //if there is no where conditions
        if (0 == expr_ids.size())
        {
            //ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
            ret = JD_ERR_NOT_SUPPORT_MULTI_SHARD_WRITE;
            jlog(WARNING, "Now we DO NOT support multi shard write operation");
            return ret;
        }
        else
        {
            //decompose where conditions into seperate sql which is linked by AND
            sql_expr = logical_plan->get_expr_by_id(expr_ids.at(0));
            vector<vector<ObRawExpr*> > atomic_exprs_array;
            
            where_ret = update_stmt->decompose_where_items(sql_expr->get_expr(), atomic_exprs_array);

            if (WHERE_IS_OR_AND == where_ret)
            {
                multimap<uint32_t, vector<ObRawExpr*> > opted_raw_exprs;
                schema_shard* shard_key = NULL;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map1;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map_tmp;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map2;
                pair<multimap<uint32_t, vector<ObRawExpr*> >::iterator, multimap<uint32_t, vector<ObRawExpr*> >::iterator> raw_exprs_same_shard;
            
                ret = reparse_where_with_route_for_one_table(result_plan,
                        table_schema,
                        atomic_exprs_array,
                        opted_raw_exprs,
                        table_all_shards);

                if (OB_SUCCESS != ret)
                {
                    return ret;
                }
                
                for (p_map1 = opted_raw_exprs.begin(); p_map1 != opted_raw_exprs.end();)
                {
                    vector<vector<ObRawExpr*> > final_exprs_array;
                    string assembled_sql;
                    shard_key = table_all_shards.at(p_map1->first);
                    vector<schema_shard*> shard_tmp;
                    shard_tmp.push_back(shard_key);
                    raw_exprs_same_shard = opted_raw_exprs.equal_range(p_map1->first);
                    for (p_map2 = raw_exprs_same_shard.first; p_map2 != raw_exprs_same_shard.second; p_map2++)
                    {
                        final_exprs_array.push_back((*p_map2).second);
                    }
            
                    if (opted_raw_exprs.size() > opted_raw_exprs.count(p_map1->first))
                    {
                        ret = JD_ERR_NOT_SUPPORT_MULTI_SHARD_WRITE;
                        jlog(WARNING, "Now we DO NOT support multi shard write operation");
                        return ret;
                    }
                    
                    for (i = 0; i < opted_raw_exprs.count(p_map1->first); i++)
                    {
                        p_map1++;
                    }
                    
                    string where_conditions;
                    append_distributed_where_items(result_plan, where_conditions, final_exprs_array);
                    update_stmt->make_exec_plan_unit_string(result_plan, where_conditions, shard_tmp, assembled_sql);
            
                    ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
                    if (exec_plan_unit == NULL)
                    {
                        ret = JD_ERR_PARSER_MALLOC_FAILED;
                        jlog(WARNING, "Can not malloc space for ExecPlanUnit");
                        return ret;
                    }
                    else
                    {
                        exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
                    }

                    exec_plan_unit->set_exec_unit_sql(assembled_sql);
                    jlog(INFO, "exec_plan_unit shard name: %s" ,shard_key->get_shard_name().data());
                    jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());
                    exec_plan_unit->set_exec_uint_shard_info(shard_key);
            
                    //add exec_plan_unit
                    exec_plan->add_exec_plan_unit(exec_plan_unit);
                }
            }
            else if (WHERE_IS_SUBQUERY == where_ret)
            {
                ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
                return ret;
            }
        }
    }

    return ret;
}


/**************************************************
Funtion     :   gen_exec_plan_replace
Author      :   qinbo
Date        :   2013.9.24
Description :   generate exec plan
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   
 **************************************************/
int QueryActuator::gen_exec_plan_replace(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    return gen_exec_plan_insert(result_plan, physical_plan, result_plan.err_stat_, query_id, index);
}


/**************************************************
Funtion     :   gen_exec_plan_delete
Author      :   qinbo
Date        :   2013.9.24
Description :   generate exec plan
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   

 DELETE [LOW_PRIORITY] [QUICK] [IGNORE] FROM tbl_name
 [PARTITION (partition_name,...)]
 [WHERE where_condition]
 [ORDER BY ...]
 [LIMIT row_count]
 **************************************************/
int QueryActuator::gen_exec_plan_delete(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    int ret = err_stat.err_code_ = OB_SUCCESS;
    int where_ret = WHERE_IS_OR_AND;
    ObDeleteStmt *delete_stmt = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    string table_name;
    string sql_exec_plan_unit;
    vector<string> acl_checked_tables;
    ObSqlRawExpr* sql_expr = NULL;
    uint32_t    i = 0;

    //get statement
    if (OB_SUCCESS != (ret = get_stmt(logical_plan, err_stat, query_id, delete_stmt)))
    {
        ret = JD_ERR_GEN_PLAN;
        jlog(WARNING, "Can not get stmt");
        return ret;
    }

    if (result_plan.has_sub_query)
    {
        ret = JD_ERR_SQL_NO_SUBQUERY;
        jlog(WARNING, "Now we DO NOT support sub-query");
        return ret;
    }

    table_name.assign(delete_stmt->get_table_item_by_id(delete_stmt->get_delete_table_id())->table_name_);
    schema_table* table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, table_name);
    if (NULL == table_schema)
    {
        ret = JD_ERR_CONFIG_ROUTE_ERR;
        jlog(WARNING, "Table %s should not be empty in table schema", table_name.data());
        return ret;
    }

    SameLevelExecPlan* exec_plan = (SameLevelExecPlan*) parse_malloc(sizeof (SameLevelExecPlan), NULL);
    if (exec_plan == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for SameLevelExecPlan");
        return ret;
    }
    else
    {
        exec_plan = new(exec_plan) SameLevelExecPlan();
        physical_plan->add_same_level_exec_plan(exec_plan);
    }

    //BEGIN: set acl 
    acl_checked_tables.push_back(table_name);
    if (OB_SUCCESS != (ret = check_acl(result_plan.db_name, result_plan.host_name, result_plan.user_name, ObBasicStmt::T_DELETE, acl_checked_tables)))
    {
        return ret;
    }
    //END: set acl

    //this table is not distributed table
    if (!table_schema->get_is_distributed_table())
    {
        if (1 != table_schema->get_all_shards().size())
        {
            ret = JD_ERR_SHARD_NUM_WRONG;
            jlog(WARNING, "shard manage wrong");
            return ret;
        }

        ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
        return ret;
    }
    //this table is distributed table
    else
    {
        vector<schema_shard*>   &table_all_shards= table_schema->get_all_shards();
        vector<uint64_t>        &expr_ids        = delete_stmt->get_where_exprs();
        //if there is no where conditions
        if (0 == expr_ids.size())
        {
            //ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
            ret = JD_ERR_NOT_SUPPORT_MULTI_SHARD_WRITE;
            jlog(WARNING, "Now we DO NOT support multi shard write operation");
            return ret;
        }
        else
        {
            //decompose where conditions into seperate sql which is linked by AND
            sql_expr = logical_plan->get_expr_by_id(expr_ids.at(0));
            vector<vector<ObRawExpr*> > atomic_exprs_array;
            
            where_ret = delete_stmt->decompose_where_items(sql_expr->get_expr(), atomic_exprs_array);

            if (WHERE_IS_OR_AND == where_ret)
            {
                multimap<uint32_t, vector<ObRawExpr*> > opted_raw_exprs;
                schema_shard* shard_key = NULL;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map1;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map_tmp;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map2;
                pair<multimap<uint32_t, vector<ObRawExpr*> >::iterator, multimap<uint32_t, vector<ObRawExpr*> >::iterator> raw_exprs_same_shard;

                ret = reparse_where_with_route_for_one_table(result_plan,
                        table_schema,
                        atomic_exprs_array,
                        opted_raw_exprs,
                        table_all_shards);
                
                if (OB_SUCCESS != ret)
                {
                    return ret;
                }
                for (p_map1 = opted_raw_exprs.begin(); p_map1 != opted_raw_exprs.end();)
                {
                    vector<vector<ObRawExpr*> > final_exprs_array;
                    string assembled_sql;
                    shard_key = table_all_shards.at(p_map1->first);
                    vector<schema_shard*> shard_tmp;
                    shard_tmp.push_back(shard_key);
                    raw_exprs_same_shard = opted_raw_exprs.equal_range(p_map1->first);
                    for (p_map2 = raw_exprs_same_shard.first; p_map2 != raw_exprs_same_shard.second; p_map2++)
                    {
                        final_exprs_array.push_back((*p_map2).second);
                    }

                    if (opted_raw_exprs.size() > opted_raw_exprs.count(p_map1->first))
                    {
                        ret = JD_ERR_NOT_SUPPORT_MULTI_SHARD_WRITE;
                        jlog(WARNING, "Now we DO NOT support multi shard write operation");
                        return ret;
                    }

                    for (i = 0; i < opted_raw_exprs.count(p_map1->first); i++)
                    {
                        p_map1++;
                    }

                    string where_conditions;
                    append_distributed_where_items(result_plan, where_conditions, final_exprs_array);
                    delete_stmt->make_exec_plan_unit_string(result_plan, where_conditions, shard_tmp, assembled_sql);

                    ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
                    if (exec_plan_unit == NULL)
                    {
                        ret = JD_ERR_PARSER_MALLOC_FAILED;
                        jlog(WARNING, "Can not malloc space for ExecPlanUnit");
                        return ret;
                    }
                    else
                    {
                        exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
                    }

                    exec_plan_unit->set_exec_unit_sql(assembled_sql);
                    jlog(INFO, "exec_plan_unit shard name: %s" ,shard_key->get_shard_name().data());
                    jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());
                    exec_plan_unit->set_exec_uint_shard_info(shard_key);

                    //add exec_plan_unit
                    exec_plan->add_exec_plan_unit(exec_plan_unit);
                }
            }
            else if (WHERE_IS_SUBQUERY == where_ret)
            {
                ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
                return ret;
            }
        }
    }

    return ret;

}


/**************************************************
Funtion     :   gen_exec_plan_insert
Author      :   qinbo
Date        :   2013.9.24
Description :   generate exec plan
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   
 **************************************************/
int QueryActuator::gen_exec_plan_insert(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    int ret = err_stat.err_code_ = OB_SUCCESS;
    ObInsertStmt *insert_stmt = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    string table_name;
    string sql_exec_plan_unit;
    schema_shard*   shard_info = NULL;
    vector<string> acl_checked_tables;
    
    // get statement
    if (OB_SUCCESS != (ret = get_stmt(logical_plan, err_stat, query_id, insert_stmt)))
    {
        ret = JD_ERR_GEN_PLAN;
        jlog(WARNING, "Can not get stmt");
        return ret;
    }

    if (result_plan.has_sub_query)
    {
        ret = JD_ERR_SQL_NO_SUBQUERY;
        jlog(WARNING, "Now we DO NOT support sub-query");
        return ret;
    }

    table_name.assign(insert_stmt->get_table_item_by_id(insert_stmt->get_table_id())->table_name_);
    schema_table* table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, table_name);
    if (NULL == table_schema)
    {
        ret = JD_ERR_CONFIG_ROUTE_ERR;
        jlog(WARNING, "Table %s should not be empty in table schema", table_name.data());
        return ret;
    }

    SameLevelExecPlan* exec_plan = (SameLevelExecPlan*) parse_malloc(sizeof (SameLevelExecPlan), NULL);
    if (exec_plan == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for SameLevelExecPlan");
        return ret;
    }
    else
    {
        exec_plan = new(exec_plan) SameLevelExecPlan();
        physical_plan->add_same_level_exec_plan(exec_plan);
    }

    //BEGIN: set acl 
    acl_checked_tables.push_back(table_name);
    if (OB_SUCCESS != (ret = check_acl(result_plan.db_name, result_plan.host_name, result_plan.user_name, ObBasicStmt::T_INSERT, acl_checked_tables)))
    {
        return ret;
    }
    //END: set acl

    vector<schema_shard*>  &all_table_shards = table_schema->get_all_shards();
    if (0 == all_table_shards.size())
    {
        ret = JD_ERR_SHARD_NUM_WRONG;
        jlog(WARNING, "shard manage wrong");
        return ret;
    }

    //this table is not distributed table
    if (!table_schema->get_is_distributed_table())
    {
        if (1 != table_schema->get_all_shards().size())
        {
            ret = JD_ERR_SHARD_NUM_WRONG;
            jlog(WARNING, "shard manage wrong");
            return ret;
        }

        ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
        if (exec_plan == NULL)
        {
            ret = JD_ERR_PARSER_MALLOC_FAILED;
            jlog(WARNING, "Can not malloc space for ExecPlanUnit");
            return ret;
        }
        else
        {
            exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
        }

        //actually, this sql only sent to one shard
        ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
        return ret;
    }
    //this table is distributed table
    else
    {
        if (0 == insert_stmt->get_column_size())
        {
            ret = JD_ERR_SQL_NOT_SUPPORT;
            jlog(WARNING, "MUST set column name in INSERT OPERATION");
            return ret;
        }

        multimap<uint32_t, uint32_t > sorted_insert_rows_value;
        bool     has_auto_incr_sharding_key = false;
        ret = reparse_insert_stmt_rows_value(result_plan, insert_stmt, table_schema,
                                            sorted_insert_rows_value, //first: shard index;  second: row index
                                            all_table_shards,
                                            has_auto_incr_sharding_key);

        if (OB_SUCCESS == ret)
        {
            multimap<uint32_t, uint32_t>::iterator p_map1;
            multimap<uint32_t, uint32_t>::iterator p_map_tmp;
            multimap<uint32_t, uint32_t>::iterator p_map2;
            pair<multimap<uint32_t, uint32_t>::iterator, multimap<uint32_t, uint32_t>::iterator> raw_exprs_same_shard;
            
            for (p_map1 = sorted_insert_rows_value.begin(); p_map1 != sorted_insert_rows_value.end();)
            {
                vector<uint32_t> insert_rows_index;
                string insert_rows;
                string assembled_sql;
                shard_info = all_table_shards.at(p_map1->first);
                vector<schema_shard*> shard_tmp;
                shard_tmp.push_back(shard_info);
                raw_exprs_same_shard = sorted_insert_rows_value.equal_range(p_map1->first);
                for (p_map2 = raw_exprs_same_shard.first; p_map2 != raw_exprs_same_shard.second; p_map2++)
                {
                    insert_rows_index.push_back((*p_map2).second);
                }

                if (sorted_insert_rows_value.size() > sorted_insert_rows_value.count(p_map1->first))
                {
                    ret = JD_ERR_NOT_SUPPORT_MULTI_SHARD_WRITE;
                    jlog(WARNING, "Now we DO NOT support multi shard write operation");
                    return ret;
                }
                
                for (uint32_t i = 0; i < sorted_insert_rows_value.count(p_map1->first); i++)
                {
                    p_map1++;
                }

                if (has_auto_incr_sharding_key)
                {
                    insert_stmt->set_auto_incr_sharding_key(true);
                    insert_stmt->set_auto_incr_column_name(table_schema->get_sequence_name());
                }
                else
                {
                    insert_stmt->set_auto_incr_sharding_key(false);
                }
                if (OB_SUCCESS != (ret = insert_stmt->append_distributed_insert_items(result_plan, insert_rows_index, insert_rows)))
                {
                    return ret;
                }
                
                insert_stmt->make_exec_plan_unit_string(result_plan, insert_rows, shard_tmp, assembled_sql);
                ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
                if (exec_plan_unit == NULL)
                {
                    ret = JD_ERR_PARSER_MALLOC_FAILED;
                    jlog(WARNING, "Can not malloc space for ExecPlanUnit");
                    return ret;
                }
                else
                {
                    exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
                }
            
                exec_plan_unit->set_exec_unit_sql(assembled_sql);
                jlog(INFO, "exec_plan_unit shard name: %s" ,shard_info->get_shard_name().data());
                jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());
                exec_plan_unit->set_exec_uint_shard_info(shard_info);
            
                //add exec_plan_unit
                exec_plan->add_exec_plan_unit(exec_plan_unit);
            }
        }
    }
    
    return ret;
}

/**************************************************
Funtion     :   generate_select_plan_single_table
Author      :   qinbo
Date        :   2013.9.27
Description :   generate select exec plan with single table
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   
return      :
 **************************************************/
int QueryActuator::generate_select_plan_single_table(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    int ret = err_stat.err_code_ = OB_SUCCESS;
    int where_ret = WHERE_IS_OR_AND;
    ObSelectStmt *select_stmt = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    string table_name;
    string sql_exec_plan_unit;
    ObSqlRawExpr* sql_expr = NULL;
    vector<string> acl_checked_tables;

    // get statement
    if (OB_SUCCESS != (ret = get_stmt(logical_plan, err_stat, query_id, select_stmt)))
    {
        ret = JD_ERR_GEN_PLAN;
        jlog(WARNING, "Can not get stmt");
        return ret;
    }

    const vector<FromItem> &from_items = select_stmt->get_all_from_items();
    if (from_items.size() > 1)
    {
        ret = JD_ERR_LOGICAL_TREE_WRONG;
        jlog(WARNING, "From table size is not right");
        return ret;
    }


    if (select_stmt->get_joined_table_size() == 0)
    {
        table_name = from_items.at(0).table_name_;
    }
    else  if (!select_stmt->is_join_tables_binded(result_plan,select_stmt, table_name))
    {
        ret = JD_ERR_SQL_NOT_SUPPORT;
        return ret;
    }
    
    schema_table* table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, table_name);
    if (NULL == table_schema)
    {
        ret = JD_ERR_CONFIG_ROUTE_ERR;
        jlog(WARNING, "Table %s should not be empty in table schema", table_name.data());
        return ret;
    }
    
    SameLevelExecPlan* exec_plan = (SameLevelExecPlan*) parse_malloc(sizeof (SameLevelExecPlan), NULL);
    if (exec_plan == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for SameLevelExecPlan");
        return ret;
    }
    else
    {
        exec_plan = new(exec_plan) SameLevelExecPlan();
        physical_plan->add_same_level_exec_plan(exec_plan);
    }

    //BEGIN: set acl 
    if (!from_items.at(0).is_joined_)
    {
        acl_checked_tables.push_back(from_items.at(0).table_name_);
    }
    else
    {
        JoinedTable* joined_table = select_stmt->get_joined_table(from_items.at(0).table_id_);
        acl_checked_tables.push_back(select_stmt->get_table_item_by_id(joined_table->table_ids_.at(0))->table_name_);
        acl_checked_tables.push_back(select_stmt->get_table_item_by_id(joined_table->table_ids_.at(1))->table_name_);
    }
    if (OB_SUCCESS != (ret = check_acl(result_plan.db_name, result_plan.host_name, result_plan.user_name, ObBasicStmt::T_SELECT, acl_checked_tables)))
    {
        return ret;
    }
    //END: set acl

    //set sql post process info
    QueryPostReduce* query_post_reduce_info = (QueryPostReduce*) parse_malloc(sizeof (QueryPostReduce), NULL);
    if (query_post_reduce_info == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for QueryPostReduce");
        return ret;
    }
    else
    {
        query_post_reduce_info = new(query_post_reduce_info) QueryPostReduce();
        query_post_reduce_info->set_post_reduce_info(result_plan, select_stmt);
        exec_plan->set_query_post_reduce_info(query_post_reduce_info);
    }

    //this table is not distributed table
    if (!table_schema->get_is_distributed_table())
    {
        if (1 != table_schema->get_all_shards().size())
        {
            ret = JD_ERR_SHARD_NUM_WRONG;
            jlog(WARNING, "shard manage wrong");
            return ret;
        }
        
        select_stmt->set_sql_dispatched_multi_shards(false);
        ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
        return ret;
    }
    //this table is distributed table
    else
    {
        vector<schema_shard*>   &table_all_shards= table_schema->get_all_shards();
        vector<uint64_t>        &expr_ids        = select_stmt->get_where_exprs();
        /*if there is no where conditions*/
        if (0 == expr_ids.size())
        {
            if (select_stmt->is_has_avg_in_having())
            {
                ret = JD_ERR_AVG_NOT_SUPPORT_IN_SELECT_HAVING;
                jlog(WARNING, "do not support multi shard select query with avg OP in having clause for the result is not precise.");
                return ret;
            }
            select_stmt->set_sql_dispatched_multi_shards(true);
            ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
            return ret;
        }
        else
        {
            //decompose where conditions into seperate sql which is linked by AND
            sql_expr = logical_plan->get_expr_by_id(expr_ids.at(0));
            vector<vector<ObRawExpr*> > atomic_exprs_array;

            where_ret = select_stmt->decompose_where_items(sql_expr->get_expr(), atomic_exprs_array);
            
            if (WHERE_IS_OR_AND == where_ret)
            {
                multimap<uint32_t, vector<ObRawExpr*> > opted_raw_exprs;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map1;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map_tmp;
                multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map2;
                pair<multimap<uint32_t, vector<ObRawExpr*> >::iterator, multimap<uint32_t, vector<ObRawExpr*> >::iterator> raw_exprs_same_shard;

                ret = reparse_where_with_route_for_one_table(result_plan,
                        table_schema,
                        atomic_exprs_array,
                        opted_raw_exprs,
                        table_all_shards);

                if (OB_SUCCESS != ret)
                {
                    return ret;
                }
                
                set_sql_dispatched_info(select_stmt, opted_raw_exprs);
                for (p_map1 = opted_raw_exprs.begin(); p_map1 != opted_raw_exprs.end();)
                {
                    vector<vector<ObRawExpr*> > final_exprs_array;
                    string assembled_sql;
                    schema_shard* shard_key = NULL;
                    shard_key = table_all_shards.at(p_map1->first);
                    vector<schema_shard*> shard_tmp;
                    shard_tmp.push_back(shard_key);
                    raw_exprs_same_shard = opted_raw_exprs.equal_range(p_map1->first);
                    for (p_map2 = raw_exprs_same_shard.first; p_map2 != raw_exprs_same_shard.second; p_map2++)
                    {
                        final_exprs_array.push_back((*p_map2).second);
                    }

                    if ((opted_raw_exprs.size() > opted_raw_exprs.count(p_map1->first))&&(select_stmt->is_has_avg_in_having()))
                    {
                        ret = JD_ERR_AVG_NOT_SUPPORT_IN_SELECT_HAVING;
                        jlog(WARNING, "do not support multi shard select query with avg OP in having clause for the result is not precise.");
                        return ret;
                    }

                    for (uint32_t i = 0; i < opted_raw_exprs.count(p_map1->first); i++)
                    {
                        p_map1++;
                    }

                    string where_conditions;
                    
                    append_distributed_where_items(result_plan, where_conditions, final_exprs_array);
                    select_stmt->make_exec_plan_unit_string(result_plan, where_conditions, shard_tmp, assembled_sql);
                    
                    ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
                    if (exec_plan_unit == NULL)
                    {
                        ret = JD_ERR_PARSER_MALLOC_FAILED;
                        jlog(WARNING, "Can not malloc space for ExecPlanUnit");
                        return ret;
                    }
                    else
                    {
                        exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
                    }

                    exec_plan_unit->set_exec_unit_sql(assembled_sql);
                    jlog(INFO, "exec_plan_unit shard name: %s" ,shard_key->get_shard_name().data());
                    jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());
                    exec_plan_unit->set_exec_uint_shard_info(shard_key);

                    //add exec_plan_unit
                    exec_plan->add_exec_plan_unit(exec_plan_unit);
                }
            }
            else if (WHERE_IS_SUBQUERY == where_ret)
            {
                ret = distribute_sql_to_all_shards( result_plan, query_id, table_schema, exec_plan);
                return ret;
            }
        }
    }

    return ret;
}

/**************************************************
Funtion     :   generate_select_plan_multi_table
Author      :   qinbo
Date        :   2013.9.27
Description :   generate select exec plan with multi table
Input       :   ObLogicalPlan *logical_plan,
                FinalExecPlan*& physical_plan,
                ErrStat& err_stat
Output      :   
 **************************************************/
int QueryActuator::generate_select_plan_multi_table(
        ResultPlan& result_plan,
        FinalExecPlan* physical_plan,
        ErrStat& err_stat,
        const uint64_t& query_id,
        uint32_t* index)
{
    int ret = err_stat.err_code_ = OB_SUCCESS;
    int where_ret = WHERE_IS_OR_AND;
    ObSelectStmt *select_stmt = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    FromItem  from_item;
    ObSqlRawExpr* sql_expr = NULL;
    vector<vector<schema_shard*> >  all_binding_table_shards;
    vector<schema_shard*>           one_binding_table_shards;
    uint32_t i = 0;
    vector<string>  acl_checked_tables;

    // get statement
    if (OB_SUCCESS != (ret = get_stmt(logical_plan, err_stat, query_id, select_stmt)))
    {
        ret = JD_ERR_GEN_PLAN;
        jlog(WARNING, "Can not get stmt");
        return ret;
    }

    const vector<FromItem> &from_items = select_stmt->get_all_from_items();
    if (from_items.size() <= 1)
    {
        ret = JD_ERR_LOGICAL_TREE_WRONG;
        jlog(WARNING, "From table size is not right");
        return ret;
    }

    if (select_stmt->get_joined_table_size() > 0)
    {
        ret = JD_ERR_SQL_NOT_SUPPORT;
        jlog(WARNING, "Now we DO NOT support multi-table join query");
        return ret;
    }
    
    //now we only support binding tables in multi-table query
    if(!is_from_tables_binding(result_plan,from_items))
    {
        ret = JD_ERR_SQL_NOT_SUPPORT;
        jlog(WARNING, "Can not support no-binding tables query");
        return ret;
    }

    SameLevelExecPlan* exec_plan = (SameLevelExecPlan*) parse_malloc(sizeof (SameLevelExecPlan), NULL);
    if (exec_plan == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for SameLevelExecPlan");
        return ret;
    }
    else
    {
        exec_plan = new(exec_plan) SameLevelExecPlan();
        physical_plan->add_same_level_exec_plan(exec_plan);
    }

    //BEGIN: set acl 
    for (i = 0; i< from_items.size(); i++)
    {
        from_item = from_items.at(i);
        if (!from_item.is_joined_)
        {
            acl_checked_tables.push_back(from_item.table_name_);
        }
        else
        {
            JoinedTable* joined_table = select_stmt->get_joined_table(from_item.table_id_);
            acl_checked_tables.push_back(select_stmt->get_table_item_by_id(joined_table->table_ids_.at(0))->table_name_);
            acl_checked_tables.push_back(select_stmt->get_table_item_by_id(joined_table->table_ids_.at(1))->table_name_);
        }
    }
    i = 0;
    if (OB_SUCCESS != (ret = check_acl(result_plan.db_name, result_plan.host_name, result_plan.user_name, ObBasicStmt::T_SELECT, acl_checked_tables)))
    {
        return ret;
    }
    //END: set acl

    //set sql post process info
    QueryPostReduce* query_post_reduce_info = (QueryPostReduce*) parse_malloc(sizeof (QueryPostReduce), NULL);
    if (query_post_reduce_info == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for QueryPostReduce");
        return ret;
    }
    else
    {
        query_post_reduce_info = new(query_post_reduce_info) QueryPostReduce();
        query_post_reduce_info->set_post_reduce_info(result_plan,select_stmt);
        exec_plan->set_query_post_reduce_info(query_post_reduce_info);
    }

    vector<uint64_t> &expr_ids = select_stmt->get_where_exprs();

    generate_all_table_shards(result_plan, from_items, all_binding_table_shards);
    if (0 == all_binding_table_shards.size())
    {
        ret = JD_ERR_SHARD_NUM_WRONG;
        jlog(WARNING, "shard manage wrong");
        return ret;
    }
    
    //if there is no where conditions
    if (0 == expr_ids.size())
    {
        /*
        ___________________________
        table1.1 table2.1 table3.1    ----> one_binding_shards's content
        table1.2 table2.2 table3.2
        .........................
        */
        if (all_binding_table_shards.size() > 1)
        {
            select_stmt->set_sql_dispatched_multi_shards(true);
            if (select_stmt->is_has_avg_in_having())
            {
                ret = JD_ERR_AVG_NOT_SUPPORT_IN_SELECT_HAVING;
                jlog(WARNING, "do not support multi shard select query with avg OP in having clause for the result is not precise.");
                return ret;
            }
        }
        
        for (i = 0; i < all_binding_table_shards.size(); i++)
        {
            one_binding_table_shards = all_binding_table_shards.at(i);
            string     assembled_sql;
            ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
            if (exec_plan_unit == NULL)
            {
                ret = JD_ERR_PARSER_MALLOC_FAILED;
                jlog(WARNING, "Can not malloc space for ExecPlanUnit");
                return ret;
            }
            else
            {
                exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
            }

            select_stmt->make_exec_plan_unit_string(result_plan, "", one_binding_table_shards, assembled_sql);

            //generate sql exec plan
            exec_plan_unit->set_exec_unit_sql(assembled_sql);
            //we default think the other binding tables is located with the table's first shard
            exec_plan_unit->set_exec_uint_shard_info(one_binding_table_shards.at(0));
            jlog(INFO, "exec_plan_unit shard name: %s" ,one_binding_table_shards.at(0)->get_shard_name().data());
            jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());

            //add exec_plan_unit
            exec_plan->add_exec_plan_unit(exec_plan_unit);
        }
    }
    else
    {
        //decompose where conditions into seperate sql which is linked by AND
        sql_expr = logical_plan->get_expr_by_id(expr_ids.at(0));

        vector<vector<ObRawExpr*> > atomic_exprs_array;
        where_ret = select_stmt->decompose_where_items(sql_expr->get_expr(), atomic_exprs_array);

        if (WHERE_IS_OR_AND == where_ret)
        {
            multimap<uint32_t, vector<ObRawExpr*> > opted_raw_exprs;
            jlog(INFO, "multi table atomic_exprs_array num: %d" ,atomic_exprs_array.size());
            multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map1;
            multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map_tmp;
            multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map2;
            pair<multimap<uint32_t, vector<ObRawExpr*> >::iterator, multimap<uint32_t, vector<ObRawExpr*> >::iterator> raw_exprs_same_shard;

            reparse_where_with_route_for_multi_tables(result_plan,
                    select_stmt,
                    atomic_exprs_array,
                    opted_raw_exprs,
                    all_binding_table_shards);
            
            set_sql_dispatched_info(select_stmt, opted_raw_exprs);
            
            for (p_map1 = opted_raw_exprs.begin(); p_map1 != opted_raw_exprs.end();)
            {
                vector<vector<ObRawExpr*> > final_exprs_array;
                string assembled_sql;
                uint32_t shard_key_index = 0;
                shard_key_index = p_map1->first;
                one_binding_table_shards = all_binding_table_shards.at(shard_key_index);
                raw_exprs_same_shard = opted_raw_exprs.equal_range(p_map1->first);
                for (p_map2 = raw_exprs_same_shard.first; p_map2 != raw_exprs_same_shard.second; p_map2++)
                {
                    final_exprs_array.push_back((*p_map2).second);
                }
                
                if ((opted_raw_exprs.size() > opted_raw_exprs.count(p_map1->first))&&(select_stmt->is_has_avg_in_having()))
                {
                    ret = JD_ERR_AVG_NOT_SUPPORT_IN_SELECT_HAVING;
                    jlog(WARNING, "do not support multi shard select query with avg OP in having clause for the result is not precise.");
                    return ret;
                }

                for (i = 0; i < opted_raw_exprs.count(p_map1->first); i++)
                {
                    p_map1++;
                }

                ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
                if (exec_plan_unit == NULL)
                {
                    ret = JD_ERR_PARSER_MALLOC_FAILED;
                    jlog(WARNING, "Can not malloc space for ExecPlanUnit");
                    return ret;
                }
                else
                {
                    exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
                }

                string where_conditions;
                append_distributed_where_items(result_plan, where_conditions, final_exprs_array);
                select_stmt->make_exec_plan_unit_string(result_plan, where_conditions, one_binding_table_shards, assembled_sql);
                exec_plan_unit->set_exec_unit_sql(assembled_sql);
                exec_plan_unit->set_exec_uint_shard_info(one_binding_table_shards.at(0));
                jlog(INFO, "exec_plan_unit shard name: %s" ,one_binding_table_shards.at(0)->get_shard_name().data());
                jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());
                
                //add exec_plan_unit
                exec_plan->add_exec_plan_unit(exec_plan_unit);
            }
        }
        else if (WHERE_IS_SUBQUERY == where_ret)
        {

        }
    }

    return ret;
}

/**************************************************
Funtion     :   reparse_where_with_route_for_one_table
Author      :   qinbo
Date        :   2013.10.17
Description :   reparse distributed where conditions items
Input       :   ResultPlan& result_plan,
                schema_table* table_schema,
                vector<vector<ObRawExpr*> > &un_opt_raw_exprs,
                multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs,
                vector<schema_shard*>  &all_table_shards
Output      :   
 **************************************************/
int QueryActuator::reparse_where_with_route_for_one_table(
                    ResultPlan& result_plan,
                    schema_table* table_schema,
                    vector<vector<ObRawExpr*> > &un_opt_raw_exprs,
                    multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs,
                    vector<schema_shard*>  &all_table_shards)
{
    int ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;;

    if (un_opt_raw_exprs.size() == 0)
    {
        return ret;
    }

    for (uint32_t i = 0; i < un_opt_raw_exprs.size(); i++)
    {
        vector<ObRawExpr*> atomic_exprs = un_opt_raw_exprs.at(i);
        vector<ObRawExpr*> partition_sql_exprs;

        search_partition_sql_exprs(result_plan, atomic_exprs, partition_sql_exprs);

        //if there is no route sql, this sql should be sent to all shards
        if (partition_sql_exprs.size() == 0)
        {
            for (uint32_t j = 0; j < all_table_shards.size(); j++)
            {
                opted_raw_exprs.insert(pair<uint32_t, vector<ObRawExpr*> >(j, atomic_exprs));
            }
        }
        //if there is route sql, this sql should be sent to related shards
        else
        {
            ret = build_shard_exprs_array_with_route_one_table(
                    result_plan,
                    table_schema,
                    partition_sql_exprs,
                    atomic_exprs,
                    opted_raw_exprs,
                    all_table_shards);
        }
    }
    return ret;

}


/**************************************************
Funtion     :   reparse_where_with_route_for_multi_tables
Author      :   qinbo
Date        :   2013.10.29
Description :   reparse distributed where conditions items
Input       :   ResultPlan& result_plan,
                ObSelectStmt *select_stmt,
                vector<vector<ObRawExpr*> > &un_opt_raw_exprs,
                multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs,
                vector<vector<schema_shard*> > &all_binding_tables_shards
Output      :   
 **************************************************/
int QueryActuator::reparse_where_with_route_for_multi_tables(
                        ResultPlan& result_plan,
                        ObSelectStmt *select_stmt,
                        vector<vector<ObRawExpr*> > &un_opt_raw_exprs,
                        multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs,
                        vector<vector<schema_shard*> > &all_binding_tables_shards)
{
    int  ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;;
    schema_shard*       shard_info = NULL;
    vector<schema_shard*>    one_binding_table_shards;
    uint32_t i = 0;
    uint32_t j = 0;
    
    if (un_opt_raw_exprs.size() == 0)
    {
        return ret;
    }

    /*
    ___________________________
    table1.1 table2.1 table3.1    ----> one_binding_shards's content
    table1.2 table2.2 table3.2
    .........................
    */

    const vector<FromItem> &from_items = select_stmt->get_all_from_items();
    if (from_items.size() < 2)
    {
        ret = JD_ERR_LOGICAL_TREE_WRONG;
        jlog(WARNING, "From table size is not right");
        return ret;
    }

    //un_opt_raw_exprs' CONTENT is a set of AND exprs(such as a AND b AND c) 
    for (i = 0; i < un_opt_raw_exprs.size(); i++)
    {
        vector<ObRawExpr*> atomic_exprs = un_opt_raw_exprs.at(i);
        vector<ObRawExpr*> partition_sql_exprs;

        search_partition_sql_exprs(result_plan, atomic_exprs, partition_sql_exprs);

        //if there is no route sql, this sql should be sent to all shards
        if (partition_sql_exprs.size() == 0)
        {
            for (j = 0; j < all_binding_tables_shards.size(); j++)
            {
                one_binding_table_shards = all_binding_tables_shards.at(j);
                //all table's shards in [one_binding_table_shards] is located at the same server
                //so we can use the first table's first shard to behalf the same server
                //solution here is not precise, it's just a compromise solution NOW
                shard_info = one_binding_table_shards.at(0);
                opted_raw_exprs.insert(pair<uint32_t, vector<ObRawExpr*> >(j, atomic_exprs));
                #if 0 
                for (k = 0; k < one_binding_table_shards.size(); k++)
                {
                    shard_info = one_binding_table_shards.at(k);
                    opted_raw_exprs.insert(pair<schema_shard*, vector<ObRawExpr*> >(shard_info, atomic_exprs));
                }
                #endif
            }
        }
        //if there is route sql, this sql should be sent to related shards
        else
        {
            ret = build_shard_exprs_array_with_route_multi_table(
                    result_plan,
                    partition_sql_exprs,        //expr related ROUTE info in a set of AND exprs
                    atomic_exprs,               //a set of AND exprs(such as a AND b AND c)
                    opted_raw_exprs,            //exprs related server info
                    all_binding_tables_shards); 
        }
    }
    
    return ret;
}



/**************************************************
Funtion     :   reparse_insert_stmt_rows_value
Author      :   qinbo
Date        :   2014.1.6
Description :   reparse insert rows info
Input       :   ResultPlan& result_plan,
                ObInsertStmt* insert_stmt,
                schema_table* table_schema,
                multimap<uint32_t, uint32_t > &sorted_insert_rows_value,
                vector<schema_shard*>  &all_table_shards
                bool &has_auto_incr_sharding_key
Output      :   
 **************************************************/
int QueryActuator::reparse_insert_stmt_rows_value(
                    ResultPlan& result_plan,
                    ObInsertStmt* insert_stmt,
                    schema_table* table_schema,
                    multimap<uint32_t, uint32_t > &sorted_insert_rows_value, //first: shard index;  second: row index
                    vector<schema_shard*>  &all_table_shards,
                    bool &has_auto_incr_sharding_key)
{
    int  ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;;
    vector<schema_shard*> all_related_shards;
    vector<schema_shard*>   binding_shards_info;
    schema_column*  column_info = NULL;
    ObSqlRawExpr*   sql_expr    = NULL;
    uint32_t        shards_index= 0;
    uint32_t        shard_key_index= 0;
    uint32_t        i = 0;
    bool            has_found_sharding_key = false;
    vector<vector<uint64_t> > all_value_rows = insert_stmt->get_all_value_rows();
    
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    for (i = 0; i < insert_stmt->get_column_size(); i++)
    {            
        column_info = table_schema->get_column_from_table(insert_stmt->get_column_item(i)->column_name_);
        if (column_info->is_sharding_key())
        {
            shard_key_index = i;
            has_found_sharding_key = true;
            #if 0
            if (insert_stmt->get_column_item(i)->column_name_ == table_schema->get_sequence_name())
            {
                ret = JD_ERR_NOT_SUPPORT_INSERT_AUTOINCR_KEY;
                jlog(WARNING, "Now we DO NOT support insert operation with auto_increment key");
                return ret;
            }
            #endif
            break;
        }
    }

    //jlog (INFO,"table_schema->get_sequence_name() :%s", table_schema->get_sequence_name().data() );
    //not found sharding key
    if (!has_found_sharding_key)
    {
        //if (table_schema->get_sequence_name().empty())
        {
            ret = JD_ERR_NOT_SUPPORT_INSERT_NO_KEY;
            jlog(WARNING, "Now we DO NOT support insert operation without sharding-key");
            return ret;
        }
        #if 0
        else
        {
            has_auto_incr_sharding_key = true;
        }
        #endif
    }
    
    for (i = 0; i < all_value_rows.size(); i++)
    {
        vector<uint64_t>& value_row = all_value_rows.at(i);
        sql_expr = logical_plan->get_expr_by_id(value_row.at(shard_key_index));

        map<string, SqlItemType> &sharding_key_tmp = table_schema->get_sharding_key();
        vector<key_data> key_relations;
        vector<schema_shard*> route_shards;
        
        map<string, SqlItemType>::iterator it = sharding_key_tmp.begin();

        if (it == sharding_key_tmp.end())
        {
            ret = JD_ERR_CONFIG_ROUTE_ERR;
            jlog(WARNING, "meta data sharding key is null.");
            return ret;
        }

        if (!has_auto_incr_sharding_key)
        {
            while (it != sharding_key_tmp.end())
            {
                key_data key_relation;
                key_relation.db_name        = result_plan.db_name;
                key_relation.table_name     = table_schema->get_table_name();
                key_relation.sharding_key   = it->first;
                //key_relation.key_type       = it->second;
                key_relation.key_value_num  = 1;
                
                if (it->first == column_info->get_column_name())
                {
                    key_relation.key_relation   = T_OP_EQ;
                    if (!sql_expr->get_expr()->is_const())
                    {
                        ret = JD_ERR_COLUMN_NOT_MATCH;
                        jlog(WARNING, "expr is not const");
                        return ret;
                    }
                    else
                    {
                        ObConstRawExpr *const_expr = dynamic_cast<ObConstRawExpr *> (const_cast<ObRawExpr *> (sql_expr->get_expr()));
                        const_expr->get_ob_const_expr_to_key_data(key_relation, 0);
                    }
                    key_relation.key_type  = sql_expr->get_expr()->get_expr_type();
                    key_relations.push_back(key_relation);
                    break;
                }
                it++;
            }
            
            if (key_relations.size() > 0)
            {
                if (!router::get_instance().get_route_result_with_lock(key_relations, route_shards, result_plan.err_stat_.err_code_)
                    ||(route_shards.size() == 0))
                {
                    if (result_plan.err_stat_.err_code_ != 0)
                    {
                        jlog(WARNING, "route sharding key mismatch...");
                        return result_plan.err_stat_.err_code_;
                    }
                    ret = JD_ERR_CONFIG_ROUTE_ERR;
                    jlog(WARNING, "route info manage error");
                    return ret;
                }
            }
            
            for (uint32_t j = 0; j < route_shards.size(); j++)
            {
                shards_index = search_shard_from_one_table_shards(all_table_shards, route_shards.at(j));  
                if (SHARD_NOT_FOUND != shards_index)
                {
                    sorted_insert_rows_value.insert(pair<uint32_t, uint32_t>(shards_index, i));
                }
                else
                {
                    sorted_insert_rows_value.clear();
                    ret = JD_ERR_SHARD_NUM_WRONG;
                    jlog(WARNING, "shard manage wrong");
                    return ret;
                }
            }
        }
        //have auto_increment column
        else
        {
            if (table_schema->get_sequence_name() != it->first)
            {
                ret = JD_ERR_COLUMN_NOT_MATCH;
                jlog(WARNING, "expr is not const");
                return ret;
            }
            key_data key_relation;
            key_relation.db_name        = result_plan.db_name;
            key_relation.table_name     = table_schema->get_table_name();
            key_relation.sharding_key   = it->first;
            key_relation.key_type       = it->second;
            key_relation.key_value_num  = 1;
            key_relation.key_relation   = T_OP_EQ;
            
            if (!meta_reader::get_instance().get_auto_incr_sequ_value(key_relation.db_name, 
                                            key_relation.table_name, 
                                            key_relation.sharding_key, 
                                            key_relation.value.key_integer[0]))
            {
                ret = JD_ERR_GET_AUTO_INCR_ID_ERR;
                jlog(WARNING, "get insert auto increment id error.");
                return ret;
            }
            
            key_relations.push_back(key_relation);
            if (!router::get_instance().get_route_result_with_lock(key_relations, route_shards, result_plan.err_stat_.err_code_)
                ||(route_shards.size() != 1))
            {
                if (result_plan.err_stat_.err_code_ != 0)
                {
                    jlog(WARNING, "route sharding key mismatch...");
                    return result_plan.err_stat_.err_code_;
                }
                ret = JD_ERR_CONFIG_ROUTE_ERR;
                jlog(WARNING, "route info manage error");
                return ret;
            }
            
            shards_index = search_shard_from_one_table_shards(all_table_shards, route_shards.at(0));  
            if (SHARD_NOT_FOUND != shards_index)
            {
                sorted_insert_rows_value.insert(pair<uint32_t, uint32_t>(shards_index, i));
                insert_stmt->set_auto_incr_id_value(i, key_relation.value.key_integer[0]);
            }
            else
            {
                sorted_insert_rows_value.clear();
                ret = JD_ERR_SHARD_NUM_WRONG;
                jlog(WARNING, "shard manage wrong");
                return ret;
            }
        }
    }
    
    return ret;
}


/**************************************************
Funtion     :   build_shard_exprs_array_with_route_one_table
Author      :   qinbo
Date        :   2013.10.17
Description :   generate distributed where conditions items
Input       :   ResultPlan& result_plan,
                string &sql
                vector<vector<ObRawExpr*> > partition_sql_exprs
                vector<vector<ObRawExpr*> > atomic_exprs,
                multimap<uint32_t , vector<ObRawExpr*> > &opted_raw_exprs
                vector<schema_shard*>  &all_table_shards
Output      :   
 **************************************************/
int QueryActuator::build_shard_exprs_array_with_route_one_table(
        ResultPlan& result_plan,
        schema_table* table_schema,
        vector<ObRawExpr*> partition_sql_exprs,
        vector<ObRawExpr*> atomic_exprs,
        multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs,
        vector<schema_shard*>  &all_table_shards)
{
    int ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;;
    ObRawExpr* raw_expr = NULL;
    vector<vector<schema_shard*> > all_related_shards;
    vector<schema_shard*> shard_tmp1(MAX_SQL_EXEC_PLAN_SHARD_NUM);
    vector<schema_shard*> shard_tmp2;
    uint32_t i = 0;
    uint32_t shards_index = 0;

    for (i = 0; i < partition_sql_exprs.size(); i++)
    {
        raw_expr = partition_sql_exprs.at(i);

        string table_name = table_schema->get_table_name();
        map<string, SqlItemType> &sharding_key_tmp = table_schema->get_sharding_key();
        vector<key_data> key_relations;
        vector<schema_shard*> shard_info;

        map<string, SqlItemType>::iterator it = sharding_key_tmp.begin();
        while (it != sharding_key_tmp.end())
        {
            key_data key_relation;
            key_relation.db_name        = result_plan.db_name;
            key_relation.table_name     = table_name;
            key_relation.sharding_key   = it->first;
            //key_relation.key_type       = raw_expr->get_expr_type();
            if (raw_expr->convert_ob_expr_to_route(result_plan, key_relation))
            {
                key_relations.push_back(key_relation);
            }
            else
            {
                ret = JD_ERR_VALUE_MISMATCH;
                return ret;
            }
            it++;
        }
        if (key_relations.size() > 0)
        {
            if (!router::get_instance().get_route_result_with_lock(key_relations, shard_info, result_plan.err_stat_.err_code_))
            {
                if (result_plan.err_stat_.err_code_ != 0)
                {
                    jlog(WARNING, "route sharding key mismatch...");
                    return result_plan.err_stat_.err_code_;
                }
                ret = JD_ERR_CONFIG_ROUTE_ERR;
                jlog(WARNING, "route info manage error");
                return ret;
            }
        }

        if (shard_info.size() > 0)
        {
            for (uint32_t j=0 ;j < shard_info.size(); j++)
            {
                if (0 == j)
                {
                    jlog(INFO, "FOUND SHARD BEGIN");
                }
                jlog(INFO, "The found shard: %s" , shard_info.at(j)->get_shard_name().data());
                if (j == shard_info.size()-1)
                {
                    jlog(INFO, "FOUND SHARD END");
                }
            }
            all_related_shards.push_back(shard_info);
        }
    }

    if (all_related_shards.size() == 0)
    {
        ret = JD_ERR_SHARD_NUM_WRONG;
        jlog(WARNING, "shard manage wrong");
        return ret;
    }

    if (all_related_shards.size() == 1)
    {
        shard_tmp1 = all_related_shards.at(0);
        for (i = 0; i < shard_tmp1.size(); i++)
        {
            shards_index = search_shard_from_one_table_shards(all_table_shards, shard_tmp1.at(i));  
            if (SHARD_NOT_FOUND != shards_index)
            {
                opted_raw_exprs.insert(pair<uint32_t, vector<ObRawExpr*> >(shards_index, atomic_exprs));
            }
            else
            {
                ret = JD_ERR_SHARD_NUM_WRONG;
                jlog(WARNING, "shard manage wrong");
                return ret;
            }
        }
        return ret;
    }
    //get shards' intersection
    else if (all_related_shards.size() == 2)
    {
        set_intersection(all_related_shards.at(0).begin(), all_related_shards.at(0).end(),
                all_related_shards.at(1).begin(), all_related_shards.at(1).end(), shard_tmp1.begin());
    }
    else
    {
        set_intersection(all_related_shards.at(0).begin(), all_related_shards.at(0).end(),
                all_related_shards.at(1).begin(), all_related_shards.at(1).end(), shard_tmp1.begin());
        for (i = 2; i < all_related_shards.size(); i++)
        {
            vector<schema_shard*> shard_tmp3(MAX_SQL_EXEC_PLAN_SHARD_NUM);
            shard_tmp2 = all_related_shards.at(i);
            set_intersection(shard_tmp1.begin(), shard_tmp1.end(), shard_tmp2.begin(), shard_tmp2.end(), shard_tmp3.begin());
            shard_tmp1.clear();
            shard_tmp1.resize(MAX_SQL_EXEC_PLAN_SHARD_NUM, NULL);
            shard_tmp1 = shard_tmp3;
        }
    }

    for (i = 0; i < shard_tmp1.size(); i++)
    {
        if (NULL != shard_tmp1.at(i))
        {
            shards_index = search_shard_from_one_table_shards(all_table_shards, shard_tmp1.at(i));  
            if (SHARD_NOT_FOUND != shards_index)
            {
                opted_raw_exprs.insert(pair<uint32_t, vector<ObRawExpr*> >(shards_index, atomic_exprs));
                jlog(INFO, "The intersection shard: %s" , all_table_shards.at(shards_index)->get_shard_name().data());
            }
            else
            {
                ret = JD_ERR_SHARD_NUM_WRONG;
                jlog(WARNING, "shard manage wrong");
                return ret;
            }
        }
    }

    return ret;
}


/**************************************************
Funtion     :   build_shard_exprs_array_with_route_multi_table
Author      :   qinbo
Date        :   2013.10.17
Description :   generate distributed where conditions items
Input       :   ResultPlan& result_plan,
                ObSelectStmt *select_stmt,
                vector<ObRawExpr*> partition_sql_exprs,
                vector<ObRawExpr*> atomic_exprs,
                multimap<schema_shard*, vector<ObRawExpr*> > &opted_raw_exprs
                vector<vector<schema_shard*> > &all_binding_tables_shards
Output      :   
 **************************************************/
int QueryActuator::build_shard_exprs_array_with_route_multi_table(
        ResultPlan& result_plan,
        vector<ObRawExpr*> partition_sql_exprs,
        vector<ObRawExpr*> atomic_exprs,
        multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs,
        vector<vector<schema_shard*> > &all_binding_tables_shards)
{
    int ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;;
    ObRawExpr* raw_expr = NULL;
    vector<schema_shard*> one_binding_tables_shards;
    schema_table*   table_schema = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint32_t shards_index = 0;
    /*
    ___________________________
    table1.1 table2.1 table3.1    ----> one_binding_shards's content
    table1.2 table2.2 table3.2
    table1.3 table2.3 table3.3
    */

    for (i = 0; i < partition_sql_exprs.size(); i++)
    {
        raw_expr = partition_sql_exprs.at(i);

        string table_name;
        
        if (!raw_expr->try_get_table_name(result_plan,table_name))
        {
            jlog(INFO,"Can not get table name from raw_expr.");
            continue;
        }
        
        table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, table_name);

        for (j = 0; j<all_binding_tables_shards.size();j++)
        {
            one_binding_tables_shards = all_binding_tables_shards.at(j);
            
        }
        
        map<string, SqlItemType> &sharding_key_tmp = table_schema->get_sharding_key();
        vector<key_data> key_relations;
        vector<schema_shard*> route_shards;

        map<string, SqlItemType>::iterator it = sharding_key_tmp.begin();

        while (it != sharding_key_tmp.end())
        {
            key_data key_relation;
            key_relation.db_name        = result_plan.db_name;
            key_relation.table_name     = table_name;
            key_relation.sharding_key   = it->first;
            //key_relation.key_type       = raw_expr->get_expr_type();
            if (raw_expr->convert_ob_expr_to_route(result_plan, key_relation))
            {
                key_relations.push_back(key_relation);
            }
            else
            {
                ret = JD_ERR_VALUE_MISMATCH;
                return ret;
            }
            it++;
        }

        if (key_relations.size() > 0)
        {
            if ((!router::get_instance().get_route_result_with_lock(key_relations, route_shards, result_plan.err_stat_.err_code_))
                    || (route_shards.size() == 0))
            {
                if (result_plan.err_stat_.err_code_ != 0)
                {
                    jlog(WARNING, "route sharding key mismatch...");
                    return result_plan.err_stat_.err_code_;
                }
                ret = JD_ERR_CONFIG_ROUTE_ERR;
                jlog(WARNING, "route info manage error");
                return ret;
            }
        }

        for (k = 0; k < route_shards.size(); k++)
        {
            shards_index = search_shard_from_multi_tables_shards(all_binding_tables_shards, route_shards.at(k));  
            if (SHARD_NOT_FOUND != shards_index)
            {
                opted_raw_exprs.insert(pair<uint32_t, vector<ObRawExpr*> >(shards_index, atomic_exprs));
            }
            else
            {
                opted_raw_exprs.clear();
                ret = JD_ERR_SHARD_NUM_WRONG;
                jlog(WARNING, "shard manage wrong");
                return ret;
            }
        }
    }

    return ret;
}

/**************************************************
Funtion     :   search_shard_from_one_table_shards
Author      :   qinbo
Date        :   2013.12.24
Description :   search one shard from one table's shards and return 
                index
Input       :   vector<schema_shard*> &table_all_shards
                schema_shard* goal_shard
Output      :   index
 **************************************************/
int QueryActuator::search_shard_from_one_table_shards( vector<schema_shard*> &table_all_shards,
                                                        schema_shard* goal_shard)
{
    if (table_all_shards.size() == 0)
    {
        return SHARD_NOT_FOUND;
    }

    for (uint32_t i=0; i< table_all_shards.size();i++)
    {
        if (table_all_shards.at(i) == goal_shard)
        {
            return i;
        }
    }
    return SHARD_NOT_FOUND;

}


/**************************************************
Funtion     :   generate_all_table_shards
Author      :   qinbo
Date        :   2013.10.24
Description :   generate all tables' shards
Input       :   ResultPlan& result_plan,
                vector<string> &table_names,
                vector<vector<schema_shard*> > &all_binding_tables_shards
Output      :   
 **************************************************/
void QueryActuator::generate_all_table_shards(ResultPlan& result_plan,
                                              const vector<FromItem> &from_items,
                                              vector<vector<schema_shard*> > &all_binding_tables_shards)
{
    string   db_name;
    db_name.assign(result_plan.db_name);
    schema_table* table_schema = NULL;
        
    if(!is_from_tables_binding(result_plan,from_items))
    {
        return;
    }

    /*
    ___________________________
    table1.1 table2.1 table3.1    ----> one_binding_shards's content
    table1.2 table2.2 table3.2
    .........................
    */
    table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, from_items.at(0).table_name_);
    if (NULL == table_schema)
    {
        jlog(WARNING, "Table %s should not be empty in table schema", from_items.at(0).table_name_.data());
        return;
    }

    uint32_t binding_shards_num = table_schema->get_all_shards().size();

    for (uint32_t i = 0; i < binding_shards_num; i++)
    {
        vector<schema_shard*>  one_binding_shards;
        for (uint32_t j = 0; j < from_items.size(); j++)
        {
            table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, from_items.at(j).table_name_);
            one_binding_shards.push_back(table_schema->get_all_shards().at(i));
        }
        all_binding_tables_shards.push_back(one_binding_shards);
    }
    return;
}


/**************************************************
Funtion     :   search_shard_from_multi_tables_shards
Author      :   qinbo
Date        :   2013.12.24
Description :   search one shard from all_binding_tables_shards and return 
                all_binding_tables_shards index
Input       :   vector<vector<schema_shard*> > &all_binding_tables_shards,
                schema_shard* goal_shard
Output      :   index
 **************************************************/
int QueryActuator::search_shard_from_multi_tables_shards( vector<vector<schema_shard*> > &all_binding_tables_shards,
                                                   schema_shard* goal_shard)
{
    if (all_binding_tables_shards.size() == 0)
    {
        return SHARD_NOT_FOUND;
    }

    for (uint32_t i=0; i< all_binding_tables_shards.size();i++)
    {
        for (uint32_t j=0; j< all_binding_tables_shards.at(i).size();j++)
        {
            if (all_binding_tables_shards.at(i).at(j) == goal_shard)
            {
                return i;
            }
        }
    }
    return SHARD_NOT_FOUND;

}


/**************************************************
Funtion     :   search_partition_sql_exprs
Author      :   qinbo
Date        :   2013.10.18
Description :   find all expr that need to be partitioned
Input       :   ResultPlan& result_plan
                vector<ObRawExpr*>  atomic_exprs,
                vector<ObRawExpr*>  &partition_sql_exprs
Output      :   
 **************************************************/
int QueryActuator::search_partition_sql_exprs(
        ResultPlan& result_plan,
        vector<ObRawExpr*> &atomic_exprs,
        vector<ObRawExpr*> &partition_sql_exprs)
{
    uint32_t i = 0;
    ObRawExpr* raw_expr = NULL;

    for (i = 0; i < atomic_exprs.size(); i++)
    {
        raw_expr = atomic_exprs.at(i);

        if (raw_expr->is_need_to_get_route(result_plan))
        {
            partition_sql_exprs.push_back(raw_expr);
        }
    }

    return 0;
}

/**************************************************
Funtion     :   append_distributed_where_items
Author      :   qinbo
Date        :   2013.10.17
Description :   generate distributed where conditions items
Input       :   ResultPlan& result_plan,
                string &sql
                vector<vector<ObRawExpr*> > &atomic_exprs_array
Output      :   
 **************************************************/
void QueryActuator::append_distributed_where_items(ResultPlan& result_plan,
                                                string &sql,
                                                vector<vector<ObRawExpr*> > &atomic_exprs_array)
{
    if (atomic_exprs_array.size() == 0)
    {
        return;
    }

    sql.append(" WHERE ");
    
    for (uint32_t i = 0; i < atomic_exprs_array.size(); i++)
    {
        if (i > 0)
        {
            sql.append(" OR ");
        }
        vector<ObRawExpr*> atomic_exprs = atomic_exprs_array.at(i);
        for (uint32_t j = 0; j < atomic_exprs.size(); j++)
        {
            string assembled_sql_tmp;
            
            if (j > 0)
            {
                sql.append(" AND ");
            }
            atomic_exprs.at(j)->to_string(result_plan, assembled_sql_tmp);
            sql.append(assembled_sql_tmp);
        }
    }

    return;
}


/**************************************************
Funtion     :   distribute_sql_to_all_shards
Author      :   qinbo
Date        :   2013.11.6
Description :   vector elem is already existing
Input       :   ResultPlan& result_plan,
                schema_table* table_schema,
                SameLevelExecPlan* exec_plan
Output      :   
return      :
 **************************************************/
int QueryActuator::distribute_sql_to_all_shards( 
                    ResultPlan& result_plan,
                    const uint64_t& query_id,
                    schema_table* table_schema,
                    SameLevelExecPlan* exec_plan)
{
    uint32_t i   = 0;
    int ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    schema_shard*   shard_info  = NULL;
    ObBasicStmt*    stmt        = NULL;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    if (0 == table_schema->get_all_shards().size())
    {
        ret = JD_ERR_SHARD_NUM_WRONG;
        jlog(WARNING, "shard manage wrong");
        return ret;
    }

    stmt = logical_plan->get_query(query_id);
    
    for (i = 0; i < table_schema->get_all_shards().size(); i++)
    {
        string     assembled_sql = "";
        shard_info = table_schema->get_all_shards().at(i);
        vector<schema_shard*> shard_tmp;
        shard_tmp.push_back(shard_info);
        
        ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
        if (exec_plan_unit == NULL)
        {
            ret = JD_ERR_PARSER_MALLOC_FAILED;
            jlog(WARNING, "Can not malloc space for ExecPlanUnit");
            return ret;
        }
        else
        {
            exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
        }
    
        stmt->make_exec_plan_unit_string(result_plan, "", shard_tmp, assembled_sql);
        //generate sql exec plan
        exec_plan_unit->set_exec_unit_sql(assembled_sql);
        exec_plan_unit->set_exec_uint_shard_info(shard_info);
        jlog(INFO, "exec_plan_unit shard name: %s" ,shard_info->get_shard_name().c_str());
        jlog(INFO, "exec_plan_unit SQL name  : %s" ,assembled_sql.data());
        //add exec_plan_unit
        exec_plan->add_exec_plan_unit(exec_plan_unit);
    }

    return ret;
}


/**************************************************
Funtion     :   send_sql_to_config_server
Author      :   qinbo
Date        :   2013.12.11
Description :   send sql(show/set command) to config server
                show/set command wo do not truely support now
Input       :   ResultPlan& result_plan,
                FinalExecPlan* physical_plan,
                string sql
Output      :   
return      :   
 **************************************************/
int QueryActuator::send_sql_to_config_server( 
                            ResultPlan& result_plan,
                            FinalExecPlan* physical_plan,
                            string sql)
{
    int ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    
    SameLevelExecPlan* exec_plan = (SameLevelExecPlan*) parse_malloc(sizeof (SameLevelExecPlan), NULL);
    if (exec_plan == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for SameLevelExecPlan");
        return ret;
    }
    else
    {
        exec_plan = new(exec_plan) SameLevelExecPlan();
        physical_plan->add_same_level_exec_plan(exec_plan);
    }

    exec_plan->set_sql_sent_goal(SEND_SQL_TO_CONFIG_SERVER);
    
    ExecPlanUnit* exec_plan_unit = (ExecPlanUnit*) parse_malloc(sizeof (ExecPlanUnit), NULL);
    if (exec_plan_unit == NULL)
    {
        ret = JD_ERR_PARSER_MALLOC_FAILED;
        jlog(WARNING, "Can not malloc space for ExecPlanUnit");
        return ret;
    }
    else
    {
        exec_plan_unit = new(exec_plan_unit) ExecPlanUnit();
        exec_plan->add_exec_plan_unit(exec_plan_unit);
    }
    
    exec_plan_unit->set_exec_unit_sql(sql);
    return ret;
}

/**************************************************
Funtion     :   is_from_tables_binding
Author      :   qinbo
Date        :   2013.12.24
Description :   whether all from tables are binding(shard with the same COLUMN KEY)
Input       :   ResultPlan& result_plan,
                vector<FromItem> &from_items
Output      :   
return      :   bool
 **************************************************/
bool QueryActuator::is_from_tables_binding(ResultPlan& result_plan,
                                          const vector<FromItem> &from_items)
{
    schema_table*   table_schema          = NULL;
    schema_table*   first_table_schema    = NULL;
    first_table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, from_items.at(0).table_name_);
    
    if (NULL == first_table_schema)
    {
        jlog(ERROR, "shard schema manage error!!");
        return false;
    }
    
    if (from_items.size() < 2) 
    {
        jlog(ERROR, "from items manage error!!");
        return false;
    }

    vector<string> &binding_tables = first_table_schema->get_relation_table();
    if (binding_tables.size() == 0)
    {
        return false;
    }
    
    for (uint32_t i = 1; i < from_items.size(); i++)
    {
        table_schema = meta_reader::get_instance().get_table_schema_with_lock(result_plan.db_name, from_items.at(i).table_name_);
        vector<string>::iterator pos;
        pos = find(binding_tables.begin(),binding_tables.end(),from_items.at(i).table_name_);
        if(pos == binding_tables.end())
        {
            return false;
        }
    }
    return true;
    
}

/**************************************************
Funtion     :   set_sql_dispatched_info
Author      :   qinbo
Date        :   2013.12.24
Description :   set "select_stmt" "is_sql_dispatched_multi_shards"
Input       :   ObSelectStmt *select_stmt, 
                multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs
Output      :   
return      :   
 **************************************************/
void QueryActuator::set_sql_dispatched_info(ObSelectStmt *select_stmt, 
                                            multimap<uint32_t, vector<ObRawExpr*> > &opted_raw_exprs)
{
    multimap<uint32_t, vector<ObRawExpr*> >::iterator p_map;
    uint32_t counter = 0;
        
    for (p_map = opted_raw_exprs.begin(); p_map != opted_raw_exprs.end();)
    {
        for (uint32_t i = 0; i < opted_raw_exprs.count(p_map->first); i++)
        {
            p_map++;
        }
        counter++;
    }

    if (counter > 1)
    {
        select_stmt->set_sql_dispatched_multi_shards(true);
    }
    else
    {
        select_stmt->set_sql_dispatched_multi_shards(false);
    }
}


/**************************************************
Funtion     :   check_acl
Author      :   tangchao
Date        :   2014.1.23
Description :   check acl 
Input       :   string db, string host, string user, ObBasicStmt::StmtType  queryType,
                vector<string> &acl_checked_tables
Output      :   acl checked ok or false
return      :   
 **************************************************/
int QueryActuator::check_acl(string db, string host, string user, ObBasicStmt::StmtType  queryType,
                                    vector<string> &acl_checked_tables)
{
    int error = OB_SUCCESS;
    uint32_t next = 0;

    jlog(INFO, "enter dispatch_command::check_acl");
    
#if 0
    if (5 == result_plan.meta_db_name.size() &&  0 == result_plan.meta_db_name.compare("mysql"))
    {
        if (!acl_check(user, host, SUPER_PRIV))
        {
            error = ERROR_ACCESS_DENY_NO_SUPER_PRIVILEGE;
        }
        return error;
    }
#endif
    
    /* convenient */
    switch (queryType)
    {
            /* Read from database */
        case ObBasicStmt::T_SHOW_SCHEMA:
        case ObBasicStmt::T_SHOW_DATABASES:
        case ObBasicStmt::T_SHOW_TABLES:
        case ObBasicStmt::T_SHOW_COLUMNS:
        case ObBasicStmt::T_SHOW_VARIABLES:
        case ObBasicStmt::T_SHOW_CREATE_TABLE:
        case ObBasicStmt::T_SHOW_TABLE_STATUS:
        {
            break;
        }
        case ObBasicStmt::T_SELECT:
        {
            for (next = 0; next < acl_checked_tables.size(); next++)
            {
                if (!acl_check(user, host, db, acl_checked_tables[next], SELECT_PRIV))
                {
                    error = ERROR_ACCESS_DENY_SELECT_TABLE;
                    break;
                }
            }
        }
            break;
            /* Write from database */
        case ObBasicStmt::T_DELETE:
        {
            for (next = 0; next < acl_checked_tables.size(); next++)
            {
                if (!acl_check(user, host, db, acl_checked_tables[next], DELETE_PRIV))
                {
                    error = ERROR_ACCESS_DENY_SELECT_TABLE;
                    break;
                }
            }
        }
            break;
        case ObBasicStmt::T_VARIABLE_SET:
        case ObBasicStmt::T_INSERT:
        case ObBasicStmt::T_REPLACE:
        {
            for (next = 0; next < acl_checked_tables.size(); next++)
            {
                if (!acl_check(user, host, db, acl_checked_tables[next], INSERT_PRIV))
                {
                    error = ERROR_ACCESS_DENY_INSERT_TABLE;
                    break;
                }
            }
        }
            break;
        case ObBasicStmt::T_UPDATE:
        {
            for (next = 0; next < acl_checked_tables.size(); next++)
            {
                if (!acl_check(user, host, db, acl_checked_tables[next], UPDATE_PRIV))
                {
                    error = ERROR_ACCESS_DENY_UPDATE_TABLE;
                    break;
                }
            }
        }
            break;
        case ObBasicStmt::T_SHOW_SERVER_STATUS:
        case ObBasicStmt::T_SHOW_WARNINGS:
        case ObBasicStmt::T_SHOW_GRANTS:
        case ObBasicStmt::T_SHOW_PARAMETERS:
        case ObBasicStmt::T_SHOW_PROCESSLIST:
        {
            if (!acl_check(user, host, SUPER_PRIV))
            {
                error = ERROR_ACCESS_DENY_NO_SUPER_PRIVILEGE;
            }
            break;
        }
        default:
            error = JD_ERR_INVALID_ACL_CHECK_CMD;
            break;
    }

    return error;
}



