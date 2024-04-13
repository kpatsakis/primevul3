wait_for_safe_slave(MYSQL *connection)
{
	char *read_master_log_pos = NULL;
	char *slave_sql_running = NULL;
	char *curr_slave_coordinates = NULL;
	char *prev_slave_coordinates = NULL;

	const int sleep_time = 3;
	const ssize_t routine_start_time = (ssize_t)my_time(MY_WME);
	const ssize_t timeout = opt_safe_slave_backup_timeout;

	int open_temp_tables = 0;
	bool result = true;

	mysql_variable status[] = {
		{"Read_Master_Log_Pos", &read_master_log_pos},
		{"Slave_SQL_Running", &slave_sql_running},
		{NULL, NULL}
	};

	sql_thread_started = false;

	read_mysql_variables(connection, "SHOW SLAVE STATUS", status, false);

	if (!(read_master_log_pos && slave_sql_running)) {
		msg("Not checking slave open temp tables for "
			"--safe-slave-backup because host is not a slave\n");
		goto cleanup;
	}

	if (strcmp(slave_sql_running, "Yes") == 0) {
		/* Stopping slave may take significant amount of time,
		take that into account as part of total timeout.
		*/
		sql_thread_started = true;
		xb_mysql_query(connection, "STOP SLAVE SQL_THREAD", false);
	}

retry:
	open_temp_tables = get_open_temp_tables(connection);
	msg_ts("Slave open temp tables: %d\n", open_temp_tables);
	curr_slave_coordinates = get_slave_coordinates(connection);

	while (open_temp_tables &&
	       routine_start_time + timeout > (ssize_t)my_time(MY_WME)) {
		msg_ts("Starting slave SQL thread, waiting %d seconds, then "
		       "checking Slave_open_temp_tables again (%d seconds of "
		       "sleep time remaining)...\n",
		       sleep_time,
		       (int)(routine_start_time + timeout - (ssize_t)my_time(MY_WME)));
		free(prev_slave_coordinates);
		prev_slave_coordinates = curr_slave_coordinates;
		curr_slave_coordinates = NULL;

		xb_mysql_query(connection, "START SLAVE SQL_THREAD", false);
		os_thread_sleep(sleep_time * 1000000);

		curr_slave_coordinates = get_slave_coordinates(connection);
		msg_ts("Slave pos:\n\tprev: %s\n\tcurr: %s\n",
		       prev_slave_coordinates, curr_slave_coordinates);
		if (prev_slave_coordinates && curr_slave_coordinates &&
		    strcmp(prev_slave_coordinates, curr_slave_coordinates) == 0) {
			msg_ts("Slave pos hasn't moved during wait period, "
			       "not stopping the SQL thread.\n");
		}
		else {
			msg_ts("Stopping SQL thread.\n");
			xb_mysql_query(connection, "STOP SLAVE SQL_THREAD", false);
		}

		open_temp_tables = get_open_temp_tables(connection);
		msg_ts("Slave open temp tables: %d\n", open_temp_tables);
	}

	if (open_temp_tables == 0) {
		/* We are in a race here, slave might open other temp tables
		inbetween last check and stop. So we have to re-check
		and potentially retry after stopping SQL thread. */
		xb_mysql_query(connection, "STOP SLAVE SQL_THREAD", false);
		open_temp_tables = get_open_temp_tables(connection);
		if (open_temp_tables != 0) {
			goto retry;
		}

		msg_ts("Slave is safe to backup.\n");
		goto cleanup;
	}

	result = false;

	msg_ts("Slave_open_temp_tables did not become zero after "
	       "%d seconds\n", opt_safe_slave_backup_timeout);

	msg_ts("Restoring SQL thread state to %s\n",
	       sql_thread_started ? "STARTED" : "STOPPED");
	if (sql_thread_started) {
		xb_mysql_query(connection, "START SLAVE SQL_THREAD", false);
	}
	else {
		xb_mysql_query(connection, "STOP SLAVE SQL_THREAD", false);
	}

cleanup:
	free(prev_slave_coordinates);
	free(curr_slave_coordinates);
	free_mysql_variables(status);

	return(result);
}