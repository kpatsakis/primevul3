evutil_socket_finished_connecting_(evutil_socket_t fd)
{
	int e;
	ev_socklen_t elen = sizeof(e);

	if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)&e, &elen) < 0)
		return -1;

	if (e) {
		if (EVUTIL_ERR_CONNECT_RETRIABLE(e))
			return 0;
		EVUTIL_SET_SOCKET_ERROR(e);
		return -1;
	}

	return 1;
}
