int connect_unix_socket(const char * sockpath)
{
	int s;
	struct sockaddr_un addr;

	s = socket(AF_UNIX, SOCK_STREAM, 0);
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sockpath, sizeof(addr.sun_path));
	if(connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
		fprintf(stderr, "connecting to %s : ", addr.sun_path);
		perror("connect");
		exit(1);
	}
	printf("Connected to %s\n", addr.sun_path);
	return s;
}
