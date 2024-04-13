static size_t mz_zip_file_read_func(void *pOpaque, mz_uint64 file_ofs,
                                    void *pBuf, size_t n) {
  mz_zip_archive *pZip = (mz_zip_archive *)pOpaque;
  mz_int64 cur_ofs = MZ_FTELL64(pZip->m_pState->m_pFile);
  if (((mz_int64)file_ofs < 0) ||
      (((cur_ofs != (mz_int64)file_ofs)) &&
       (MZ_FSEEK64(pZip->m_pState->m_pFile, (mz_int64)file_ofs, SEEK_SET))))
    return 0;
  return MZ_FREAD(pBuf, 1, n, pZip->m_pState->m_pFile);
}