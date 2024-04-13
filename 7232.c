mz_bool mz_zip_writer_finalize_heap_archive(mz_zip_archive *pZip, void **pBuf,
                                            size_t *pSize) {
  if ((!pZip) || (!pZip->m_pState) || (!pBuf) || (!pSize)) return MZ_FALSE;
  if (pZip->m_pWrite != mz_zip_heap_write_func) return MZ_FALSE;
  if (!mz_zip_writer_finalize_archive(pZip)) return MZ_FALSE;

  *pBuf = pZip->m_pState->m_pMem;
  *pSize = pZip->m_pState->m_mem_size;
  pZip->m_pState->m_pMem = NULL;
  pZip->m_pState->m_mem_size = pZip->m_pState->m_mem_capacity = 0;
  return MZ_TRUE;
}