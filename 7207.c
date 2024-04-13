mz_bool mz_zip_reader_init_file(mz_zip_archive *pZip, const char *pFilename,
                                mz_uint32 flags) {
  mz_uint64 file_size;
  MZ_FILE *pFile = MZ_FOPEN(pFilename, "rb");
  if (!pFile) return MZ_FALSE;
  if (MZ_FSEEK64(pFile, 0, SEEK_END)) {
    MZ_FCLOSE(pFile);
    return MZ_FALSE;
  }
  file_size = MZ_FTELL64(pFile);
  if (!mz_zip_reader_init_internal(pZip, flags)) {
    MZ_FCLOSE(pFile);
    return MZ_FALSE;
  }
  pZip->m_pRead = mz_zip_file_read_func;
  pZip->m_pIO_opaque = pZip;
  pZip->m_pState->m_pFile = pFile;
  pZip->m_archive_size = file_size;
  if (!mz_zip_reader_read_central_dir(pZip, flags)) {
    mz_zip_reader_end(pZip);
    return MZ_FALSE;
  }
  return MZ_TRUE;
}