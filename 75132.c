inline bool IsHexDigit(unsigned char c) {
  return (('0' <= c && c <= '9') || ('A' <= c && c <= 'F') ||
          ('a' <= c && c <= 'f'));
}
