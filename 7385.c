static size_t mz_zip_mem_read_func(void *pOpaque, mz_uint64 file_ofs,
                                   void *pBuf, size_t n) {
  mz_zip_archive *pZip = (mz_zip_archive *)pOpaque;
  size_t s = (file_ofs >= pZip->m_archive_size)
                 ? 0
                 : (size_t)MZ_MIN(pZip->m_archive_size - file_ofs, n);
  memcpy(pBuf, (const mz_uint8 *)pZip->m_pState->m_pMem + file_ofs, s);
  return s;
}