static void mz_write_le16(mz_uint8 *p, mz_uint16 v) {
  p[0] = (mz_uint8)v;
  p[1] = (mz_uint8)(v >> 8);
}