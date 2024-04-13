static bool send_client_authreq(PgSocket *client)
{
	uint8_t saltlen = 0;
	int res;
	int auth_type = client->client_auth_type;

	if (auth_type == AUTH_MD5) {
		saltlen = 4;
		get_random_bytes((void*)client->tmp_login_salt, saltlen);
	} else if (auth_type == AUTH_PLAIN) {
		/* nothing to do */
	} else {
		return false;
	}

	SEND_generic(res, client, 'R', "ib", auth_type, client->tmp_login_salt, saltlen);
	if (!res)
		disconnect_client(client, false, "failed to send auth req");
	return res;
}
