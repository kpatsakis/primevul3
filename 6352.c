check_server_version(unsigned long version_number,
		     const char *version_string,
		     const char *version_comment,
		     const char *innodb_version)
{
	bool version_supported = false;
	bool mysql51 = false;

	mysql_server_version = version_number;

	server_flavor = FLAVOR_UNKNOWN;
	if (strstr(version_comment, "Percona") != NULL) {
		server_flavor = FLAVOR_PERCONA_SERVER;
	} else if (strstr(version_comment, "MariaDB") != NULL ||
		   strstr(version_string, "MariaDB") != NULL) {
		server_flavor = FLAVOR_MARIADB;
	} else if (strstr(version_comment, "MySQL") != NULL) {
		server_flavor = FLAVOR_MYSQL;
	}

	mysql51 = version_number > 50100 && version_number < 50500;
	version_supported = version_supported
		|| (mysql51 && innodb_version != NULL);
	version_supported = version_supported
		|| (version_number > 50500 && version_number < 50800);
	version_supported = version_supported
		|| ((version_number > 100000 && version_number < 100300)
		    && server_flavor == FLAVOR_MARIADB);

	if (mysql51 && innodb_version == NULL) {
		msg("Error: Built-in InnoDB in MySQL 5.1 is not "
		    "supported in this release. You can either use "
		    "Percona XtraBackup 2.0, or upgrade to InnoDB "
		    "plugin.\n");
	} else if (version_number > 80000 && version_number < 90000) {
		msg("Error: MySQL 8.0 and Percona Server 8.0 are not "
		    "supported by Percona Xtrabackup 2.4.x series. "
		    "Please use Percona Xtrabackup 8.0.x for backups "
		    "and restores.\n");
	} else if (!version_supported) {
		msg("Error: Unsupported server version: '%s'. Please "
		    "report a bug at "
		    "https://jira.percona.com/projects/PXB\n",
		    version_string);
	}

	return(version_supported);
}