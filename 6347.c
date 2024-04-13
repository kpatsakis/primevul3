has_innodb_buffer_pool_dump()
{
	if ((server_flavor == FLAVOR_PERCONA_SERVER ||
		server_flavor == FLAVOR_MYSQL) &&
		mysql_server_version >= 50603) {
		return(true);
	}

	if (server_flavor == FLAVOR_MARIADB && mysql_server_version >= 10000) {
		return(true);
	}

	msg_ts("Server has no support for innodb_buffer_pool_dump_now");
	return(false);
}