mz_uint tdefl_create_comp_flags_from_zip_params(int level, int window_bits,
                                                int strategy) {
  mz_uint comp_flags =
      s_tdefl_num_probes[(level >= 0) ? MZ_MIN(10, level) : MZ_DEFAULT_LEVEL] |
      ((level <= 3) ? TDEFL_GREEDY_PARSING_FLAG : 0);
  if (window_bits > 0) comp_flags |= TDEFL_WRITE_ZLIB_HEADER;

  if (!level)
    comp_flags |= TDEFL_FORCE_ALL_RAW_BLOCKS;
  else if (strategy == MZ_FILTERED)
    comp_flags |= TDEFL_FILTER_MATCHES;
  else if (strategy == MZ_HUFFMAN_ONLY)
    comp_flags &= ~TDEFL_MAX_PROBES_MASK;
  else if (strategy == MZ_FIXED)
    comp_flags |= TDEFL_FORCE_ALL_STATIC_BLOCKS;
  else if (strategy == MZ_RLE)
    comp_flags |= TDEFL_RLE_MATCHES;

  return comp_flags;
}