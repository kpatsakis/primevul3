static void SetErrorMessage(const std::string &msg, const char **err) {
  if (err) {
#ifdef _WIN32
    (*err) = _strdup(msg.c_str());
#else
    (*err) = strdup(msg.c_str());
#endif
  }
}