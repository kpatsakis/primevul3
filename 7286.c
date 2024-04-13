static MZ_FORCEINLINE int mz_zip_reader_filename_compare(
    const mz_zip_array *pCentral_dir_array,
    const mz_zip_array *pCentral_dir_offsets, mz_uint l_index, const char *pR,
    mz_uint r_len) {
  const mz_uint8 *pL = &MZ_ZIP_ARRAY_ELEMENT(
                     pCentral_dir_array, mz_uint8,
                     MZ_ZIP_ARRAY_ELEMENT(pCentral_dir_offsets, mz_uint32,
                                          l_index)),
                 *pE;
  mz_uint l_len = MZ_READ_LE16(pL + MZ_ZIP_CDH_FILENAME_LEN_OFS);
  mz_uint8 l = 0, r = 0;
  pL += MZ_ZIP_CENTRAL_DIR_HEADER_SIZE;
  pE = pL + MZ_MIN(l_len, r_len);
  while (pL < pE) {
    if ((l = MZ_TOLOWER(*pL)) != (r = MZ_TOLOWER(*pR))) break;
    pL++;
    pR++;
  }
  return (pL == pE) ? (int)(l_len - r_len) : (l - r);
}