size_t tinfl_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len,
                                   const void *pSrc_buf, size_t src_buf_len,
                                   int flags) {
  tinfl_decompressor decomp;
  tinfl_status status;
  tinfl_init(&decomp);
  status =
      tinfl_decompress(&decomp, (const mz_uint8 *)pSrc_buf, &src_buf_len,
                       (mz_uint8 *)pOut_buf, (mz_uint8 *)pOut_buf, &out_buf_len,
                       (flags & ~TINFL_FLAG_HAS_MORE_INPUT) |
                           TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
  return (status != TINFL_STATUS_DONE) ? TINFL_DECOMPRESS_MEM_TO_MEM_FAILED
                                       : out_buf_len;
}