tdefl_status tdefl_compress_buffer(tdefl_compressor *d, const void *pIn_buf,
                                   size_t in_buf_size, tdefl_flush flush) {
  MZ_ASSERT(d->m_pPut_buf_func);
  return tdefl_compress(d, pIn_buf, &in_buf_size, NULL, NULL, flush);
}