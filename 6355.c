parse_show_engine_innodb_status(MYSQL *connection)
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW row;

	mysql_result = xb_mysql_query(connection, "SHOW ENGINE INNODB STATUS",
				      true);

	ut_ad(mysql_num_fields(mysql_result) == 3);

	if ((row = mysql_fetch_row(mysql_result))) {
		std::stringstream data(row[2]);
		std::string line;

		while (std::getline(data, line)) {
			lsn_t lsn;
			if (sscanf(line.c_str(), "Log flushed up to " LSN_PF,
				   &lsn) == 1) {
				backup_redo_log_flushed_lsn = lsn;
			}
		}
	}

	mysql_free_result(mysql_result);
}