static MZ_FORCEINLINE mz_bool mz_zip_array_reserve(mz_zip_archive *pZip,
                                                   mz_zip_array *pArray,
                                                   size_t new_capacity,
                                                   mz_uint growing) {
  if (new_capacity > pArray->m_capacity) {
    if (!mz_zip_array_ensure_capacity(pZip, pArray, new_capacity, growing))
      return MZ_FALSE;
  }
  return MZ_TRUE;
}