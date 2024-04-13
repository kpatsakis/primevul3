mdl_lock_tables()
{
	msg_ts("Initializing MDL on all current tables.\n");
	MYSQL_RES *mysql_result = NULL;
	MYSQL_ROW row;
	mdl_con = xb_mysql_connect();
	if (mdl_con != NULL) {
		xb_mysql_query(mdl_con, "BEGIN", false, true);
		mysql_result = xb_mysql_query(mdl_con, "SELECT NAME, SPACE FROM "
									"INFORMATION_SCHEMA.INNODB_SYS_TABLES", true, true);
		while ((row = mysql_fetch_row(mysql_result))) {
			if (fil_is_user_tablespace_id(atoi(row[1]))) {
				char full_table_name[MAX_FULL_NAME_LEN + 1];
				innobase_format_name(full_table_name, sizeof(full_table_name), row[0]);
				if (is_fts_index(full_table_name)) {
					// We will eventually get to the row to lock the main table
					msg_ts("%s is a Full-Text Index. Skipping\n", full_table_name);
					continue;
				} else if (is_tmp_table(full_table_name)) {
					// We cannot run SELECT ... #sql-; Skipped to avoid invalid query.
					msg_ts("%s is a temporary table. Skipping\n", full_table_name);
					continue;
				}

				msg_ts("Locking MDL for %s\n", full_table_name);
				char *lock_query;
				xb_a(asprintf(&lock_query,
						"SELECT 1 FROM %s LIMIT 0", full_table_name));

				xb_mysql_query(mdl_con, lock_query, false, false);

				free(lock_query);
			}
		}
		mysql_free_result(mysql_result);
	}
}