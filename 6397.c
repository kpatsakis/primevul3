connect_to_upstream_proxy(struct conn_s *connptr, struct request_s *request)
{
	unsigned len;
	unsigned char buff[512]; /* won't use more than 7 + 255 */
	unsigned short port;
	size_t ulen, passlen;

	struct upstream *cur_upstream = connptr->upstream_proxy;

	ulen = cur_upstream->ua.user ? strlen(cur_upstream->ua.user) : 0;
	passlen = cur_upstream->pass ? strlen(cur_upstream->pass) : 0;


	log_message(LOG_CONN,
		    "Established connection to %s proxy \"%s\" using file descriptor %d.",
		    proxy_type_name(cur_upstream->type), cur_upstream->host, connptr->server_fd);

	if (cur_upstream->type == PT_SOCKS4) {

		buff[0] = 4; /* socks version */
		buff[1] = 1; /* connect command */
		port = htons(request->port);
		memcpy(&buff[2], &port, 2); /* dest port */
		memcpy(&buff[4], "\0\0\0\1" /* socks4a fake ip */
				 "\0" /* user */, 5);
		len = strlen(request->host);
		if(len>255)
			return -1;
		memcpy(&buff[9], request->host, len+1);
		if (9+len+1 != safe_write(connptr->server_fd, buff, 9+len+1))
			return -1;
		if (8 != safe_read(connptr->server_fd, buff, 8))
			return -1;
		if (buff[0]!=0 || buff[1]!=90)
			return -1;

	} else if (cur_upstream->type == PT_SOCKS5) {

		/* init */
		int n_methods = ulen ? 2 : 1;
		buff[0] = 5; /* socks version */
		buff[1] = n_methods; /* number of methods  */
		buff[2] = 0; /* no auth method */
		if (ulen) buff[3] = 2;  /* auth method -> username / password */
		if (2+n_methods != safe_write(connptr->server_fd, buff, 2+n_methods))
			return -1;
		if (2 != safe_read(connptr->server_fd, buff, 2))
			return -1;
		if (buff[0] != 5 || (buff[1] != 0 && buff[1] != 2))
			return -1;

		if (buff[1] == 2) {
			/* authentication */
			char in[2];
			char out[515];
			char *cur = out;
			size_t c;
			*cur++ = 1;	/* version */
			c = ulen & 0xFF;
			*cur++ = c;
			memcpy(cur, cur_upstream->ua.user, c);
			cur += c;
			c = passlen & 0xFF;
			*cur++ = c;
			memcpy(cur, cur_upstream->pass, c);
			cur += c;

			if((cur - out) != safe_write(connptr->server_fd, out, cur - out))
				return -1;

			if(2 != safe_read(connptr->server_fd, in, 2))
				return -1;
			if(in[1] != 0 || !(in[0] == 5 || in[0] == 1)) {
				return -1;
			}
		}
		/* connect */
		buff[0] = 5; /* socks version */
		buff[1] = 1; /* connect */
		buff[2] = 0; /* reserved */
		buff[3] = 3; /* domainname */
		len=strlen(request->host);
		if(len>255)
			return -1;
		buff[4] = len; /* length of domainname */
		memcpy(&buff[5], request->host, len); /* dest ip */
		port = htons(request->port);
		memcpy(&buff[5+len], &port, 2); /* dest port */
		if (7+len != safe_write(connptr->server_fd, buff, 7+len))
			return -1;
		if (4 != safe_read(connptr->server_fd, buff, 4))
			return -1;
		if (buff[0]!=5 || buff[1]!=0)
			return -1;
		switch(buff[3]) {
			case 1: len=4; break; /* ip v4 */
			case 4: len=16; break; /* ip v6 */
			case 3: /* domainname */
				if (1 != safe_read(connptr->server_fd, buff, 1))
					return -1;
				len = buff[0]; /* max = 255 */
				break;
			default: return -1;
		}
		if (2+len != safe_read(connptr->server_fd, buff, 2+len))
			return -1;
	} else {
		return -1;
	}

	if (connptr->connect_method)
		return 0;

	return establish_http_connection(connptr, request);
}