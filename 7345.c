static void tdefl_optimize_huffman_table(tdefl_compressor *d, int table_num,
                                         int table_len, int code_size_limit,
                                         int static_table) {
  int i, j, l, num_codes[1 + TDEFL_MAX_SUPPORTED_HUFF_CODESIZE];
  mz_uint next_code[TDEFL_MAX_SUPPORTED_HUFF_CODESIZE + 1];
  MZ_CLEAR_OBJ(num_codes);
  if (static_table) {
    for (i = 0; i < table_len; i++)
      num_codes[d->m_huff_code_sizes[table_num][i]]++;
  } else {
    tdefl_sym_freq syms0[TDEFL_MAX_HUFF_SYMBOLS], syms1[TDEFL_MAX_HUFF_SYMBOLS],
        *pSyms;
    int num_used_syms = 0;
    const mz_uint16 *pSym_count = &d->m_huff_count[table_num][0];
    for (i = 0; i < table_len; i++)
      if (pSym_count[i]) {
        syms0[num_used_syms].m_key = (mz_uint16)pSym_count[i];
        syms0[num_used_syms++].m_sym_index = (mz_uint16)i;
      }

    pSyms = tdefl_radix_sort_syms(num_used_syms, syms0, syms1);
    tdefl_calculate_minimum_redundancy(pSyms, num_used_syms);

    for (i = 0; i < num_used_syms; i++) num_codes[pSyms[i].m_key]++;

    tdefl_huffman_enforce_max_code_size(num_codes, num_used_syms,
                                        code_size_limit);

    MZ_CLEAR_OBJ(d->m_huff_code_sizes[table_num]);
    MZ_CLEAR_OBJ(d->m_huff_codes[table_num]);
    for (i = 1, j = num_used_syms; i <= code_size_limit; i++)
      for (l = num_codes[i]; l > 0; l--)
        d->m_huff_code_sizes[table_num][pSyms[--j].m_sym_index] = (mz_uint8)(i);
  }

  next_code[1] = 0;
  for (j = 0, i = 2; i <= code_size_limit; i++)
    next_code[i] = j = ((j + num_codes[i - 1]) << 1);

  for (i = 0; i < table_len; i++) {
    mz_uint rev_code = 0, code, code_size;
    if ((code_size = d->m_huff_code_sizes[table_num][i]) == 0) continue;
    code = next_code[code_size]++;
    for (l = code_size; l > 0; l--, code >>= 1)
      rev_code = (rev_code << 1) | (code & 1);
    d->m_huff_codes[table_num][i] = (mz_uint16)rev_code;
  }
}