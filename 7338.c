int mz_compress(unsigned char *pDest, mz_ulong *pDest_len,
                const unsigned char *pSource, mz_ulong source_len) {
  return mz_compress2(pDest, pDest_len, pSource, source_len,
                      MZ_DEFAULT_COMPRESSION);
}