evutil_closesocket(evutil_socket_t sock)
{
#ifndef _WIN32
	return close(sock);
#else
	return closesocket(sock);
#endif
}
