get_mysql_vars(MYSQL *connection)
{
	char *gtid_mode_var = NULL;
	char *version_var = NULL;
	char *version_comment_var = NULL;
	char *innodb_version_var = NULL;
	char *have_backup_locks_var = NULL;
	char *have_backup_safe_binlog_info_var = NULL;
	char *log_bin_var = NULL;
	char *lock_wait_timeout_var= NULL;
	char *wsrep_on_var = NULL;
	char *slave_parallel_workers_var = NULL;
	char *gtid_slave_pos_var = NULL;
	char *innodb_buffer_pool_filename_var = NULL;
	char *datadir_var = NULL;
	char *innodb_log_group_home_dir_var = NULL;
	char *innodb_log_file_size_var = NULL;
	char *innodb_log_files_in_group_var = NULL;
	char *innodb_data_file_path_var = NULL;
	char *innodb_data_home_dir_var = NULL;
	char *innodb_undo_directory_var = NULL;
	char *innodb_page_size_var = NULL;
	char *innodb_log_checksums_var = NULL;
	char *innodb_log_checksum_algorithm_var = NULL;
	char *innodb_checksum_algorithm_var = NULL;
	char *innodb_track_changed_pages_var = NULL;
	char *server_uuid_var = NULL;

	unsigned long server_version = mysql_get_server_version(connection);

	bool ret = true;

	mysql_variable mysql_vars[] = {
		{"have_backup_locks", &have_backup_locks_var},
		{"have_backup_safe_binlog_info",
		 &have_backup_safe_binlog_info_var},
		{"log_bin", &log_bin_var},
		{"lock_wait_timeout", &lock_wait_timeout_var},
		{"gtid_mode", &gtid_mode_var},
		{"version", &version_var},
		{"version_comment", &version_comment_var},
		{"innodb_version", &innodb_version_var},
		{"wsrep_on", &wsrep_on_var},
		{"slave_parallel_workers", &slave_parallel_workers_var},
		{"gtid_slave_pos", &gtid_slave_pos_var},
		{"innodb_buffer_pool_filename",
			&innodb_buffer_pool_filename_var},
		{"datadir", &datadir_var},
		{"innodb_log_group_home_dir", &innodb_log_group_home_dir_var},
		{"innodb_log_file_size", &innodb_log_file_size_var},
		{"innodb_log_files_in_group", &innodb_log_files_in_group_var},
		{"innodb_data_file_path", &innodb_data_file_path_var},
		{"innodb_data_home_dir", &innodb_data_home_dir_var},
		{"innodb_undo_directory", &innodb_undo_directory_var},
		{"innodb_page_size", &innodb_page_size_var},
		{"innodb_log_checksums", &innodb_log_checksums_var},
		{"innodb_log_checksum_algorithm",
			&innodb_log_checksum_algorithm_var},
		{"innodb_checksum_algorithm", &innodb_checksum_algorithm_var},
		{"innodb_track_changed_pages", &innodb_track_changed_pages_var},
		{"server_uuid", &server_uuid_var},
		{NULL, NULL}
	};

	read_mysql_variables(connection, "SHOW VARIABLES",
				mysql_vars, true);

	if (have_backup_locks_var != NULL && !opt_no_backup_locks) {
		have_backup_locks = true;
	}

	if (have_backup_safe_binlog_info_var == NULL &&
	    opt_binlog_info == BINLOG_INFO_LOCKLESS) {

		msg("Error: --binlog-info=LOCKLESS is not supported by the "
		    "server\n");
		return(false);
	}

	if (lock_wait_timeout_var != NULL) {
		have_lock_wait_timeout = true;
	}

	if (wsrep_on_var != NULL) {
		have_galera_enabled = true;
	}

	/* Check server version compatibility and detect server flavor */

	if (!(ret = check_server_version(server_version, version_var,
					 version_comment_var,
					 innodb_version_var))) {
		goto out;
	}

	if (server_version > 50500) {
		have_flush_engine_logs = true;
	}

	if (slave_parallel_workers_var != NULL
		&& atoi(slave_parallel_workers_var) > 0) {
		have_multi_threaded_slave = true;
	}

	if (innodb_buffer_pool_filename_var != NULL) {
		buffer_pool_filename = strdup(innodb_buffer_pool_filename_var);
	}

	if ((gtid_mode_var && strcmp(gtid_mode_var, "ON") == 0) ||
	    (gtid_slave_pos_var && *gtid_slave_pos_var)) {
		have_gtid_slave = true;
	}

	if (opt_binlog_info == BINLOG_INFO_AUTO) {

		if (have_backup_safe_binlog_info_var != NULL &&
		    !have_gtid_slave)
			opt_binlog_info = BINLOG_INFO_LOCKLESS;
		else if (log_bin_var != NULL && !strcmp(log_bin_var, "ON"))
			opt_binlog_info = BINLOG_INFO_ON;
		else
			opt_binlog_info = BINLOG_INFO_OFF;
	}

	msg("Using server version %s\n", version_var);

	if (!(ret = detect_mysql_capabilities_for_backup())) {
		goto out;
	}

	/* make sure datadir value is the same in configuration file */
	if (check_if_param_set("datadir")) {
		if (!directory_exists(mysql_data_home, false)) {
			msg("Warning: option 'datadir' points to "
			    "nonexistent directory '%s'\n", mysql_data_home);
		}
		if (!directory_exists(datadir_var, false)) {
			msg("Warning: MySQL variable 'datadir' points to "
			    "nonexistent directory '%s'\n", datadir_var);
		}
		if (!equal_paths(mysql_data_home, datadir_var)) {
			msg("Warning: option 'datadir' has different "
				"values:\n"
				"  '%s' in defaults file\n"
				"  '%s' in SHOW VARIABLES\n",
				mysql_data_home, datadir_var);
		}
	}

	/* get some default values is they are missing from my.cnf */
	if (!check_if_param_set("datadir") && datadir_var && *datadir_var) {
		strmake(mysql_real_data_home, datadir_var, FN_REFLEN - 1);
		mysql_data_home= mysql_real_data_home;
	}

	if (!check_if_param_set("innodb_data_file_path")
	    && innodb_data_file_path_var && *innodb_data_file_path_var) {
		innobase_data_file_path = my_strdup(PSI_NOT_INSTRUMENTED,
			innodb_data_file_path_var, MYF(MY_FAE));
	}

	if (!check_if_param_set("innodb_data_home_dir")
	    && innodb_data_home_dir_var && *innodb_data_home_dir_var) {
		innobase_data_home_dir = my_strdup(PSI_NOT_INSTRUMENTED,
			innodb_data_home_dir_var, MYF(MY_FAE));
	}

	if (!check_if_param_set("innodb_log_group_home_dir")
	    && innodb_log_group_home_dir_var
	    && *innodb_log_group_home_dir_var) {
		srv_log_group_home_dir = my_strdup(PSI_NOT_INSTRUMENTED,
			innodb_log_group_home_dir_var, MYF(MY_FAE));
	}

	if (!check_if_param_set("innodb_undo_directory")
	    && innodb_undo_directory_var && *innodb_undo_directory_var) {
		srv_undo_dir = my_strdup(PSI_NOT_INSTRUMENTED,
			innodb_undo_directory_var, MYF(MY_FAE));
	}

	if (!check_if_param_set("innodb_log_files_in_group")
	    && innodb_log_files_in_group_var) {
		char *endptr;

		innobase_log_files_in_group = strtol(
			innodb_log_files_in_group_var, &endptr, 10);
		ut_ad(*endptr == 0);
	}

	if (!check_if_param_set("innodb_log_file_size")
	    && innodb_log_file_size_var) {
		char *endptr;

		innobase_log_file_size = strtoll(
			innodb_log_file_size_var, &endptr, 10);
		ut_ad(*endptr == 0);
	}

	if (!check_if_param_set("innodb_page_size") && innodb_page_size_var) {
		char *endptr;

		innobase_page_size = strtoll(
			innodb_page_size_var, &endptr, 10);
		ut_ad(*endptr == 0);
	}

	if (!innodb_log_checksum_algorithm_specified &&
		innodb_log_checksum_algorithm_var) {
		for (uint i = 0;
		     i < innodb_checksum_algorithm_typelib.count;
		     i++) {
			if (strcasecmp(innodb_log_checksum_algorithm_var,
			    innodb_checksum_algorithm_typelib.type_names[i])
			    == 0) {
				srv_log_checksum_algorithm = i;
			}
		}
	}

	if (!innodb_checksum_algorithm_specified &&
		innodb_checksum_algorithm_var) {
		for (uint i = 0;
		     i < innodb_checksum_algorithm_typelib.count;
		     i++) {
			if (strcasecmp(innodb_checksum_algorithm_var,
			    innodb_checksum_algorithm_typelib.type_names[i])
			    == 0) {
				srv_checksum_algorithm = i;
			}
		}
	}

	if (!innodb_log_checksum_algorithm_specified &&
		innodb_log_checksums_var) {
		if (strcasecmp(innodb_log_checksums_var, "ON") == 0) {
			srv_log_checksum_algorithm =
				SRV_CHECKSUM_ALGORITHM_STRICT_CRC32;
		} else {
			srv_log_checksum_algorithm =
				SRV_CHECKSUM_ALGORITHM_NONE;
		}
	}

	memset(server_uuid, 0, ENCRYPTION_SERVER_UUID_LEN + 1);
	if (server_uuid_var != NULL) {
		strncpy(server_uuid, server_uuid_var,
			ENCRYPTION_SERVER_UUID_LEN);
	}

	if (innodb_track_changed_pages_var != NULL &&
	    strcasecmp(innodb_track_changed_pages_var, "ON") == 0) {
	  have_changed_page_bitmaps = true;
	}

out:
	free_mysql_variables(mysql_vars);

	return(ret);
}