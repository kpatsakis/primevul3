static void tdefl_huffman_enforce_max_code_size(int *pNum_codes,
                                                int code_list_len,
                                                int max_code_size) {
  int i;
  mz_uint32 total = 0;
  if (code_list_len <= 1) return;
  for (i = max_code_size + 1; i <= TDEFL_MAX_SUPPORTED_HUFF_CODESIZE; i++)
    pNum_codes[max_code_size] += pNum_codes[i];
  for (i = max_code_size; i > 0; i--)
    total += (((mz_uint32)pNum_codes[i]) << (max_code_size - i));
  while (total != (1UL << max_code_size)) {
    pNum_codes[max_code_size]--;
    for (i = max_code_size - 1; i > 0; i--)
      if (pNum_codes[i]) {
        pNum_codes[i]--;
        pNum_codes[i + 1] += 2;
        break;
      }
    total--;
  }
}