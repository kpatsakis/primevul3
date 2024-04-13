size_t flash_strlen(const char *str) {
  size_t len = 0;
  uint32_t *s = (uint32_t *)str;

  while (1) {
    uint32_t w = *s++;
    if ((w & 0xff) == 0) break;
    len++; w >>= 8;
    if ((w & 0xff) == 0) break;
    len++; w >>= 8;
    if ((w & 0xff) == 0) break;
    len++; w >>= 8;
    if ((w & 0xff) == 0) break;
    len++;
  }
  return len;
}
