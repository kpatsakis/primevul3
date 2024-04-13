static MZ_FORCEINLINE void tdefl_record_literal(tdefl_compressor *d,
                                                mz_uint8 lit) {
  d->m_total_lz_bytes++;
  *d->m_pLZ_code_buf++ = lit;
  *d->m_pLZ_flags = (mz_uint8)(*d->m_pLZ_flags >> 1);
  if (--d->m_num_flags_left == 0) {
    d->m_num_flags_left = 8;
    d->m_pLZ_flags = d->m_pLZ_code_buf++;
  }
  d->m_huff_count[0][lit]++;
}