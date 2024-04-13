std::string GetHostName() {
#if defined(OS_WIN)
  EnsureWinsockInit();
#endif

  char buffer[256];
  int result = gethostname(buffer, sizeof(buffer));
  if (result != 0) {
    DLOG(INFO) << "gethostname() failed with " << result;
    buffer[0] = '\0';
  }
  return std::string(buffer);
}
