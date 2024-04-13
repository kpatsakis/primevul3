xb_mysql_connect()
{
	MYSQL *connection = mysql_init(NULL);
	char mysql_port_str[std::numeric_limits<int>::digits10 + 3];

	sprintf(mysql_port_str, "%d", opt_port);

	if (connection == NULL) {
		msg("Failed to init MySQL struct: %s.\n",
			mysql_error(connection));
		return(NULL);
	}

	msg_ts("Connecting to MySQL server host: %s, user: %s, password: %s, "
	       "port: %s, socket: %s\n", opt_host ? opt_host : "localhost",
	       opt_user ? opt_user : "not set",
	       opt_password ? "set" : "not set",
	       opt_port != 0 ? mysql_port_str : "not set",
	       opt_socket ? opt_socket : "not set");

#ifdef HAVE_OPENSSL
	/*
	Print a warning if explicitly defined combination of --ssl-mode other than
	VERIFY_CA or VERIFY_IDENTITY with explicit --ssl-ca or --ssl-capath values.
	*/
	if (ssl_mode_set_explicitly &&
	    opt_ssl_mode < SSL_MODE_VERIFY_CA &&
	    (opt_ssl_ca || opt_ssl_capath))
	{
		msg("WARNING: no verification of server certificate will "
		       "be done. Use --ssl-mode=VERIFY_CA or "
		       "VERIFY_IDENTITY.\n");
	}

	/* Set SSL parameters: key, cert, ca, capath, cipher, clr, clrpath. */
	if (opt_ssl_mode >= SSL_MODE_VERIFY_CA)
		mysql_ssl_set(connection, opt_ssl_key, opt_ssl_cert, opt_ssl_ca,
			      opt_ssl_capath, opt_ssl_cipher);
	else
		mysql_ssl_set(connection, opt_ssl_key, opt_ssl_cert, NULL,
			      NULL, opt_ssl_cipher);
	mysql_options(connection, MYSQL_OPT_SSL_CRL, opt_ssl_crl);
	mysql_options(connection, MYSQL_OPT_SSL_CRLPATH, opt_ssl_crlpath);
	mysql_options(connection, MYSQL_OPT_TLS_VERSION, opt_tls_version);
	mysql_options(connection, MYSQL_OPT_SSL_MODE, &opt_ssl_mode);
#endif

	if (!mysql_real_connect(connection,
				opt_host ? opt_host : "localhost",
				opt_user,
				opt_password,
				"" /*database*/, opt_port,
				opt_socket, 0)) {
		msg("Failed to connect to MySQL server: %s.\n",
			mysql_error(connection));
		mysql_close(connection);
		return(NULL);
	}

	xb_mysql_query(connection, "SET SESSION wait_timeout=2147483",
		       false, true);

	xb_mysql_query(connection, "SET SESSION autocommit=1",
		       false, true);

	xb_mysql_query(connection, "SET NAMES utf8",
		       false, true);

	return(connection);
}