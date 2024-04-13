read_mysql_variables(MYSQL *connection, const char *query, mysql_variable *vars,
	bool vertical_result)
{
	MYSQL_RES *mysql_result = xb_mysql_query(connection, query, true);
	read_mysql_variables_from_result(mysql_result, vars, vertical_result);
	mysql_free_result(mysql_result);
}