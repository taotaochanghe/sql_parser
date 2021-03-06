#include <string>
#include "ob_stmt.h"
#include "sql_select_stmt.h"
#include "sql_logical_plan.h"
#include "parse_malloc.h"
#include "utility.h"

using namespace jdbd::sql;
using namespace jdbd::common;
using namespace std;

ObStmt::ObStmt(StmtType type)
: ObBasicStmt(type)
{
    // means no where conditions
    //where_expr_id_ = OB_INVALID_ID;
    // placeholder of index 0
    //tables_hash_.add_column_desc(OB_INVALID_ID, OB_INVALID_ID);
}

ObStmt::~ObStmt()
{
}

int ObStmt::add_table_item(
        ResultPlan& result_plan,
        const string& table_name,
        const string& alias_name,
        uint64_t& table_id,
        const TableItem::TableType type,
        const uint64_t ref_id)
{
    int& ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    table_id = OB_INVALID_ID;
    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);

    TableItem item;
    if (ret == OB_SUCCESS)
    {
        switch (type)
        {
        case TableItem::ALIAS_TABLE:
            if (table_name == alias_name)
            {
                ret = JD_ERR_ILLEGAL_NAME;
                jlog(WARNING, "table '%.*s' must not alias the same name", (int32_t)table_name.size(), table_name.data());
                break;
            }
            /* go through */
        case TableItem::BASE_TABLE:
        {
            string db_name_tmp;
            db_name_tmp.assign(result_plan.db_name);
            schema_table *schema_table = meta_reader::get_instance().get_table_schema_with_lock(db_name_tmp, table_name);

            if (NULL == schema_table)
            {
                ret = JD_ERR_TABLE_UNKNOWN;
                jlog(WARNING, "table '%.*s' does not exist", (int32_t)table_name.size(), table_name.data());
                break;
            }
            item.ref_id_ = schema_table->get_table_id();

            if (item.ref_id_ == OB_INVALID_ID)
            {
                ret = JD_ERR_TABLE_UNKNOWN;
                jlog(WARNING, "table '%.*s' does not exist", (int32_t)table_name.size(), table_name.data());
                break;
            }

            //if (type == TableItem::BASE_TABLE)
            item.table_id_ = item.ref_id_;
            // else
            //   item.table_id_ = logical_plan->generate_table_id();
            break;
        }
        case TableItem::GENERATED_TABLE:
            if (ref_id == OB_INVALID_ID)
            {
                ret = JD_ERR_ILLEGAL_ID;
                jlog(WARNING, "illegal ref_id %lu", ref_id);
                break;
            }
            item.ref_id_ = ref_id;
            item.table_id_ = logical_plan->generate_table_id();
            break;
        default:
            /* won't be here */
            ret = JD_ERR_PARSER_SYNTAX;
            jlog(WARNING, "Unknown table type when add_table_item");
            break;
        }
    }

    /* to check if the table is duplicated */
    if (ret == OB_SUCCESS)
    {
        TableItem old_item;
        uint32_t size = table_items_.size();
        for (uint32_t i = 0; ret == OB_SUCCESS && i < size; ++i)
        {
            old_item = table_items_[i];
            if (alias_name.size() == 0)
            {
                if (table_name == old_item.table_name_
                        || table_name == old_item.alias_name_)
                {
                    ret = JD_ERR_TABLE_DUPLICATE;
                    jlog(WARNING, "table %.*s is ambiguous", (int32_t)table_name.size(), table_name.data());
                    break;
                }
            }
            else if (old_item.alias_name_.size() == 0)
            {
                if (table_name == old_item.table_name_
                        || alias_name == old_item.table_name_)
                {
                    ret = JD_ERR_TABLE_DUPLICATE;
                    jlog(WARNING, "table %.*s is ambiguous", (int32_t)old_item.table_name_.size(), old_item.table_name_.data());
                    break;
                }
            }
            else
            {
                if (table_name == old_item.alias_name_)
                {
                    ret = JD_ERR_TABLE_DUPLICATE;
                    jlog(WARNING, "table %.*s is ambiguous", (int32_t)table_name.size(), table_name.data());
                    break;
                }
                if (alias_name == old_item.table_name_
                        || alias_name == old_item.alias_name_)
                {
                    ret = JD_ERR_TABLE_DUPLICATE;
                    jlog(WARNING, "table %.*s is ambiguous", (int32_t)alias_name.size(), alias_name.data());
                    break;
                }
            }
        }
    }

    if (ret == OB_SUCCESS)
    {
        item.table_name_ = table_name;
        item.alias_name_ = alias_name;
        item.type_ = type;
        item.has_scan_columns_ = false;
        item.need_display_table_name = false;
        table_items_.push_back(item);
    }

