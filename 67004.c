unsigned char *flash_memcpy(unsigned char *dst, const unsigned char *src, size_t c) {
  unsigned char *d = dst;
  uint32_t *s = (uint32_t *)src;
  size_t len = c;

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
  return dst;
}
