mz_bool mz_zip_writer_init_heap(mz_zip_archive *pZip,
                                size_t size_to_reserve_at_beginning,
                                size_t initial_allocation_size) {
  pZip->m_pWrite = mz_zip_heap_write_func;
  pZip->m_pIO_opaque = pZip;
  if (!mz_zip_writer_init(pZip, size_to_reserve_at_beginning)) return MZ_FALSE;
  if (0 != (initial_allocation_size = MZ_MAX(initial_allocation_size,
                                             size_to_reserve_at_beginning))) {
    if (NULL == (pZip->m_pState->m_pMem = pZip->m_pAlloc(
                     pZip->m_pAlloc_opaque, 1, initial_allocation_size))) {
      mz_zip_writer_end(pZip);
      return MZ_FALSE;
    }
    pZip->m_pState->m_mem_capacity = initial_allocation_size;
  }
  return MZ_TRUE;
}