#if 0
    // for tables bitset usage
    if (ret == OB_SUCCESS)
    {
        if ((ret = tables_hash_.add_column_desc(item.table_id_, OB_INVALID_ID)) != OB_SUCCESS)
            jlog(WARNING, 
                "Can not add table_id to hash table");
    }
#endif  

    if (ret == OB_SUCCESS)
        table_id = item.table_id_;
    return ret;
}

uint64_t ObStmt::get_table_item(const string& table_name, TableItem** table_item)
{
    // table name mustn't be empty
    uint32_t num = table_items_.size();
    for (uint32_t i = 0; i < num; i++)
    {
        TableItem& item = table_items_[i];
        if (table_name == item.table_name_ || table_name == item.alias_name_)
        {
            if (table_item)
                *table_item = &item;
            return item.table_id_;
        }
    }

    return OB_INVALID_ID;
}

TableItem* ObStmt::get_table_item_by_id(uint64_t table_id)
{
    TableItem *table_item = NULL;
    uint32_t num = table_items_.size();
    for (uint32_t i = 0; i < num; i++)
    {
        if (table_items_[i].table_id_ == table_id)
        {
            table_item = &table_items_[i];
            break;
        }
    }
    return table_item;
}

ColumnItem* ObStmt::get_column_item(
        const string* table_name,
        const string& column_name)
{
    uint64_t table_id = OB_INVALID_ID;

    if (table_name && table_name->size() != 0)
    {
        table_id = get_table_item(*table_name);
        if (table_id == OB_INVALID_ID)
            return NULL;
    }

    uint32_t num = column_items_.size();
    for (uint32_t i = 0; i < num; i++)
    {
        ColumnItem& column_item = column_items_[i];
        if (column_name == column_item.column_name_ &&
                (column_item.is_name_unique_ || table_id == column_item.table_id_))
        {
            return &column_item;
        }
    }

    return NULL;
}

ColumnItem* ObStmt::get_column_item_by_id(uint64_t table_id, uint64_t column_id)
{
    ColumnItem *column_item = NULL;
    uint32_t num = column_items_.size();
    for (uint32_t i = 0; i < num; i++)
    {
        if (table_id == column_items_[i].table_id_ && column_id == column_items_[i].column_id_)
        {
            column_item = &column_items_[i];
            break;
        }
    }
    return column_item;
}

int ObStmt::add_column_item(const ColumnItem& column_item)
{
    int ret = OB_SUCCESS;
    // ugly implement
    TableItem* table_item = get_table_item_by_id(column_item.table_id_);
    if (table_item == NULL)
        ret = OB_ERROR;
    else
    {
        table_item->has_scan_columns_ = true;
        column_items_.push_back(column_item);
    }
    return ret;
}

