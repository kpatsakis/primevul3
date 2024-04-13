void *tdefl_compress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len,
                                 size_t *pOut_len, int flags) {
  tdefl_output_buffer out_buf;
  MZ_CLEAR_OBJ(out_buf);
  if (!pOut_len)
    return MZ_FALSE;
  else
    *pOut_len = 0;
  out_buf.m_expandable = MZ_TRUE;
  if (!tdefl_compress_mem_to_output(
          pSrc_buf, src_buf_len, tdefl_output_buffer_putter, &out_buf, flags))
    return NULL;
  *pOut_len = out_buf.m_size;
  return out_buf.m_pBuf;
}