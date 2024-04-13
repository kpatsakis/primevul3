static void tdefl_start_static_block(tdefl_compressor *d) {
  mz_uint i;
  mz_uint8 *p = &d->m_huff_code_sizes[0][0];

  for (i = 0; i <= 143; ++i) *p++ = 8;
  for (; i <= 255; ++i) *p++ = 9;
  for (; i <= 279; ++i) *p++ = 7;
  for (; i <= 287; ++i) *p++ = 8;

  memset(d->m_huff_code_sizes[1], 5, 32);

  tdefl_optimize_huffman_table(d, 0, 288, 15, MZ_TRUE);
  tdefl_optimize_huffman_table(d, 1, 32, 15, MZ_TRUE);

  TDEFL_PUT_BITS(1, 2);
}