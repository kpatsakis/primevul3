mz_bool tdefl_compress_mem_to_output(const void *pBuf, size_t buf_len,
                                     tdefl_put_buf_func_ptr pPut_buf_func,
                                     void *pPut_buf_user, int flags) {
  tdefl_compressor *pComp;
  mz_bool succeeded;
  if (((buf_len) && (!pBuf)) || (!pPut_buf_func)) return MZ_FALSE;
  pComp = (tdefl_compressor *)MZ_MALLOC(sizeof(tdefl_compressor));
  if (!pComp) return MZ_FALSE;
  succeeded = (tdefl_init(pComp, pPut_buf_func, pPut_buf_user, flags) ==
               TDEFL_STATUS_OKAY);
  succeeded =
      succeeded && (tdefl_compress_buffer(pComp, pBuf, buf_len, TDEFL_FINISH) ==
                    TDEFL_STATUS_DONE);
  MZ_FREE(pComp);
  return succeeded;
}