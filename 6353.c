lock_tables_for_backup(MYSQL *connection, int timeout, int retry_count)
{
	if (have_lock_wait_timeout) {
		char query[200];

		ut_snprintf(query, sizeof(query),
			    "SET SESSION lock_wait_timeout=%d", timeout);

		xb_mysql_query(connection, query, false);
	}

	if (have_backup_locks) {
		for (int i = 0; i <= retry_count; ++i) {
			msg_ts("Executing LOCK TABLES FOR BACKUP...\n");
			xb_mysql_query(connection, "LOCK TABLES FOR BACKUP",
				       false, false);
			uint err = mysql_errno(connection);
			if (err == ER_LOCK_WAIT_TIMEOUT) {
				os_thread_sleep(1000000);
				continue;
			}
			if (err == 0) {
				tables_locked = true;
			}
			break;
		}
		if (!tables_locked) {
			exit(EXIT_FAILURE);
		}
		return(true);
	}

	msg_ts("Error: LOCK TABLES FOR BACKUP is not supported.\n");

	return(false);
}