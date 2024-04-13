static const char *ReadString(std::string *s, const char *ptr, size_t len) {
  // Read untile NULL(\0).
  const char *p = ptr;
  const char *q = ptr;
  while ((size_t(q - ptr) < len) && (*q) != 0) {
    q++;
  }

  if (size_t(q - ptr) >= len) {
    (*s) = std::string();
    return NULL;
  }

  (*s) = std::string(p, q);

  return q + 1;  // skip '\0'
}