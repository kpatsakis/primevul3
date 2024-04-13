evutil_socket_geterror(evutil_socket_t sock)
{
	int optval, optvallen=sizeof(optval);
	int err = WSAGetLastError();
	if (err == WSAEWOULDBLOCK && sock >= 0) {
		if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)&optval,
					   &optvallen))
			return err;
		if (optval)
			return optval;
	}
	return err;
}
