static MZ_FORCEINLINE mz_bool mz_zip_array_push_back(mz_zip_archive *pZip,
                                                     mz_zip_array *pArray,
                                                     const void *pElements,
                                                     size_t n) {
  size_t orig_size = pArray->m_size;
  if (!mz_zip_array_resize(pZip, pArray, orig_size + n, MZ_TRUE))
    return MZ_FALSE;
  memcpy((mz_uint8 *)pArray->m_p + orig_size * pArray->m_element_size,
         pElements, n * pArray->m_element_size);
  return MZ_TRUE;
}