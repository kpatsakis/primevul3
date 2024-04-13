int socket_create_unix(const char *filename)
{
	struct sockaddr_un name;
	int sock;
	size_t size;
#ifdef SO_NOSIGPIPE
	int yes = 1;
#endif

	unlink(filename);

	/* Create the socket. */
	sock = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		return -1;
	}

#ifdef SO_NOSIGPIPE
	if (setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, (void*)&yes, sizeof(int)) == -1) {
		perror("setsockopt()");
		socket_close(sock);
		return -1;
	}
#endif

	/* Bind a name to the socket. */
	name.sun_family = AF_LOCAL;
	strncpy(name.sun_path, filename, sizeof(name.sun_path));
	name.sun_path[sizeof(name.sun_path) - 1] = '\0';

	/* The size of the address is
	   the offset of the start of the filename,
	   plus its length,
	   plus one for the terminating null byte.
	   Alternatively you can just do:
	   size = SUN_LEN (&name);
	 */
	size = (offsetof(struct sockaddr_un, sun_path)
			+ strlen(name.sun_path) + 1);

	if (bind(sock, (struct sockaddr *) &name, size) < 0) {
		perror("bind");
		socket_close(sock);
		return -1;
	}

	if (listen(sock, 10) < 0) {
		perror("listen");
		socket_close(sock);
		return -1;
	}

	return sock;
}
