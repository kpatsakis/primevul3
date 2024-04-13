mz_bool mz_zip_reader_init(mz_zip_archive *pZip, mz_uint64 size,
                           mz_uint32 flags) {
  if ((!pZip) || (!pZip->m_pRead)) return MZ_FALSE;
  if (!mz_zip_reader_init_internal(pZip, flags)) return MZ_FALSE;
  pZip->m_archive_size = size;
  if (!mz_zip_reader_read_central_dir(pZip, flags)) {
    mz_zip_reader_end(pZip);
    return MZ_FALSE;
  }
  return MZ_TRUE;
}