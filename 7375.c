static MZ_FORCEINLINE mz_bool mz_zip_array_resize(mz_zip_archive *pZip,
                                                  mz_zip_array *pArray,
                                                  size_t new_size,
                                                  mz_uint growing) {
  if (new_size > pArray->m_capacity) {
    if (!mz_zip_array_ensure_capacity(pZip, pArray, new_size, growing))
      return MZ_FALSE;
  }
  pArray->m_size = new_size;
  return MZ_TRUE;
}