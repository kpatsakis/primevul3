xb_mysql_numrows(MYSQL *connection, const char *query, bool die_on_error)
{
	my_ulonglong rows_count = 0;
	MYSQL_RES *result = xb_mysql_query(connection, query, true,
		die_on_error);
	if (result) {
		rows_count = mysql_num_rows(result);
		mysql_free_result(result);
	}
	return rows_count;
}