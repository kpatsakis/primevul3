unlock_all(MYSQL *connection)
{
	if (binlog_locked) {
		msg_ts("Executing UNLOCK BINLOG\n");
		xb_mysql_query(connection, "UNLOCK BINLOG", false);
	}

	msg_ts("Executing UNLOCK TABLES\n");
	xb_mysql_query(connection, "UNLOCK TABLES", false);

	msg_ts("All tables unlocked\n");
}