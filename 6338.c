have_queries_to_wait_for(MYSQL *connection, uint threshold)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	bool all_queries;

	result = xb_mysql_query(connection, "SHOW FULL PROCESSLIST", true);

	all_queries = (opt_lock_wait_query_type == QUERY_TYPE_ALL);
	while ((row = mysql_fetch_row(result)) != NULL) {
		const char	*info		= row[7];
		char		*id		= row[0];
		int		duration;

		duration = (row[5] != NULL) ? atoi(row[5]) : 0;

		if (info != NULL
		    && duration >= (int)threshold
		    && ((all_queries && is_query(info))
		    	|| is_update_query(info))) {
			msg_ts("Waiting for query %s (duration %d sec): %s",
			       id, duration, info);
			mysql_free_result(result);
			return(true);
		}
	}

	mysql_free_result(result);

	return(false);
}