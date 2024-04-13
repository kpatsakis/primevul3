static mz_bool tdefl_compress_lz_codes(tdefl_compressor *d) {
  mz_uint flags;
  mz_uint8 *pLZ_codes;

  flags = 1;
  for (pLZ_codes = d->m_lz_code_buf; pLZ_codes < d->m_pLZ_code_buf;
       flags >>= 1) {
    if (flags == 1) flags = *pLZ_codes++ | 0x100;
    if (flags & 1) {
      mz_uint sym, num_extra_bits;
      mz_uint match_len = pLZ_codes[0],
              match_dist = (pLZ_codes[1] | (pLZ_codes[2] << 8));
      pLZ_codes += 3;

      MZ_ASSERT(d->m_huff_code_sizes[0][s_tdefl_len_sym[match_len]]);
      TDEFL_PUT_BITS(d->m_huff_codes[0][s_tdefl_len_sym[match_len]],
                     d->m_huff_code_sizes[0][s_tdefl_len_sym[match_len]]);
      TDEFL_PUT_BITS(match_len & mz_bitmasks[s_tdefl_len_extra[match_len]],
                     s_tdefl_len_extra[match_len]);

      if (match_dist < 512) {
        sym = s_tdefl_small_dist_sym[match_dist];
        num_extra_bits = s_tdefl_small_dist_extra[match_dist];
      } else {
        sym = s_tdefl_large_dist_sym[match_dist >> 8];
        num_extra_bits = s_tdefl_large_dist_extra[match_dist >> 8];
      }
      MZ_ASSERT(d->m_huff_code_sizes[1][sym]);
      TDEFL_PUT_BITS(d->m_huff_codes[1][sym], d->m_huff_code_sizes[1][sym]);
      TDEFL_PUT_BITS(match_dist & mz_bitmasks[num_extra_bits], num_extra_bits);
    } else {
      mz_uint lit = *pLZ_codes++;
      MZ_ASSERT(d->m_huff_code_sizes[0][lit]);
      TDEFL_PUT_BITS(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
    }
  }

  TDEFL_PUT_BITS(d->m_huff_codes[0][256], d->m_huff_code_sizes[0][256]);

  return (d->m_pOutput_buf < d->m_pOutput_buf_end);
}