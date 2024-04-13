int socket_send(int fd, void *data, size_t length)
{
	int flags = 0;
#ifdef MSG_NOSIGNAL
	flags |= MSG_NOSIGNAL;
#endif
	return send(fd, data, length, flags);
}
