int mz_deflateInit2(mz_streamp pStream, int level, int method, int window_bits,
                    int mem_level, int strategy) {
  tdefl_compressor *pComp;
  mz_uint comp_flags =
      TDEFL_COMPUTE_ADLER32 |
      tdefl_create_comp_flags_from_zip_params(level, window_bits, strategy);

  if (!pStream) return MZ_STREAM_ERROR;
  if ((method != MZ_DEFLATED) || ((mem_level < 1) || (mem_level > 9)) ||
      ((window_bits != MZ_DEFAULT_WINDOW_BITS) &&
       (-window_bits != MZ_DEFAULT_WINDOW_BITS)))
    return MZ_PARAM_ERROR;

  pStream->data_type = 0;
  pStream->adler = MZ_ADLER32_INIT;
  pStream->msg = NULL;
  pStream->reserved = 0;
  pStream->total_in = 0;
  pStream->total_out = 0;
  if (!pStream->zalloc) pStream->zalloc = def_alloc_func;
  if (!pStream->zfree) pStream->zfree = def_free_func;

  pComp = (tdefl_compressor *)pStream->zalloc(pStream->opaque, 1,
                                              sizeof(tdefl_compressor));
  if (!pComp) return MZ_MEM_ERROR;

  pStream->state = (struct mz_internal_state *)pComp;

  if (tdefl_init(pComp, NULL, NULL, comp_flags) != TDEFL_STATUS_OKAY) {
    mz_deflateEnd(pStream);
    return MZ_PARAM_ERROR;
  }

  return MZ_OK;
}