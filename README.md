﻿
<BODY style="MARGIN: 10px"><DIV>
<DIV>说明</DIV>
<DIV>=========</DIV>
<DIV>该项目为mysql&nbsp;sql解析器，词法分析和语法分析基于oceanbase&nbsp;0.4版本，新写sql优化以及根据本地化处理原则和数据路由生成执行计划。</DIV>
<DIV>整体过程如下：</DIV>
<DIV>&nbsp;</DIV>
<DIV>编译：</DIV>
<DIV>STEP1：$&nbsp;sh&nbsp;gen_parser.sh</DIV>
<DIV>STEP2:&nbsp;$&nbsp;make</DIV>
<DIV>&nbsp;</DIV>
<DIV>执行：&nbsp;$&nbsp;./sql_parser</DIV>
<DIV>&nbsp;</DIV>
<DIV>select&nbsp;join语句如下时，解析后的语法树：</DIV>
<DIV>$&nbsp;./sql_parser.exe</DIV>
<DIV>&nbsp;</DIV>
<DIV>&lt;&lt;Part&nbsp;1&nbsp;:&nbsp;SQL&nbsp;STRING&gt;&gt;&nbsp;SELECT&nbsp;tt.id,&nbsp;tt.name,&nbsp;pp.age&nbsp;FROM&nbsp;tt&nbsp;JOIN&nbsp;pp&nbsp;ON&nbsp;tt.id&nbsp;=&nbsp;pp.id&nbsp;where&nbsp;tt.id&nbsp;=&nbsp;100&nbsp;ORDER&nbsp;BY&nbsp;tt.name</DIV>
<DIV>&lt;&lt;Part&nbsp;2&nbsp;:&nbsp;PARSE&nbsp;TREE&gt;&gt;</DIV>
<DIV>|-T_STMT_LIST</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;|-T_SELECT</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_PROJECT_LIST</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_PROJECT_STRING</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;tt</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;id</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_PROJECT_STRING</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;tt</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;name</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_PROJECT_STRING</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;pp</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;age</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_FROM_LIST</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_JOINED_TABLE</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_JOIN_INNER</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;tt</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;pp</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_EQ</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;tt</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;id</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;pp</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;id</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_EQ</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;tt</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;id</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_INT&nbsp;:&nbsp;100</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_SORT_LIST</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_SORT_KEY</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_OP_NAME_FIELD</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;tt</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_IDENT&nbsp;:&nbsp;name</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-T_SORT_ASC</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-NULL</DIV>
<DIV>&nbsp;</DIV>
<DIV>&lt;&lt;Part&nbsp;3&nbsp;:&nbsp;LOGICAL&nbsp;PLAN&gt;&gt;</DIV>
<DIV>&nbsp;</DIV>
<DIV>&lt;LogicalPlan&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&lt;StmtList&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObSelectStmt&nbsp;0&nbsp;Begin</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;TableItemList&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{Num&nbsp;0,&nbsp;TableId:1,&nbsp;TableName:tt,&nbsp;AliasName:NULL,&nbsp;Type:BASE_TABLE,&nbsp;RefId:&nbsp;1}</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{Num&nbsp;1,&nbsp;TableId:2,&nbsp;TableName:pp,&nbsp;AliasName:NULL,&nbsp;Type:BASE_TABLE,&nbsp;RefId:&nbsp;2}</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;TableItemList&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ColumnItemList&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{Num&nbsp;0,&nbsp;ColumnId:1,&nbsp;ColumnName:id,&nbsp;TableRef:1}</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{Num&nbsp;1,&nbsp;ColumnId:6,&nbsp;ColumnName:id,&nbsp;TableRef:2}</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{Num&nbsp;2,&nbsp;ColumnId:2,&nbsp;ColumnName:name,&nbsp;TableRef:1}</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{Num&nbsp;3,&nbsp;ColumnId:8,&nbsp;ColumnName:age,&nbsp;TableRef:2}</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ColumnItemList&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;WHERE&nbsp;::=&nbsp;&lt;5&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SELECT&nbsp;::=&nbsp;&lt;2,&nbsp;id&gt;,&nbsp;&lt;3,&nbsp;name&gt;,&nbsp;&lt;4,&nbsp;age&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FROM&nbsp;::=&nbsp;&lt;1&gt;&nbsp;INNER&nbsp;JOIN&nbsp;&lt;2&gt;&nbsp;ON&nbsp;&lt;1&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ORDER&nbsp;BY&nbsp;::=&nbsp;&lt;6,&nbsp;ASC&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObSelectStmt&nbsp;0&nbsp;End</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&lt;/StmtList&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&lt;ExprList&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;0&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;expr_id&nbsp;=&nbsp;1</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(table_id&nbsp;:&nbsp;column_id)&nbsp;=&nbsp;(NULL&nbsp;:&nbsp;65519)</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryOpRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_OP_EQ</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[1,&nbsp;1]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[2,&nbsp;6]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;0&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;1&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;expr_id&nbsp;=&nbsp;2</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(table_id&nbsp;:&nbsp;column_id)&nbsp;=&nbsp;(NULL&nbsp;:&nbsp;65518)</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[1,&nbsp;1]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;1&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;2&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;expr_id&nbsp;=&nbsp;3</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(table_id&nbsp;:&nbsp;column_id)&nbsp;=&nbsp;(NULL&nbsp;:&nbsp;65517)</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[1,&nbsp;2]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;2&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;3&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;expr_id&nbsp;=&nbsp;4</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(table_id&nbsp;:&nbsp;column_id)&nbsp;=&nbsp;(NULL&nbsp;:&nbsp;65516)</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[2,&nbsp;8]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;3&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;4&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;expr_id&nbsp;=&nbsp;5</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(table_id&nbsp;:&nbsp;column_id)&nbsp;=&nbsp;(NULL&nbsp;:&nbsp;65515)</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryOpRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_OP_EQ</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[1,&nbsp;1]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObConstRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_INT&nbsp;:&nbsp;100</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;4&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;5&nbsp;Begin&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;expr_id&nbsp;=&nbsp;6</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(table_id&nbsp;:&nbsp;column_id)&nbsp;=&nbsp;(1&nbsp;:&nbsp;2)</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ObBinaryRefRawExpr</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T_REF_COLUMN&nbsp;:&nbsp;[table_id,&nbsp;column_id]&nbsp;=&nbsp;[1,&nbsp;2]</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;ObSqlRawExpr&nbsp;5&nbsp;End&gt;</DIV>
<DIV>&nbsp;&nbsp;&nbsp;&nbsp;&lt;/ExprList&gt;</DIV>
<DIV>&lt;/LogicalPlan&gt;</DIV>
<DIV>&nbsp;</DIV>
<DIV>&nbsp;</DIV>
<DIV>最终经过tablet定位以及sql重新拼装，最终生成的执行计划为：</DIV>
<DIV>I0227&nbsp;14:07:37&nbsp;10661&nbsp;jd_exec_plan.cpp:1712]:&nbsp;exec_plan_unit&nbsp;shard&nbsp;name:&nbsp;tt1</DIV>
<DIV>I0227&nbsp;14:07:37&nbsp;10661&nbsp;jd_exec_plan.cpp:1713]:&nbsp;exec_plan_unit&nbsp;SQL&nbsp;name&nbsp;&nbsp;:&nbsp;SELECT&nbsp;tt.id,&nbsp;tt.name,&nbsp;pp.age&nbsp;FROM&nbsp;tt1&nbsp;AS&nbsp;tt&nbsp;&nbsp;JOIN&nbsp;pp1&nbsp;AS&nbsp;pp&nbsp;&nbsp;ON&nbsp;tt.id&nbsp;=&nbsp;pp.id&nbsp;&nbsp;&nbsp;WHERE&nbsp;tt.id&nbsp;=&nbsp;100&nbsp;&nbsp;ORDER&nbsp;BY&nbsp;tt.name&nbsp;ASC&nbsp;LIMIT&nbsp;0,&nbsp;100000000</DIV>
<DIV>&nbsp;</DIV>
<DIV>然后就可以把sql语句发送到相应的分片中去执行。</DIV>
<DIV>&nbsp;</DIV>
<DIV>&nbsp;</DIV>
<DIV>DATE&nbsp;:&nbsp;2014.2.26</DIV>
<DIV>EMAIL:&nbsp;gqinbo@gmail.com</DIV>
<DIV>&nbsp;</DIV></DIV></BODY>
