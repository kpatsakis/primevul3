static MZ_FORCEINLINE void mz_zip_array_clear(mz_zip_archive *pZip,
                                              mz_zip_array *pArray) {
  pZip->m_pFree(pZip->m_pAlloc_opaque, pArray->m_p);
  memset(pArray, 0, sizeof(mz_zip_array));
}