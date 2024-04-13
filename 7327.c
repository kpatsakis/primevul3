static tdefl_sym_freq *tdefl_radix_sort_syms(mz_uint num_syms,
                                             tdefl_sym_freq *pSyms0,
                                             tdefl_sym_freq *pSyms1) {
  mz_uint32 total_passes = 2, pass_shift, pass, i, hist[256 * 2];
  tdefl_sym_freq *pCur_syms = pSyms0, *pNew_syms = pSyms1;
  MZ_CLEAR_OBJ(hist);
  for (i = 0; i < num_syms; i++) {
    mz_uint freq = pSyms0[i].m_key;
    hist[freq & 0xFF]++;
    hist[256 + ((freq >> 8) & 0xFF)]++;
  }
  while ((total_passes > 1) && (num_syms == hist[(total_passes - 1) * 256]))
    total_passes--;
  for (pass_shift = 0, pass = 0; pass < total_passes; pass++, pass_shift += 8) {
    const mz_uint32 *pHist = &hist[pass << 8];
    mz_uint offsets[256], cur_ofs = 0;
    for (i = 0; i < 256; i++) {
      offsets[i] = cur_ofs;
      cur_ofs += pHist[i];
    }
    for (i = 0; i < num_syms; i++)
      pNew_syms[offsets[(pCur_syms[i].m_key >> pass_shift) & 0xFF]++] =
          pCur_syms[i];
    {
      tdefl_sym_freq *t = pCur_syms;
      pCur_syms = pNew_syms;
      pNew_syms = t;
    }
  }
  return pCur_syms;
}