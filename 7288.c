static tdefl_status tdefl_flush_output_buffer(tdefl_compressor *d) {
  if (d->m_pIn_buf_size) {
    *d->m_pIn_buf_size = d->m_pSrc - (const mz_uint8 *)d->m_pIn_buf;
  }

  if (d->m_pOut_buf_size) {
    size_t n = MZ_MIN(*d->m_pOut_buf_size - d->m_out_buf_ofs,
                      d->m_output_flush_remaining);
    memcpy((mz_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs,
           d->m_output_buf + d->m_output_flush_ofs, n);
    d->m_output_flush_ofs += (mz_uint)n;
    d->m_output_flush_remaining -= (mz_uint)n;
    d->m_out_buf_ofs += n;

    *d->m_pOut_buf_size = d->m_out_buf_ofs;
  }

  return (d->m_finished && !d->m_output_flush_remaining) ? TDEFL_STATUS_DONE
                                                         : TDEFL_STATUS_OKAY;
}