backup_cleanup()
{
	free(mysql_binlog_position);
	free(buffer_pool_filename);
	free(backup_uuid);
	backup_uuid = NULL;

	if (mysql_connection) {
		mysql_close(mysql_connection);
	}
}