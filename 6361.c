get_open_temp_tables(MYSQL *connection)
{
	char *slave_open_temp_tables = NULL;
	mysql_variable status[] = {
		{"Slave_open_temp_tables", &slave_open_temp_tables},
		{NULL, NULL}
	};
	int result = false;

	read_mysql_variables(connection,
		"SHOW STATUS LIKE 'slave_open_temp_tables'", status, true);

	result = slave_open_temp_tables ? atoi(slave_open_temp_tables) : 0;

	free_mysql_variables(status);

	return(result);
}