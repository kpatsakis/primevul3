mz_ulong mz_compressBound(mz_ulong source_len) {
  return mz_deflateBound(NULL, source_len);
}