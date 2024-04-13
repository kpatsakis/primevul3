kill_long_queries(MYSQL *connection, uint timeout)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	bool all_queries;
	char kill_stmt[100];

	result = xb_mysql_query(connection, "SHOW FULL PROCESSLIST", true);

	all_queries = (opt_kill_long_query_type == QUERY_TYPE_ALL);
	while ((row = mysql_fetch_row(result)) != NULL) {
		const char	*info		= row[7];
		int		duration	= atoi(row[5]);
		char		*id		= row[0];

		if (info != NULL &&
		    duration >= (int)timeout &&
		    ((all_queries && is_query(info)) ||
		    	is_select_query(info))) {
			msg_ts("Killing query %s (duration %d sec): %s\n",
			       id, duration, info);
			ut_snprintf(kill_stmt, sizeof(kill_stmt),
				    "KILL %s", id);
			xb_mysql_query(connection, kill_stmt, false, false);
		}
	}

	mysql_free_result(result);
}