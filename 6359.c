has_innodb_buffer_pool_dump_pct()
{
	if ((server_flavor == FLAVOR_PERCONA_SERVER ||
		server_flavor == FLAVOR_MYSQL) &&
		mysql_server_version >= 50702) {
		return(true);
	}

	if (server_flavor == FLAVOR_MARIADB && mysql_server_version >= 10110) {
		return(true);
	}

	return(false);
}