int ObStmt::add_column_item(
        ResultPlan& result_plan,
        const string& column_name,
        const string* table_name,
        ColumnItem** col_item)
{
    int& ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    ColumnItem column_item;
    TableItem* table_item = NULL;
    ColumnItem* ret_item = NULL;
    column_item.table_id_ = OB_INVALID_ID;
    column_item.column_id_ = OB_INVALID_ID;

    if (table_name)
    {
        column_item.table_id_ = get_table_item(*table_name, &table_item);
        if (column_item.table_id_ == OB_INVALID_ID)
        {
            ret = JD_ERR_TABLE_UNKNOWN;
            jlog(WARNING, "Unknown table name %.*s", (int32_t)table_name->size(), table_name->data());
            return ret;
        }
        column_item.is_name_unique_ = false;
    }
    else
    {
        column_item.is_name_unique_ = true;
    }

    if (column_item.table_id_ != OB_INVALID_ID)
    {
        ret = check_table_column(
                result_plan,
                column_name,
                *table_item,
                column_item.column_id_,
                column_item.data_type_);
        if (ret != OB_SUCCESS)
        {
            return ret;
        }
        else if (column_item.column_id_ == OB_INVALID_ID)
        {
            ret = JD_ERR_COLUMN_UNKNOWN;
            jlog(WARNING, "Unknown column name %.*s", (int32_t)column_name.size(), column_name.data());
            return ret;
        }
        table_item->has_scan_columns_ = true;
    }
    else
    {
        uint32_t num = table_items_.size();
        uint64_t column_id = OB_INVALID_ID;
        ObObjType column_type = ObMinType;
        for (uint32_t i = 0; i < num; i++)
        {
            TableItem& table_item = get_table_item(i);
            ret = check_table_column(result_plan, column_name, table_item, column_id, column_type);
            if (ret == OB_SUCCESS)
            {
                if (column_item.table_id_ != OB_INVALID_ID)
                {
                    ret = JD_ERR_COLUMN_DUPLICATE;
                    jlog(WARNING, "Column name %.*s is ambiguous", (int32_t)column_name.size(), column_name.data());
                    return ret;
                }
                column_item.table_id_ = table_item.table_id_;
                column_item.column_id_ = column_id;
                column_item.data_type_ = column_type;
                table_item.has_scan_columns_ = true;
            }
            else if (ret != JD_ERR_COLUMN_UNKNOWN)
            {
                return ret;
            }
        }
        if (column_item.column_id_ == OB_INVALID_ID)
        {
            ret = JD_ERR_COLUMN_UNKNOWN;
            jlog(WARNING, "Unknown column name %.*s", (int32_t)column_name.size(), column_name.data());
            return ret;
        }
    }

    column_item.column_name_ = column_name;
    // not be used now
    column_item.is_group_based_ = false;
    column_item.query_id_ = 0;

    if (table_name)
    {
        column_items_.push_back(column_item);
#if 0
        if (ret != OB_SUCCESS)
        {
            jlog(WARNING, "Can not add column %.*s", column_name.size(), column_name.data());
            return ret;
        }
#endif
        ret_item = &column_items_.back();
    }
    else
    {
        bool bExist = false;
        uint32_t num = column_items_.size();
        for (uint32_t i = 0; i < num; i++)
        {
            ColumnItem& item = column_items_[i];
            if (column_name == item.column_name_)
            {
                item.is_name_unique_ = true;
                ret_item = &item;
                bExist = true;
                break;
            }
        }
        if (!bExist)
        {
            column_items_.push_back(column_item);
#if 0
            if (ret != OB_SUCCESS)
            {
                jlog(WARNING, "Can not add column %.*s", column_name.size(), column_name.data());
                return ret;
            }
#endif
            ret_item = &column_items_.back();
        }
    }
    if (col_item != NULL)
        *col_item = ret_item;

    return ret;
}

int ObStmt::check_table_column(
        ResultPlan& result_plan,
        const string& column_name,
        const TableItem& table_item,
        uint64_t& column_id,
        ObObjType& column_type)
{
    int& ret = result_plan.err_stat_.err_code_ = OB_SUCCESS;
    column_id = OB_INVALID_ID;

    ObLogicalPlan* logical_plan = static_cast<ObLogicalPlan*> (result_plan.plan_tree_);
    OB_ASSERT(NULL != logical_plan);
    
    switch (table_item.type_)
    {
        case TableItem::BASE_TABLE:
            // get through
        case TableItem::ALIAS_TABLE:
        {
            string db_name_tmp;
            db_name_tmp.assign(result_plan.db_name);
            schema_column* schema_column = meta_reader::get_instance().get_column_schema_with_lock(db_name_tmp, table_item.table_name_, column_name);
            if (NULL != schema_column)
            {
                column_id = schema_column->get_column_id();
                column_type = trans_int_type2obj_type(schema_column->get_column_type());
                if ((column_type <= ObMinType)||(column_type >= ObMaxType))
                {
                    ret = JD_ERR_PARSER_SYNTAX;
                    jlog(WARNING, "Unknown table type when check_table_column1");
                    break;
                }
            }
            break;
        }
        case TableItem::GENERATED_TABLE:
        {
            ObBasicStmt* stmt = logical_plan->get_query(table_item.ref_id_);
            if (stmt == NULL)
            {
                ret = JD_ERR_ILLEGAL_ID;
                jlog(WARNING, "Wrong query id %lu", table_item.ref_id_);
            }
            ObSelectStmt* select_stmt = static_cast<ObSelectStmt*> (stmt);
            int32_t num = select_stmt->get_select_item_size();
            for (int32_t i = 0; i < num; i++)
            {
                const SelectItem& select_item = select_stmt->get_select_item(i);
                if (column_name == select_item.alias_name_)
                {
                    if (column_id == OB_INVALID_ID)
                    {
                        column_id = i + OB_APP_MIN_COLUMN_ID;
                        column_type = select_item.type_;
                    }
                    else
                    {
                        ret = JD_ERR_COLUMN_DUPLICATE;
                        jlog(WARNING, "column %.*s is ambiguous", (int32_t)column_name.size(), column_name.data());
                        break;
                    }
                }
            }
            break;
        }
        default:
            // won't be here
            ret = JD_ERR_PARSER_SYNTAX;
            jlog(WARNING, "Unknown table type when check_table_column1");
            break;
    }
    
    if (ret == OB_SUCCESS && column_id == OB_INVALID_ID)
    {
        ret = JD_ERR_COLUMN_UNKNOWN;
        jlog(WARNING, "Unknown table type when check_table_column2");
    }
    return ret;
}

