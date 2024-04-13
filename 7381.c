static mz_bool mz_zip_array_ensure_capacity(mz_zip_archive *pZip,
                                            mz_zip_array *pArray,
                                            size_t min_new_capacity,
                                            mz_uint growing) {
  void *pNew_p;
  size_t new_capacity = min_new_capacity;
  MZ_ASSERT(pArray->m_element_size);
  if (pArray->m_capacity >= min_new_capacity) return MZ_TRUE;
  if (growing) {
    new_capacity = MZ_MAX(1, pArray->m_capacity);
    while (new_capacity < min_new_capacity) new_capacity *= 2;
  }
  if (NULL == (pNew_p = pZip->m_pRealloc(pZip->m_pAlloc_opaque, pArray->m_p,
                                         pArray->m_element_size, new_capacity)))
    return MZ_FALSE;
  pArray->m_p = pNew_p;
  pArray->m_capacity = new_capacity;
  return MZ_TRUE;
}