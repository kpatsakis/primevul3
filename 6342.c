write_slave_info(MYSQL *connection)
{
	char *master = NULL;
	char *filename = NULL;
	char *gtid_executed = NULL;
	char *position = NULL;
	char *gtid_slave_pos = NULL;
	char *auto_position = NULL;
	char *using_gtid = NULL;
	char *slave_sql_running = NULL;

	char *ptr = NULL;
	char *writable_channel_name = NULL;
	const char* channel_info = NULL;
	const size_t channel_info_maxlen = 128;
	bool result = true;
	char channel_info_buf[channel_info_maxlen];
	std::string slave_info;

	mysql_variable status[] = {
		{"Master_Host", &master},
		{"Relay_Master_Log_File", &filename},
		{"Exec_Master_Log_Pos", &position},
		{"Executed_Gtid_Set", &gtid_executed},
		{"Channel_Name", &writable_channel_name},
		{"Auto_Position", &auto_position},
		{"Using_Gtid", &using_gtid},
		{"Slave_SQL_Running", &slave_sql_running},
		{NULL, NULL}
	};

	mysql_variable variables[] = {
		{"gtid_slave_pos", &gtid_slave_pos},
		{NULL, NULL}
	};
	read_mysql_variables(connection, "SHOW VARIABLES", variables, true);

	MYSQL_RES* slave_status_res = xb_mysql_query(connection,
		"SHOW SLAVE STATUS", true, true);
	int master_index = 0;
	while (read_mysql_variables_from_result(slave_status_res, status,
		false)) {
		if (master == NULL || filename == NULL || position == NULL) {
			msg("Failed to get master binlog coordinates "
				"from SHOW SLAVE STATUS\n");
			msg("This means that the server is not a "
				"replication slave. Ignoring the --slave-info "
				"option\n");
			/* we still want to continue the backup */
			goto cleanup;
		}

		const char* channel_name = writable_channel_name;
		if (channel_name && channel_name[0] != '\0') {
			const char* clause_format = " FOR CHANNEL '%s'";
			xb_a(channel_info_maxlen >
				strlen(channel_name) + strlen(clause_format));
			snprintf(channel_info_buf, channel_info_maxlen,
				clause_format, channel_name);
			channel_info = channel_info_buf;
		} else {
			channel_name = channel_info = "";
		}

		ut_ad(!have_multi_threaded_slave || have_gtid_slave ||
			strcasecmp(slave_sql_running, "No") == 0);

		if (slave_info.capacity() == 0) {
			slave_info.reserve(4096);
		}

		++master_index;

		/* Print slave status to a file.
		If GTID mode is used, construct a CHANGE MASTER statement with
		MASTER_AUTO_POSITION and correct a gtid_purged value. */
		if (auto_position != NULL && !strcmp(auto_position, "1")) {
			/* MySQL >= 5.6 with GTID enabled */
			for (ptr = strchr(gtid_executed, '\n');
			     ptr;
			     ptr = strchr(ptr, '\n')) {
				*ptr = ' ';
			}

			if (master_index == 1) {
				format_append(slave_info,
					"SET GLOBAL gtid_purged='%s';\n",
					gtid_executed);
			}
			format_append(slave_info,
				"CHANGE MASTER TO MASTER_AUTO_POSITION=1%s;\n",
				channel_info);

			format_append(mysql_slave_position,
				"master host '%s', purge list '%s', "
				"channel name: '%s'\n",
				master, gtid_executed, channel_name);
		} else if (using_gtid && !strcasecmp(using_gtid, "yes")) {
			/* MariaDB >= 10.0 with GTID enabled */
			if (master_index == 1) {
				format_append(slave_info,
					"SET GLOBAL gtid_slave_pos = '%s';\n",
					gtid_slave_pos);
			}
			format_append(slave_info,
				"CHANGE MASTER TO master_use_gtid = slave_pos"
				"%s;\n",
				channel_info);
			format_append(mysql_slave_position,
				"master host '%s', gtid_slave_pos %s, "
				"channel name: '%s'\n",
				master, gtid_slave_pos, channel_name);
		} else {
			format_append(slave_info,
				"CHANGE MASTER TO MASTER_LOG_FILE='%s', "
				"MASTER_LOG_POS=%s%s;\n",
				filename, position, channel_info);
			format_append(mysql_slave_position,
				"master host '%s', filename '%s', "
				"position '%s', channel name: '%s'\n",
				master, filename, position,
				channel_name);
		}
		free_mysql_variables(status);
	}
	result = backup_file_print(XTRABACKUP_SLAVE_INFO, slave_info.c_str(),
			  slave_info.size());

cleanup:
	mysql_free_result(slave_status_res);
	free_mysql_variables(status);
	free_mysql_variables(variables);

	return(result);
}