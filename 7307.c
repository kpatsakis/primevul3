static void cpy4(int *dst_val, const int *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}