/**
 * (C) 2010-2012 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * ob_show_stmt.cpp
 *
 * Authors:
 *   Guibin Du <tianguan.dgb@taobao.com>
 *
 */
#include <string> 
#include "sql_show_stmt.h"
#include "parse_malloc.h"
#include "utility.h"

using namespace jdbd::sql;
using namespace jdbd::common;
using namespace std;

ObShowStmt::ObShowStmt(StmtType stmt_type)
: ObStmt(stmt_type), sys_table_id_(OB_INVALID_ID), show_table_id_(OB_INVALID_ID)
, global_scope_(false), offset_(0), row_count_(-1), count_warnings_(false)
{
}

ObShowStmt::~ObShowStmt()
{
}

void ObShowStmt::set_like_pattern(const string like_pattern)
{
    like_pattern_ = like_pattern;
}

void ObShowStmt::print(FILE* fp, int32_t level, int32_t index)
{
  UNUSED(index);
  print_indentation(fp, level);
  fprintf(fp, "ObShowStmt %d Begin\n", index);
  print_indentation(fp, level + 1);
  fprintf(fp, "Show Statement Type ::= <%d>\n", get_stmt_type());
  ObStmt::print(fp, level + 1);
  if (sys_table_id_ != OB_INVALID_ID)
  {
    print_indentation(fp, level + 1);
    fprintf(fp, "Sys Table Id ::= <%ld>\n", sys_table_id_);
  }
  if (show_table_id_ != OB_INVALID_ID)
  {
    print_indentation(fp, level + 1);
    fprintf(fp, "Show Table Id ::= <%ld>\n", show_table_id_);
  }
  if (get_stmt_type() == T_SHOW_VARIABLES)
  {
    print_indentation(fp, level + 1);
    fprintf(fp, "Is Global ::= <%s>\n", global_scope_ ? "True" : "False");
  }
  if (like_pattern_.size() > 0)
  {
        print_indentation(fp, level + 1);
        fprintf(fp, "Like ::= '%.*s'\n", (int)like_pattern_.size(), like_pattern_.data());
  }
  if (get_stmt_type() == ObBasicStmt::T_SHOW_WARNINGS)
  {
    print_indentation(fp, level + 1);
    if (count_warnings_)
      fprintf(fp, "COUNT(*) ::= <TRUE>\n");
    else
      fprintf(fp, "Limit ::= <%ld, %ld>\n", offset_, row_count_);
  }
  if (get_stmt_type() == ObBasicStmt::T_SHOW_GRANTS)
  {
    print_indentation(fp, level + 1);
    fprintf(fp, "User name ::= '%.*s,\n", (int)user_name_.size(), user_name_.data());
  }
  print_indentation(fp, level);
  fprintf(fp, "ObShowStmt %d End\n", index);
}
