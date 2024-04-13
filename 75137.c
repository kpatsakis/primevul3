bool IsPortAllowedByFtp(int port) {
  int array_size = arraysize(kAllowedFtpPorts);
  for (int i = 0; i < array_size; i++) {
    if (kAllowedFtpPorts[i] == port) {
        return true;
    }
  }
  return IsPortAllowedByDefault(port);
}
