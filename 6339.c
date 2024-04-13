lock_binlog_maybe(MYSQL *connection, int timeout, int retry_count)
{
	if (have_backup_locks && !opt_no_lock && !binlog_locked) {
		if (have_lock_wait_timeout) {
			char query[200];

			ut_snprintf(query, sizeof(query),
				    "SET SESSION lock_wait_timeout=%d",
				    timeout);
		}

		for (int i = 0; i <= retry_count; ++i) {
			msg_ts("Executing LOCK BINLOG FOR BACKUP...\n");
			xb_mysql_query(connection, "LOCK BINLOG FOR BACKUP",
				       false, false);
			uint err = mysql_errno(connection);
			if (err == ER_LOCK_WAIT_TIMEOUT) {
				os_thread_sleep(1000000);
				continue;
			}
			if (err == 0) {
				binlog_locked = true;
			}
			break;
		}
		if (!binlog_locked) {
			exit(EXIT_FAILURE);
		}

		return(true);
	}

	return(false);
}