mz_bool mz_zip_reader_init_mem(mz_zip_archive *pZip, const void *pMem,
                               size_t size, mz_uint32 flags) {
  if (!mz_zip_reader_init_internal(pZip, flags)) return MZ_FALSE;
  pZip->m_archive_size = size;
  pZip->m_pRead = mz_zip_mem_read_func;
  pZip->m_pIO_opaque = pZip;
#ifdef __cplusplus
  pZip->m_pState->m_pMem = const_cast<void *>(pMem);
#else
  pZip->m_pState->m_pMem = (void *)pMem;
#endif
  pZip->m_pState->m_mem_size = size;
  if (!mz_zip_reader_read_central_dir(pZip, flags)) {
    mz_zip_reader_end(pZip);
    return MZ_FALSE;
  }
  return MZ_TRUE;
}