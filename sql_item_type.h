#ifndef _SQL_ITEM_TYPE_H_
#define _SQL_ITEM_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum _SqlItemType
    {
        T_INVALID = 0, // Min tag

        /* Literal data type tags */
        T_INT,
        T_STRING,
        T_BINARY,
        T_DATE, // WE may need time and timestamp here
        T_FLOAT,
        T_DOUBLE,
        T_DECIMAL,
        T_BOOL,
        T_NULL,
        T_QUESTIONMARK,
        T_UNKNOWN,

        /* Reference type tags*/
        T_REF_COLUMN,
        T_REF_EXPR,
        T_REF_QUERY,

        T_HINT, // Hint message from rowkey
        T_IDENT,
        T_STAR,
        T_SYSTEM_VARIABLE,
        T_TEMP_VARIABLE,

        /* Data type tags */
        T_TYPE_INTEGER,
        T_TYPE_FLOAT,
        T_TYPE_DOUBLE,
        T_TYPE_DECIMAL,
        T_TYPE_BOOLEAN,
        T_TYPE_DATE,
        T_TYPE_TIME,
        T_TYPE_DATETIME,
        T_TYPE_TIMESTAMP,
        T_TYPE_CHARACTER,
        T_TYPE_VARCHAR,
        T_TYPE_CREATETIME,
        T_TYPE_MODIFYTIME,

        // @note !! the order of the following tags between T_MIN_OP and T_MAX_OP SHOULD NOT be changed
        /* Operator tags */
        T_MIN_OP = 100,
        /* 1. arithmetic operators */
        T_OP_NEG, // negative
        T_OP_POS, // positive
        T_OP_ADD,
        T_OP_MINUS,
        T_OP_MUL,
        T_OP_DIV,
        T_OP_POW,
        T_OP_REM, // remainder
        T_OP_MOD,
        T_OP_EQ, /* 2. Bool operators */
        T_OP_LE,
        T_OP_LT,
        T_OP_GE,
        T_OP_GT,
        T_OP_NE,
        T_OP_IS,
        T_OP_IS_NOT,
        T_OP_BTW,
        T_OP_NOT_BTW,
        T_OP_LIKE,
        T_OP_NOT_LIKE,
        T_OP_NOT,
        T_OP_AND,
        T_OP_OR,
        T_OP_IN,
        T_OP_NOT_IN,
        T_OP_ARG_CASE,
        T_OP_CASE,
        T_OP_ROW,
        T_OP_EXISTS,

        T_OP_CNN, /* 3. String operators */

        T_FUN_SYS, // system functions, CHAR_LENGTH, ROUND, etc.
        T_OP_LEFT_PARAM_END,
        T_MAX_OP,

        T_FUN_SYS_CAST, // special system function : CAST(val AS type)

        /* 4. name filed specificator */
        T_OP_NAME_FIELD,

        // @note !! the order of the following tags between T_FUN_MAX and T_FUN_AVG SHOULD NOT be changed
        /* Function tags */
        T_FUN_MAX,
        T_FUN_MIN,
        T_FUN_SUM,
        T_FUN_COUNT,
        T_FUN_AVG,

        /* parse tree node tags */
        T_DELETE,
        T_SELECT,
        T_UPDATE,
        T_INSERT,
        T_EXPLAIN,
        T_LINK_NODE,
        T_ASSIGN_LIST,
        T_ASSIGN_ITEM,
        T_STMT_LIST,
        T_EXPR_LIST,
        T_WHEN_LIST,
        T_PROJECT_LIST,
        T_PROJECT_ITEM,
        T_FROM_LIST,
        T_SET_UNION,
        T_SET_INTERSECT,
        T_SET_EXCEPT,
        T_WHERE_CLAUSE,
        T_LIMIT_CLAUSE,
        T_SORT_LIST,
        T_SORT_KEY,
        T_SORT_ASC,
        T_SORT_DESC,
        T_ALL,
        T_DISTINCT,
        T_ALIAS,
        T_PROJECT_STRING,
        T_COLUMN_LIST,
        T_VALUE_LIST,
        T_VALUE_VECTOR,
        T_JOINED_TABLE,
        T_JOIN_INNER,
        T_JOIN_FULL,
        T_JOIN_LEFT,
        T_JOIN_RIGHT,
        T_CASE,
        T_WHEN,

        T_CREATE_TABLE,
        T_TABLE_ELEMENT_LIST,
        T_TABLE_OPTION_LIST,
        T_PRIMARY_KEY,
        T_COLUMN_DEFINITION,
        T_COLUMN_ATTRIBUTES,
        T_CONSTR_NOT_NULL,
        T_CONSTR_NULL,
        T_CONSTR_DEFAULT,
        T_CONSTR_AUTO_INCREMENT,
        T_CONSTR_PRIMARY_KEY,
        T_IF_NOT_EXISTS,
        T_IF_EXISTS,
        T_EXPIRE_INFO,
        T_TABLET_MAX_SIZE,
        T_TABLET_BLOCK_SIZE,
        T_REPLICA_NUM,
        T_COMPRESS_METHOD,
        T_USE_BLOOM_FILTER,
        T_CONSISTENT_MODE,
        T_DROP_TABLE,
        T_TABLE_LIST,

        T_ALTER_TABLE,
        T_ALTER_ACTION_LIST,
        T_TABLE_RENAME,
        T_COLUMN_DROP,
        T_COLUMN_ALTER,
        T_COLUMN_RENAME,

        T_ALTER_SYSTEM,
        T_SYTEM_ACTION_LIST,
        T_SYSTEM_ACTION,
        T_CLUSTER,
        T_SERVER_ADDRESS,

        T_SHOW_DATABASES,
        T_SHOW_TABLES,
        T_SHOW_VARIABLES,
        T_SHOW_COLUMNS,
        T_SHOW_SCHEMA,
        T_SHOW_CREATE_TABLE,
        T_SHOW_TABLE_STATUS,
        T_SHOW_PARAMETERS,
        T_SHOW_SERVER_STATUS,
        T_SHOW_WARNINGS,
        T_SHOW_GRANTS,
        T_SHOW_PROCESSLIST,
        T_SHOW_LIMIT,

        T_CREATE_USER,
        T_CREATE_USER_SPEC,
        T_DROP_USER,
        T_SET_PASSWORD,
        T_RENAME_USER,
        T_RENAME_INFO,
        T_LOCK_USER,
        T_GRANT,
        T_PRIVILEGES,
        T_PRIV_LEVEL,
        T_PRIV_TYPE,
        T_USERS,
        T_REVOKE,
        T_BEGIN,
        T_COMMIT,
        T_PREPARE,
        T_DEALLOCATE,
        T_EXECUTE,
        T_ARGUMENT_LIST,
        T_VARIABLE_SET,
        T_VAR_VAL,
        T_ROLLBACK,

        T_HINT_OPTION_LIST,
        T_READ_STATIC,

        T_MAX,

    } SqlItemType;

    typedef enum SqlPrivilegeType
    {
        T_PRIV_INVALID = 0, /*invalid privilege type*/
        T_PRIV_ALL = 1,
        T_PRIV_ALTER = 2,
        T_PRIV_CREATE = 3,
        T_PRIV_CREATE_USER = 4,
        T_PRIV_DELETE = 5,
        T_PRIV_DROP = 6,
        T_PRIV_GRANT_OPTION = 7,
        T_PRIV_INSERT = 8,
        T_PRIV_UPDATE = 9,
        T_PRIV_SELECT = 10,
        T_PRIV_REPLACE = 11,
        T_PRIV_NUM
    } SqlPrivilegeType;


#ifdef __cplusplus
}
#endif

#endif //_SQL_ITEM_TYPE_H_
