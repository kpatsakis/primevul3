void *tdefl_write_image_to_png_file_in_memory_ex(const void *pImage, int w,
                                                 int h, int num_chans,
                                                 size_t *pLen_out,
                                                 mz_uint level, mz_bool flip) {
  // Using a local copy of this array here in case MINIZ_NO_ZLIB_APIS was
  // defined.
  static const mz_uint s_tdefl_png_num_probes[11] = {
      0, 1, 6, 32, 16, 32, 128, 256, 512, 768, 1500};
  tdefl_compressor *pComp =
      (tdefl_compressor *)MZ_MALLOC(sizeof(tdefl_compressor));
  tdefl_output_buffer out_buf;
  int i, bpl = w * num_chans, y, z;
  mz_uint32 c;
  *pLen_out = 0;
  if (!pComp) return NULL;
  MZ_CLEAR_OBJ(out_buf);
  out_buf.m_expandable = MZ_TRUE;
  out_buf.m_capacity = 57 + MZ_MAX(64, (1 + bpl) * h);
  if (NULL == (out_buf.m_pBuf = (mz_uint8 *)MZ_MALLOC(out_buf.m_capacity))) {
    MZ_FREE(pComp);
    return NULL;
  }
  // write dummy header
  for (z = 41; z; --z) tdefl_output_buffer_putter(&z, 1, &out_buf);
  // compress image data
  tdefl_init(
      pComp, tdefl_output_buffer_putter, &out_buf,
      s_tdefl_png_num_probes[MZ_MIN(10, level)] | TDEFL_WRITE_ZLIB_HEADER);
  for (y = 0; y < h; ++y) {
    tdefl_compress_buffer(pComp, &z, 1, TDEFL_NO_FLUSH);
    tdefl_compress_buffer(pComp,
                          (mz_uint8 *)pImage + (flip ? (h - 1 - y) : y) * bpl,
                          bpl, TDEFL_NO_FLUSH);
  }
  if (tdefl_compress_buffer(pComp, NULL, 0, TDEFL_FINISH) !=
      TDEFL_STATUS_DONE) {
    MZ_FREE(pComp);
    MZ_FREE(out_buf.m_pBuf);
    return NULL;
  }
  // write real header
  *pLen_out = out_buf.m_size - 41;
  {
    static const mz_uint8 chans[] = {0x00, 0x00, 0x04, 0x02, 0x06};
    mz_uint8 pnghdr[41] = {0x89,
                           0x50,
                           0x4e,
                           0x47,
                           0x0d,
                           0x0a,
                           0x1a,
                           0x0a,
                           0x00,
                           0x00,
                           0x00,
                           0x0d,
                           0x49,
                           0x48,
                           0x44,
                           0x52,
                           0,
                           0,
                           (mz_uint8)(w >> 8),
                           (mz_uint8)w,
                           0,
                           0,
                           (mz_uint8)(h >> 8),
                           (mz_uint8)h,
                           8,
                           chans[num_chans],
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           (mz_uint8)(*pLen_out >> 24),
                           (mz_uint8)(*pLen_out >> 16),
                           (mz_uint8)(*pLen_out >> 8),
                           (mz_uint8)*pLen_out,
                           0x49,
                           0x44,
                           0x41,
                           0x54};
    c = (mz_uint32)mz_crc32(MZ_CRC32_INIT, pnghdr + 12, 17);
    for (i = 0; i < 4; ++i, c <<= 8)
      ((mz_uint8 *)(pnghdr + 29))[i] = (mz_uint8)(c >> 24);
    memcpy(out_buf.m_pBuf, pnghdr, 41);
  }
  // write footer (IDAT CRC-32, followed by IEND chunk)
  if (!tdefl_output_buffer_putter(
          "\0\0\0\0\0\0\0\0\x49\x45\x4e\x44\xae\x42\x60\x82", 16, &out_buf)) {
    *pLen_out = 0;
    MZ_FREE(pComp);
    MZ_FREE(out_buf.m_pBuf);
    return NULL;
  }
  c = (mz_uint32)mz_crc32(MZ_CRC32_INIT, out_buf.m_pBuf + 41 - 4,
                          *pLen_out + 4);
  for (i = 0; i < 4; ++i, c <<= 8)
    (out_buf.m_pBuf + out_buf.m_size - 16)[i] = (mz_uint8)(c >> 24);
  // compute final size of file, grab compressed data buffer and return
  *pLen_out += 57;
  MZ_FREE(pComp);
  return out_buf.m_pBuf;
}