xb_mysql_query(MYSQL *connection, const char *query, bool use_result,
		bool die_on_error)
{
	MYSQL_RES *mysql_result = NULL;

	if (mysql_query(connection, query)) {
		msg("Error: failed to execute query '%s': %u (%s) %s\n", query,
		    mysql_errno(connection),
		    mysql_errno_to_sqlstate(mysql_errno(connection)),
		    mysql_error(connection));
		if (die_on_error) {
			exit(EXIT_FAILURE);
		}
		return(NULL);
	}

	/* store result set on client if there is a result */
	if (mysql_field_count(connection) > 0) {
		if ((mysql_result = mysql_store_result(connection)) == NULL) {
			msg("Error: failed to fetch query result %s: %s\n",
			    query, mysql_error(connection));
			exit(EXIT_FAILURE);
		}

		if (!use_result) {
			mysql_free_result(mysql_result);
		}
	}

	return mysql_result;
}