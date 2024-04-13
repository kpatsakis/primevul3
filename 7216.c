static void tdefl_start_dynamic_block(tdefl_compressor *d) {
  int num_lit_codes, num_dist_codes, num_bit_lengths;
  mz_uint i, total_code_sizes_to_pack, num_packed_code_sizes, rle_z_count,
      rle_repeat_count, packed_code_sizes_index;
  mz_uint8
      code_sizes_to_pack[TDEFL_MAX_HUFF_SYMBOLS_0 + TDEFL_MAX_HUFF_SYMBOLS_1],
      packed_code_sizes[TDEFL_MAX_HUFF_SYMBOLS_0 + TDEFL_MAX_HUFF_SYMBOLS_1],
      prev_code_size = 0xFF;

  d->m_huff_count[0][256] = 1;

  tdefl_optimize_huffman_table(d, 0, TDEFL_MAX_HUFF_SYMBOLS_0, 15, MZ_FALSE);
  tdefl_optimize_huffman_table(d, 1, TDEFL_MAX_HUFF_SYMBOLS_1, 15, MZ_FALSE);

  for (num_lit_codes = 286; num_lit_codes > 257; num_lit_codes--)
    if (d->m_huff_code_sizes[0][num_lit_codes - 1]) break;
  for (num_dist_codes = 30; num_dist_codes > 1; num_dist_codes--)
    if (d->m_huff_code_sizes[1][num_dist_codes - 1]) break;

  memcpy(code_sizes_to_pack, &d->m_huff_code_sizes[0][0], num_lit_codes);
  memcpy(code_sizes_to_pack + num_lit_codes, &d->m_huff_code_sizes[1][0],
         num_dist_codes);
  total_code_sizes_to_pack = num_lit_codes + num_dist_codes;
  num_packed_code_sizes = 0;
  rle_z_count = 0;
  rle_repeat_count = 0;

  memset(&d->m_huff_count[2][0], 0,
         sizeof(d->m_huff_count[2][0]) * TDEFL_MAX_HUFF_SYMBOLS_2);
  for (i = 0; i < total_code_sizes_to_pack; i++) {
    mz_uint8 code_size = code_sizes_to_pack[i];
    if (!code_size) {
      TDEFL_RLE_PREV_CODE_SIZE();
      if (++rle_z_count == 138) {
        TDEFL_RLE_ZERO_CODE_SIZE();
      }
    } else {
      TDEFL_RLE_ZERO_CODE_SIZE();
      if (code_size != prev_code_size) {
        TDEFL_RLE_PREV_CODE_SIZE();
        d->m_huff_count[2][code_size] =
            (mz_uint16)(d->m_huff_count[2][code_size] + 1);
        packed_code_sizes[num_packed_code_sizes++] = code_size;
      } else if (++rle_repeat_count == 6) {
        TDEFL_RLE_PREV_CODE_SIZE();
      }
    }
    prev_code_size = code_size;
  }
  if (rle_repeat_count) {
    TDEFL_RLE_PREV_CODE_SIZE();
  } else {
    TDEFL_RLE_ZERO_CODE_SIZE();
  }

  tdefl_optimize_huffman_table(d, 2, TDEFL_MAX_HUFF_SYMBOLS_2, 7, MZ_FALSE);

  TDEFL_PUT_BITS(2, 2);

  TDEFL_PUT_BITS(num_lit_codes - 257, 5);
  TDEFL_PUT_BITS(num_dist_codes - 1, 5);

  for (num_bit_lengths = 18; num_bit_lengths >= 0; num_bit_lengths--)
    if (d->m_huff_code_sizes
            [2][s_tdefl_packed_code_size_syms_swizzle[num_bit_lengths]])
      break;
  num_bit_lengths = MZ_MAX(4, (num_bit_lengths + 1));
  TDEFL_PUT_BITS(num_bit_lengths - 4, 4);
  for (i = 0; (int)i < num_bit_lengths; i++)
    TDEFL_PUT_BITS(
        d->m_huff_code_sizes[2][s_tdefl_packed_code_size_syms_swizzle[i]], 3);

  for (packed_code_sizes_index = 0;
       packed_code_sizes_index < num_packed_code_sizes;) {
    mz_uint code = packed_code_sizes[packed_code_sizes_index++];
    MZ_ASSERT(code < TDEFL_MAX_HUFF_SYMBOLS_2);
    TDEFL_PUT_BITS(d->m_huff_codes[2][code], d->m_huff_code_sizes[2][code]);
    if (code >= 16)
      TDEFL_PUT_BITS(packed_code_sizes[packed_code_sizes_index++],
                     "\02\03\07"[code - 16]);
  }
}