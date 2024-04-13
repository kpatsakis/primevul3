static void swap2(unsigned short *val) {
#ifdef MINIZ_LITTLE_ENDIAN
  (void)val;
#else
  unsigned short tmp = *val;
  unsigned char *dst = reinterpret_cast<unsigned char *>(val);
  unsigned char *src = reinterpret_cast<unsigned char *>(&tmp);

  dst[0] = src[1];
  dst[1] = src[0];
#endif
}