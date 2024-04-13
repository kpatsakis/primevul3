get_slave_coordinates(MYSQL *connection)
{
	char *relay_log_file = NULL;
	char *exec_log_pos = NULL;
	char *result = NULL;

	mysql_variable slave_coordinates[] = {
		{"Relay_Master_Log_File", &relay_log_file},
		{"Exec_Master_Log_Pos", &exec_log_pos},
		{NULL, NULL}
	};

	read_mysql_variables(connection, "SHOW SLAVE STATUS",
		slave_coordinates, false);
	ut_a(asprintf(&result, "%s\\%s", relay_log_file, exec_log_pos));
	free_mysql_variables(slave_coordinates);
	return result;
}