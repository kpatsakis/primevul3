mz_bool mz_zip_writer_init_file(mz_zip_archive *pZip, const char *pFilename,
                                mz_uint64 size_to_reserve_at_beginning) {
  MZ_FILE *pFile;
  pZip->m_pWrite = mz_zip_file_write_func;
  pZip->m_pIO_opaque = pZip;
  if (!mz_zip_writer_init(pZip, size_to_reserve_at_beginning)) return MZ_FALSE;
  if (NULL == (pFile = MZ_FOPEN(pFilename, "wb"))) {
    mz_zip_writer_end(pZip);
    return MZ_FALSE;
  }
  pZip->m_pState->m_pFile = pFile;
  if (size_to_reserve_at_beginning) {
    mz_uint64 cur_ofs = 0;
    char buf[4096];
    MZ_CLEAR_OBJ(buf);
    do {
      size_t n = (size_t)MZ_MIN(sizeof(buf), size_to_reserve_at_beginning);
      if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_ofs, buf, n) != n) {
        mz_zip_writer_end(pZip);
        return MZ_FALSE;
      }
      cur_ofs += n;
      size_to_reserve_at_beginning -= n;
    } while (size_to_reserve_at_beginning);
  }
  return MZ_TRUE;
}