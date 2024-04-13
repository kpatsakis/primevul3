static size_t mz_zip_heap_write_func(void *pOpaque, mz_uint64 file_ofs,
                                     const void *pBuf, size_t n) {
  mz_zip_archive *pZip = (mz_zip_archive *)pOpaque;
  mz_zip_internal_state *pState = pZip->m_pState;
  mz_uint64 new_size = MZ_MAX(file_ofs + n, pState->m_mem_size);
#ifdef _MSC_VER
  if ((!n) ||
      ((0, sizeof(size_t) == sizeof(mz_uint32)) && (new_size > 0x7FFFFFFF)))
#else
  if ((!n) ||
      ((sizeof(size_t) == sizeof(mz_uint32)) && (new_size > 0x7FFFFFFF)))
#endif
    return 0;
  if (new_size > pState->m_mem_capacity) {
    void *pNew_block;
    size_t new_capacity = MZ_MAX(64, pState->m_mem_capacity);
    while (new_capacity < new_size) new_capacity *= 2;
    if (NULL == (pNew_block = pZip->m_pRealloc(
                     pZip->m_pAlloc_opaque, pState->m_pMem, 1, new_capacity)))
      return 0;
    pState->m_pMem = pNew_block;
    pState->m_mem_capacity = new_capacity;
  }
  memcpy((mz_uint8 *)pState->m_pMem + file_ofs, pBuf, n);
  pState->m_mem_size = (size_t)new_size;
  return n;
}