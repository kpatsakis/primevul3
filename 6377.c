write_galera_info(MYSQL *connection)
{
	char *state_uuid = NULL, *state_uuid55 = NULL;
	char *last_committed = NULL, *last_committed55 = NULL;
	bool result;

	mysql_variable status[] = {
		{"Wsrep_local_state_uuid", &state_uuid},
		{"wsrep_local_state_uuid", &state_uuid55},
		{"Wsrep_last_committed", &last_committed},
		{"wsrep_last_committed", &last_committed55},
		{NULL, NULL}
	};

	/* When backup locks are supported by the server, we should skip
	creating xtrabackup_galera_info file on the backup stage, because
	wsrep_local_state_uuid and wsrep_last_committed will be inconsistent
	without blocking commits. The state file will be created on the prepare
	stage using the WSREP recovery procedure. */
	if (have_backup_locks) {
		return(true);
	}

	read_mysql_variables(connection, "SHOW STATUS", status, true);

	if ((state_uuid == NULL && state_uuid55 == NULL)
		|| (last_committed == NULL && last_committed55 == NULL)) {
		msg("Failed to get master wsrep state from SHOW STATUS.\n");
		result = false;
		goto cleanup;
	}

	result = backup_file_printf(XTRABACKUP_GALERA_INFO,
		"%s:%s\n", state_uuid ? state_uuid : state_uuid55,
			last_committed ? last_committed : last_committed55);

cleanup:
	free_mysql_variables(status);

	return(result);
}