lock_tables_maybe(MYSQL *connection, int timeout, int retry_count)
{
	if (tables_locked || opt_lock_ddl_per_table) {
		return(true);
	}

	if (have_backup_locks) {
		return lock_tables_for_backup(connection, timeout, retry_count);
	}

	if (have_lock_wait_timeout) {
		char query[200];

		ut_snprintf(query, sizeof(query),
			    "SET SESSION lock_wait_timeout=%d", timeout);

		xb_mysql_query(connection, query, false);
	}

	if (!opt_lock_wait_timeout && !opt_kill_long_queries_timeout) {

		/* We do first a FLUSH TABLES. If a long update is running, the
		FLUSH TABLES will wait but will not stall the whole mysqld, and
		when the long update is done the FLUSH TABLES WITH READ LOCK
		will start and succeed quickly. So, FLUSH TABLES is to lower
		the probability of a stage where both mysqldump and most client
		connections are stalled. Of course, if a second long update
		starts between the two FLUSHes, we have that bad stall.

		Option lock_wait_timeout serve the same purpose and is not
		compatible with this trick.
		*/

		msg_ts("Executing FLUSH NO_WRITE_TO_BINLOG TABLES...\n");

		xb_mysql_query(connection,
			       "FLUSH NO_WRITE_TO_BINLOG TABLES", false);
	}

	if (opt_lock_wait_timeout) {
		if (!wait_for_no_updates(connection, opt_lock_wait_timeout,
					 opt_lock_wait_threshold)) {
			return(false);
		}
	}

	msg_ts("Executing FLUSH TABLES WITH READ LOCK...\n");

	if (opt_kill_long_queries_timeout) {
		start_query_killer();
	}

	if (have_galera_enabled) {
		xb_mysql_query(connection,
				"SET SESSION wsrep_causal_reads=0", false);
	}

	xb_mysql_query(connection, "FLUSH TABLES WITH READ LOCK", false);

	if (opt_kill_long_queries_timeout) {
		stop_query_killer();
	}

	tables_locked = true;

	return(true);
}