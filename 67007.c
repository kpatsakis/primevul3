char *flash_strncpy(char *dst, const char *src, size_t c) {
  char *d = dst;
  uint32_t *s = (uint32_t *)src;
  size_t slen = flash_strlen(src);
  size_t len = slen > c ? c : slen;

  while (len >= 4) {
    uint32_t w = *s++;
    *d++ = w & 0xff; w >>= 8;
    *d++ = w & 0xff; w >>= 8;
    *d++ = w & 0xff; w >>= 8;
    *d++ = w & 0xff;
    len -= 4;
  }
  if (len > 0) {
    uint32_t w = *s++;
    while (len-- > 0) {
      *d++ = w & 0xff; w >>= 8;
    }
  }
  if (slen < c) *d = 0;
  return dst;
}
