static void writeUInt(char buf[4], unsigned int i) {
  unsigned char *b = (unsigned char *)buf;

  b[0] = i;
  b[1] = i >> 8;
  b[2] = i >> 16;
  b[3] = i >> 24;
}