static MZ_FORCEINLINE mz_bool mz_zip_reader_string_equal(const char *pA,
                                                         const char *pB,
                                                         mz_uint len,
                                                         mz_uint flags) {
  mz_uint i;
  if (flags & MZ_ZIP_FLAG_CASE_SENSITIVE) return 0 == memcmp(pA, pB, len);
  for (i = 0; i < len; ++i)
    if (MZ_TOLOWER(pA[i]) != MZ_TOLOWER(pB[i])) return MZ_FALSE;
  return MZ_TRUE;
}