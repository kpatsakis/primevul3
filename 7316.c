tdefl_status tdefl_compress(tdefl_compressor *d, const void *pIn_buf,
                            size_t *pIn_buf_size, void *pOut_buf,
                            size_t *pOut_buf_size, tdefl_flush flush) {
  if (!d) {
    if (pIn_buf_size) *pIn_buf_size = 0;
    if (pOut_buf_size) *pOut_buf_size = 0;
    return TDEFL_STATUS_BAD_PARAM;
  }

  d->m_pIn_buf = pIn_buf;
  d->m_pIn_buf_size = pIn_buf_size;
  d->m_pOut_buf = pOut_buf;
  d->m_pOut_buf_size = pOut_buf_size;
  d->m_pSrc = (const mz_uint8 *)(pIn_buf);
  d->m_src_buf_left = pIn_buf_size ? *pIn_buf_size : 0;
  d->m_out_buf_ofs = 0;
  d->m_flush = flush;

  if (((d->m_pPut_buf_func != NULL) ==
       ((pOut_buf != NULL) || (pOut_buf_size != NULL))) ||
      (d->m_prev_return_status != TDEFL_STATUS_OKAY) ||
      (d->m_wants_to_finish && (flush != TDEFL_FINISH)) ||
      (pIn_buf_size && *pIn_buf_size && !pIn_buf) ||
      (pOut_buf_size && *pOut_buf_size && !pOut_buf)) {
    if (pIn_buf_size) *pIn_buf_size = 0;
    if (pOut_buf_size) *pOut_buf_size = 0;
    return (d->m_prev_return_status = TDEFL_STATUS_BAD_PARAM);
  }
  d->m_wants_to_finish |= (flush == TDEFL_FINISH);

  if ((d->m_output_flush_remaining) || (d->m_finished))
    return (d->m_prev_return_status = tdefl_flush_output_buffer(d));

#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN
  if (((d->m_flags & TDEFL_MAX_PROBES_MASK) == 1) &&
      ((d->m_flags & TDEFL_GREEDY_PARSING_FLAG) != 0) &&
      ((d->m_flags & (TDEFL_FILTER_MATCHES | TDEFL_FORCE_ALL_RAW_BLOCKS |
                      TDEFL_RLE_MATCHES)) == 0)) {
    if (!tdefl_compress_fast(d)) return d->m_prev_return_status;
  } else
#endif  // #if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN
  {
    if (!tdefl_compress_normal(d)) return d->m_prev_return_status;
  }

  if ((d->m_flags & (TDEFL_WRITE_ZLIB_HEADER | TDEFL_COMPUTE_ADLER32)) &&
      (pIn_buf))
    d->m_adler32 =
        (mz_uint32)mz_adler32(d->m_adler32, (const mz_uint8 *)pIn_buf,
                              d->m_pSrc - (const mz_uint8 *)pIn_buf);

  if ((flush) && (!d->m_lookahead_size) && (!d->m_src_buf_left) &&
      (!d->m_output_flush_remaining)) {
    if (tdefl_flush_block(d, flush) < 0) return d->m_prev_return_status;
    d->m_finished = (flush == TDEFL_FINISH);
    if (flush == TDEFL_FULL_FLUSH) {
      MZ_CLEAR_OBJ(d->m_hash);
      MZ_CLEAR_OBJ(d->m_next);
      d->m_dict_size = 0;
    }
  }

  return (d->m_prev_return_status = tdefl_flush_output_buffer(d));
}