#if 0

int32_t ObStmt::get_table_bit_index(uint64_t table_id) const
{
    int64_t idx = tables_hash_.get_idx(table_id, OB_INVALID_ID);
    return static_cast<int32_t> (idx);
}
#endif

void ObStmt::print(FILE* fp, int32_t level, int32_t index)
{
    OB_ASSERT(index >= 0);
    UNUSED(index);
    uint32_t i;
    if (table_items_.size() > 0)
    {
        print_indentation(fp, level);
        fprintf(fp, "<TableItemList Begin>\n");
        for (i = 0; i < table_items_.size(); ++i)
        {
            TableItem& item = table_items_[i];
            print_indentation(fp, level + 1);
            fprintf(fp, "{Num %d, TableId:%lu, TableName:%.*s, ",
                    i, item.table_id_, (int32_t)item.table_name_.size(), item.table_name_.data());
            if (item.alias_name_.size() > 0)
                fprintf(fp, "AliasName:%.*s, ", (int32_t)item.alias_name_.size(), item.alias_name_.data());
            else
                fprintf(fp, "AliasName:NULL, ");
            if (item.type_ == TableItem::BASE_TABLE)
                fprintf(fp, "Type:BASE_TABLE, ");
            else if (item.type_ == TableItem::ALIAS_TABLE)
                fprintf(fp, "Type:ALIAS_TABLE, ");
            else if (item.type_ == TableItem::GENERATED_TABLE)
                fprintf(fp, "Type:GENERATED_TABLE, ");
            fprintf(fp, "RefId: %lu}\n", item.ref_id_);
        }
        print_indentation(fp, level);
        fprintf(fp, "<TableItemList End>\n");
    }

    if (column_items_.size() > 0)
    {
        print_indentation(fp, level);
        fprintf(fp, "<ColumnItemList Begin>\n");
        for (i = 0; i < column_items_.size(); ++i)
        {
            ColumnItem& item = column_items_[i];
            print_indentation(fp, level + 1);
            fprintf(fp, "{Num %d, ColumnId:%lu, ColumnName:%.*s, TableRef:%lu}\n", i,
                    item.column_id_, (int32_t)item.column_name_.size(), item.column_name_.data(),
                    item.table_id_);
        }
        print_indentation(fp, level);
        fprintf(fp, "<ColumnItemList End>\n");
    }

    if (where_expr_ids_.size() > 0)
    {
        print_indentation(fp, level);
        fprintf(fp, "WHERE ::=");
        for (i = 0; i < where_expr_ids_.size(); i++)
            fprintf(fp, " <%lu>", where_expr_ids_[i]);
        fprintf(fp, "\n");
    }
}

int64_t ObStmt::make_stmt_string(ResultPlan& result_plan, string &assembled_sql)
{
    return OB_SUCCESS;
}

int64_t ObStmt::make_exec_plan_unit_string(ResultPlan& result_plan, string where_conditions, vector<schema_shard*> &shard_info,string &assembled_sql)
{
    return OB_SUCCESS;
}


/**************************************************
Funtion     :   decompose_where_items
Author      :   qinbo
Date        :   2013.9.24
Description :   generate distributed where conditions items
Input       :   ObRawExpr* sql_expr
Output      :   vector<vector<ObRawExpr*> > &atomic_exprs_array
return      :   
 **************************************************/
int ObStmt::decompose_where_items(ObRawExpr* sql_expr, vector<vector<ObRawExpr*> > &atomic_exprs_array)
{
    return OB_SUCCESS;
}



