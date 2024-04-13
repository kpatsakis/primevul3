static void cpy8(tinyexr::tinyexr_uint64 *dst_val, const tinyexr::tinyexr_uint64 *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  dst[4] = src[4];
  dst[5] = src[5];
  dst[6] = src[6];
  dst[7] = src[7];
}