dump_innodb_buffer_pool(MYSQL *connection)
{
	innodb_buffer_pool_dump = has_innodb_buffer_pool_dump();
	innodb_buffer_pool_dump_pct = has_innodb_buffer_pool_dump_pct();
	if (!innodb_buffer_pool_dump) {
		return;
	}

	innodb_buffer_pool_dump_start_time = (ssize_t)my_time(MY_WME);

	char *buf_innodb_buffer_pool_dump_pct;
	char change_bp_dump_pct_query[100];

	/* Verify if we need to change innodb_buffer_pool_dump_pct */
	if (opt_dump_innodb_buffer_pool_pct != 0 &&
		innodb_buffer_pool_dump_pct) {
			mysql_variable variables[] = {
				{"innodb_buffer_pool_dump_pct",
				&buf_innodb_buffer_pool_dump_pct},
				{NULL, NULL}
			};
			read_mysql_variables(connection,
				"SHOW GLOBAL VARIABLES "
				"LIKE 'innodb_buffer_pool_dump_pct'",
				variables, true);

			original_innodb_buffer_pool_dump_pct =
					atoi(buf_innodb_buffer_pool_dump_pct);

			free_mysql_variables(variables);
			ut_snprintf(change_bp_dump_pct_query,
				sizeof(change_bp_dump_pct_query),
				"SET GLOBAL innodb_buffer_pool_dump_pct = %u",
				opt_dump_innodb_buffer_pool_pct);
			msg_ts("Executing %s \n", change_bp_dump_pct_query);
			xb_mysql_query(mysql_connection,
				change_bp_dump_pct_query, false);
	}

	msg_ts("Executing SET GLOBAL innodb_buffer_pool_dump_now=ON...\n");
	xb_mysql_query(mysql_connection,
		"SET GLOBAL innodb_buffer_pool_dump_now=ON;", false);
}