write_xtrabackup_info(MYSQL *connection)
{
	MYSQL_STMT *stmt;
	MYSQL_BIND bind[19];
	const char *uuid = NULL;
	char *server_version = NULL;
	char* xtrabackup_info_data = NULL;
	int idx;
	my_bool null = TRUE;

	const char *ins_query = "insert into PERCONA_SCHEMA.xtrabackup_history("
		"uuid, name, tool_name, tool_command, tool_version, "
		"ibbackup_version, server_version, start_time, end_time, "
		"lock_time, binlog_pos, innodb_from_lsn, innodb_to_lsn, "
		"partial, incremental, format, compact, compressed, "
		"encrypted) "
		"values(?,?,?,?,?,?,?,from_unixtime(?),from_unixtime(?),"
		"?,?,?,?,?,?,?,?,?,?)";

	ut_ad((uint)xtrabackup_stream_fmt <
		array_elements(xb_stream_format_name));
	const char *stream_format_name =
		xb_stream_format_name[xtrabackup_stream_fmt];
	history_end_time = time(NULL);

	xtrabackup_info_data = get_xtrabackup_info(connection);
	if (!backup_file_printf(XTRABACKUP_INFO, "%s", xtrabackup_info_data)) {
		goto cleanup;
	}

	if (!opt_history) {
		goto cleanup;
	}

	uuid = get_backup_uuid(connection);
	server_version = read_mysql_one_value(connection, "SELECT VERSION()");

	xb_mysql_query(connection,
		"CREATE DATABASE IF NOT EXISTS PERCONA_SCHEMA", false);
	xb_mysql_query(connection,
		"CREATE TABLE IF NOT EXISTS PERCONA_SCHEMA.xtrabackup_history("
		"uuid VARCHAR(40) NOT NULL PRIMARY KEY,"
		"name VARCHAR(255) DEFAULT NULL,"
		"tool_name VARCHAR(255) DEFAULT NULL,"
		"tool_command TEXT DEFAULT NULL,"
		"tool_version VARCHAR(255) DEFAULT NULL,"
		"ibbackup_version VARCHAR(255) DEFAULT NULL,"
		"server_version VARCHAR(255) DEFAULT NULL,"
		"start_time TIMESTAMP NULL DEFAULT NULL,"
		"end_time TIMESTAMP NULL DEFAULT NULL,"
		"lock_time BIGINT UNSIGNED DEFAULT NULL,"
		"binlog_pos TEXT DEFAULT NULL,"
		"innodb_from_lsn BIGINT UNSIGNED DEFAULT NULL,"
		"innodb_to_lsn BIGINT UNSIGNED DEFAULT NULL,"
		"partial ENUM('Y', 'N') DEFAULT NULL,"
		"incremental ENUM('Y', 'N') DEFAULT NULL,"
		"format ENUM('file', 'tar', 'xbstream') DEFAULT NULL,"
		"compact ENUM('Y', 'N') DEFAULT NULL,"
		"compressed ENUM('Y', 'N') DEFAULT NULL,"
		"encrypted ENUM('Y', 'N') DEFAULT NULL"
		") CHARACTER SET utf8 ENGINE=innodb", false);

	/* Upgrade from previous versions */
	xb_mysql_query(connection,
		"ALTER TABLE PERCONA_SCHEMA.xtrabackup_history MODIFY COLUMN "
		"binlog_pos TEXT DEFAULT NULL", false);

	stmt = mysql_stmt_init(connection);

	mysql_stmt_prepare(stmt, ins_query, strlen(ins_query));

	memset(bind, 0, sizeof(bind));
	idx = 0;

	/* uuid */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)uuid;
	bind[idx].buffer_length = strlen(uuid);
	++idx;

	/* name */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(opt_history);
	bind[idx].buffer_length = strlen(opt_history);
	if (!(opt_history && *opt_history)) {
		bind[idx].is_null = &null;
	}
	++idx;

	/* tool_name */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = tool_name;
	bind[idx].buffer_length = strlen(tool_name);
	++idx;

	/* tool_command */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = tool_args;
	bind[idx].buffer_length = strlen(tool_args);
	++idx;

	/* tool_version */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(XTRABACKUP_VERSION);
	bind[idx].buffer_length = strlen(XTRABACKUP_VERSION);
	++idx;

	/* ibbackup_version */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(XTRABACKUP_VERSION);
	bind[idx].buffer_length = strlen(XTRABACKUP_VERSION);
	++idx;

	/* server_version */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = server_version;
	bind[idx].buffer_length = strlen(server_version);
	++idx;

	/* start_time */
	bind[idx].buffer_type = MYSQL_TYPE_LONG;
	bind[idx].buffer = &history_start_time;
	++idx;

	/* end_time */
	bind[idx].buffer_type = MYSQL_TYPE_LONG;
	bind[idx].buffer = &history_end_time;
	++idx;

	/* lock_time */
	bind[idx].buffer_type = MYSQL_TYPE_LONG;
	bind[idx].buffer = &history_lock_time;
	++idx;

	/* binlog_pos */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = mysql_binlog_position;
	if (mysql_binlog_position != NULL) {
		bind[idx].buffer_length = strlen(mysql_binlog_position);
	} else {
		bind[idx].is_null = &null;
	}
	++idx;

	/* innodb_from_lsn */
	bind[idx].buffer_type = MYSQL_TYPE_LONGLONG;
	bind[idx].buffer = (char*)(&incremental_lsn);
	++idx;

	/* innodb_to_lsn */
	bind[idx].buffer_type = MYSQL_TYPE_LONGLONG;
	bind[idx].buffer = (char*)(&metadata_to_lsn);
	++idx;

	/* partial (Y | N) */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)((xtrabackup_tables
				    || xtrabackup_tables_exclude
				    || xtrabackup_tables_file
				    || xtrabackup_databases
				    || xtrabackup_databases_exclude
				    || xtrabackup_databases_file) ? "Y" : "N");
	bind[idx].buffer_length = 1;
	++idx;

	/* incremental (Y | N) */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(
		(xtrabackup_incremental
		 || xtrabackup_incremental_basedir
		 || opt_incremental_history_name
		 || opt_incremental_history_uuid) ? "Y" : "N");
	bind[idx].buffer_length = 1;
	++idx;

	/* format (file | tar | xbstream) */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(stream_format_name);
	bind[idx].buffer_length = strlen(stream_format_name);
	++idx;

	/* compact (Y | N) */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(xtrabackup_compact ? "Y" : "N");
	bind[idx].buffer_length = 1;
	++idx;

	/* compressed (Y | N) */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(xtrabackup_compress ? "Y" : "N");
	bind[idx].buffer_length = 1;
	++idx;

	/* encrypted (Y | N) */
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = (char*)(xtrabackup_encrypt ? "Y" : "N");
	bind[idx].buffer_length = 1;
	++idx;

	ut_ad(idx == 19);

	mysql_stmt_bind_param(stmt, bind);

	mysql_stmt_execute(stmt);
	mysql_stmt_close(stmt);

cleanup:

	free(xtrabackup_info_data);
	free(server_version);

	return(true);
}