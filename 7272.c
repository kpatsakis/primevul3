int mz_deflateReset(mz_streamp pStream) {
  if ((!pStream) || (!pStream->state) || (!pStream->zalloc) ||
      (!pStream->zfree))
    return MZ_STREAM_ERROR;
  pStream->total_in = pStream->total_out = 0;
  tdefl_init((tdefl_compressor *)pStream->state, NULL, NULL,
             ((tdefl_compressor *)pStream->state)->m_flags);
  return MZ_OK;
}