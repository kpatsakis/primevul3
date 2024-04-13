static mz_uint mz_zip_writer_compute_padding_needed_for_file_alignment(
    mz_zip_archive *pZip) {
  mz_uint32 n;
  if (!pZip->m_file_offset_alignment) return 0;
  n = (mz_uint32)(pZip->m_archive_size & (pZip->m_file_offset_alignment - 1));
  return (pZip->m_file_offset_alignment - n) &
         (pZip->m_file_offset_alignment - 1);
}