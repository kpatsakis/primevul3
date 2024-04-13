bool IPv6Supported() {
#if defined(OS_POSIX)
  int test_socket;

  test_socket = socket(AF_INET6, SOCK_STREAM, 0);
  if (test_socket == -1) {
    IPv6SupportResults(IPV6_CANNOT_CREATE_SOCKETS);
    return false;
  }

  close(test_socket);
  IPv6SupportResults(IPV6_CAN_CREATE_SOCKETS);
  return true;
#elif defined(OS_WIN)
  EnsureWinsockInit();
  SOCKET test_socket;

  test_socket = socket(AF_INET6, SOCK_STREAM, 0);
  if (test_socket == INVALID_SOCKET) {
    IPv6SupportResults(IPV6_CANNOT_CREATE_SOCKETS);
    return false;
  }

  closesocket(test_socket);
  IPv6SupportResults(IPV6_CAN_CREATE_SOCKETS);
  return true;
#else
  NOTIMPLEMENTED();
  return true;
#endif  // defined(various platforms)
}
