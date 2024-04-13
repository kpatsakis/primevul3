std::string NetAddressToString(const struct addrinfo* net_address) {
#if defined(OS_WIN)
  EnsureWinsockInit();
#endif

  char buffer[INET6_ADDRSTRLEN];

  int result = getnameinfo(net_address->ai_addr,
      net_address->ai_addrlen, buffer, sizeof(buffer), NULL, 0, NI_NUMERICHOST);

  if (result != 0) {
    DLOG(INFO) << "getnameinfo() failed with " << result;
    buffer[0] = '\0';
  }
  return std::string(buffer);
}
