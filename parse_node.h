#ifndef _PARSE_NODE_H_
#define _PARSE_NODE_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "sql_item_type.h"

#define MAX_ERROR_MSG 1024

struct _ParseNode;

typedef struct _ParseNode
{
  SqlItemType   type_;

  /* attributes for terminal node, it is real value */
  int64_t      value_;
  const char*  str_value_;

  /* attributes for non-terninal node, which has children */
  int32_t      num_child_;
  struct _ParseNode** children_;
  
  // BuildPlanFunc m_fnBuildPlan;
} ParseNode;

typedef struct _ParseResult
{
  void*   yyscan_info_;
  ParseNode* result_tree_;
  const char*   input_sql_;
  int     input_sql_len_;
  void*   malloc_pool_;
  char    error_msg_[MAX_ERROR_MSG];
  int     start_col_;
  int     end_col_;
  int     line_;
  int     yycolumn_;
  int     yylineno_;
  char*   tmp_literal_;
} ParseResult;


#ifdef __cplusplus
extern "C" {
#endif

extern const char* get_type_name(int type);
extern const char* get_type_symbol(int type);

extern int parse_init(ParseResult* p);

extern int parse_terminate(ParseResult* p);

extern int parse_sql(ParseResult *p, const char* pszSql, size_t iLen);

extern void print_tree(ParseNode* pRoot, int level);

extern void destroy_tree(ParseNode* pRoot);

#ifdef __cplusplus
}
#endif

#endif //_PARSE_NODE_H_


