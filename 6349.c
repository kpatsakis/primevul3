check_dump_innodb_buffer_pool(MYSQL *connection)
{
	if (!innodb_buffer_pool_dump) {
		return;
	}
	const ssize_t timeout = opt_dump_innodb_buffer_pool_timeout;

	char *innodb_buffer_pool_dump_status;
	char change_bp_dump_pct_query[100];


	mysql_variable status[] = {
			{"Innodb_buffer_pool_dump_status",
			&innodb_buffer_pool_dump_status},
			{NULL, NULL}
	};

	read_mysql_variables(connection,
		"SHOW STATUS LIKE "
		"'Innodb_buffer_pool_dump_status'",
		status, true);

	/* check if dump has been completed */
	msg_ts("Checking if InnoDB buffer pool dump has completed\n");
	while (!strstr(innodb_buffer_pool_dump_status,
					"dump completed at")) {
		if (innodb_buffer_pool_dump_start_time +
				timeout < (ssize_t)my_time(MY_WME)){
			msg_ts("InnoDB Buffer Pool Dump was not completed "
				"after %d seconds... Adjust "
				"--dump-innodb-buffer-pool-timeout if you "
				"need higher wait time before copying %s.\n",
				opt_dump_innodb_buffer_pool_timeout,
				buffer_pool_filename);
			break;
		}

		read_mysql_variables(connection,
			"SHOW STATUS LIKE 'Innodb_buffer_pool_dump_status'",
			status, true);

		os_thread_sleep(1000000);
	}

	free_mysql_variables(status);

	/* restore original innodb_buffer_pool_dump_pct */
	if (opt_dump_innodb_buffer_pool_pct != 0 &&
			innodb_buffer_pool_dump_pct) {
		ut_snprintf(change_bp_dump_pct_query,
			sizeof(change_bp_dump_pct_query),
			"SET GLOBAL innodb_buffer_pool_dump_pct = %u",
			original_innodb_buffer_pool_dump_pct);
		xb_mysql_query(mysql_connection,
			change_bp_dump_pct_query, false);
	}
}