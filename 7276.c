static unsigned int readUInt(const char buf[4]) {
  const unsigned char *b = (const unsigned char *)buf;

  return (b[0] & 0x000000ff) | ((b[1] << 8) & 0x0000ff00) |
         ((b[2] << 16) & 0x00ff0000) | ((b[3] << 24) & 0xff000000);
}