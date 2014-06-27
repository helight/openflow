#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>

using namespace std;
using namespace boost::property_tree;

int main(void)
{
    ptree pt;
    read_xml("demo.xml", pt);

    MYSQL mysql;
    MYSQL_RES *result = NULL;

    mysql_init(&mysql);

    mysql_real_connect(&mysql, "localhost", "root", "renzhen",
                       "openflowDB", 3306, NULL, 0);

    string sql = "select * from job;";
    mysql_query(&mysql, sql.c_str());
    result = mysql_store_result(&mysql);
    int rowcount = mysql_num_rows(result);
    cout << rowcount << endl;

    mysql_close(&mysql);

    return 0;
}
