mz_uint mz_zip_reader_get_num_files(mz_zip_archive *pZip) {
  return pZip ? pZip->m_total_files : 0;
}