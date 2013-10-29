#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#include "parse_malloc.h"
#include "parse_node.h"
#include "utility.h"
#include "ob_define.h"
#include "ob_logical_plan.h"
#include "ob_select_stmt.h"
#include "ob_delete_stmt.h"
#include "ob_insert_stmt.h"
#include "ob_update_stmt.h"
#include "dml_build_plan.h"
#include "jd_exec_plan.h"


using namespace std;
using namespace oceanbase;
using namespace oceanbase::common;
using namespace oceanbase::sql;

meta_reader *g_metareader = NULL;

int
sql_parser(string sql)
{
    int ret = 0;
    QueryActuator query_actuator;

    query_actuator.init_exec_plan("qinbo");
            
    FinalExecPlan* final_exec_plan = NULL;
    if (OB_SUCCESS != (ret = query_actuator.generate_exec_plan(sql, final_exec_plan)))
    {
        fprintf(stderr, "generate_exec_plan: %s\n", query_actuator.get_result_plan().err_stat_.err_msg_);
        ret = OB_ERR_GEN_PLAN;
        return ret;
    }

    query_actuator.release_exec_plan();
}


int main(void)
{
    string sql1 = "INSERT INTO persons (lastname, address) VALUES ('Wilson', 'Champs-Elysees')";
    string sql2 = "SELECT * FROM persons";
    string sql3 = "SELECT COUNT(lastname), address AS addr FROM persons WHERE id IN (select id from order_list) GROUP BY lastname";

    string sql4 = "SELECT lastname, address FROM persons \
WHERE lastname='Bush' OR address='Adams' AND id = 10\
GROUP BY lastname HAVING(count(lastname) > 10) ORDER BY lastname desc limit 100, 10";

    string sql7 = "select lastname,address FROM persons \
WHERE id=(select id from persons where lastname='NEW YORK')";


    string sql5 = "SELECT persons.lastname, persons.address, order_list.order_desc \
FROM persons \
JOIN order_list \
ON persons.id = order_list.id \
ORDER BY persons.lastname";

    string sql6 = "select id, order_desc from (select * from order_list) ooxx where id > 50";

    string sql9 = "SELECT id FROM persons WHERE lastname = 'Attendee' AND address = 'beijing'  AND (id = 147 OR id = 155)";

    string sql8 = "SELECT id FROM persons    \
UNION ALL   \
SELECT id FROM order_list";

    string sql10 = "UPDATE persons SET address = 'BEIJING' WHERE lastname = 'Wilson'";
    string sql11 = "SELECT COUNT(lastname), address AS addr FROM persons";

    string sql12 = "INSERT INTO persons VALUES ('Gates', 'Bill', 100), ('qinbo', 'qin', 101)";

    string sql13 = "INSERT INTO persons (address, lastname)  \
SELECT address,lastname FROM persons";

    string sql14 = "DELETE FROM persons  WHERE address = 'beijing'";

    string sql15 = "SELECT COUNT(lastname), address AS addr FROM persons WHERE lastname > 'cao1' AND id = 10 AND (id = 15 OR id = 100)";
    string sqls = "select t.id, s.lastname from persons t join persons s on t.id = s.id";
    string sql_multi_table = "select * from persons, order_list \
        WHERE persons.lastname = 'Attendee' AND persons.address = 'beijing'  AND (persons.id = 147 OR persons.id = 155)";

    //sql_parser(sql4);
    //sql_parser(sql6);
    sql_parser(sql_multi_table);
    //sql_parser(sql11);
    //sql_parser(sql14);
    //sql_parser(sql9);
    //sql_parser(sql8);
    return 1;
}
