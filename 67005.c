int flash_strcmp(const char *mem, const char *flash) {
  while (1) {
    char m = *mem++;
    char c = READ_FLASH_UINT8(flash++);
    if (m == 0) return c != 0 ? -1 : 0;
    if (c == 0) return 1;
    if (c > m) return -1;
    if (m > c) return 1;
  }
}
