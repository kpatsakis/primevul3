static void mz_write_le32(mz_uint8 *p, mz_uint32 v) {
  p[0] = (mz_uint8)v;
  p[1] = (mz_uint8)(v >> 8);
  p[2] = (mz_uint8)(v >> 16);
  p[3] = (mz_uint8)(v >> 24);
}