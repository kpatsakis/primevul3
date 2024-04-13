static MZ_FORCEINLINE mz_bool mz_zip_array_ensure_room(mz_zip_archive *pZip,
                                                       mz_zip_array *pArray,
                                                       size_t n) {
  return mz_zip_array_reserve(pZip, pArray, pArray->m_size + n, MZ_TRUE);
}