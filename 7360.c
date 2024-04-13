void *tinfl_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len,
                                   size_t *pOut_len, int flags) {
  tinfl_decompressor decomp;
  void *pBuf = NULL, *pNew_buf;
  size_t src_buf_ofs = 0, out_buf_capacity = 0;
  *pOut_len = 0;
  tinfl_init(&decomp);
  for (;;) {
    size_t src_buf_size = src_buf_len - src_buf_ofs,
           dst_buf_size = out_buf_capacity - *pOut_len, new_out_buf_capacity;
    tinfl_status status = tinfl_decompress(
        &decomp, (const mz_uint8 *)pSrc_buf + src_buf_ofs, &src_buf_size,
        (mz_uint8 *)pBuf, pBuf ? (mz_uint8 *)pBuf + *pOut_len : NULL,
        &dst_buf_size,
        (flags & ~TINFL_FLAG_HAS_MORE_INPUT) |
            TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
    if ((status < 0) || (status == TINFL_STATUS_NEEDS_MORE_INPUT)) {
      MZ_FREE(pBuf);
      *pOut_len = 0;
      return NULL;
    }
    src_buf_ofs += src_buf_size;
    *pOut_len += dst_buf_size;
    if (status == TINFL_STATUS_DONE) break;
    new_out_buf_capacity = out_buf_capacity * 2;
    if (new_out_buf_capacity < 128) new_out_buf_capacity = 128;
    pNew_buf = MZ_REALLOC(pBuf, new_out_buf_capacity);
    if (!pNew_buf) {
      MZ_FREE(pBuf);
      *pOut_len = 0;
      return NULL;
    }
    pBuf = pNew_buf;
    out_buf_capacity = new_out_buf_capacity;
  }
  return pBuf;
}