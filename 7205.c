static void cpy2(unsigned short *dst_val, const unsigned short *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
}