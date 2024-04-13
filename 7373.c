mz_bool mz_zip_reader_is_file_encrypted(mz_zip_archive *pZip,
                                        mz_uint file_index) {
  mz_uint m_bit_flag;
  const mz_uint8 *p = mz_zip_reader_get_cdh(pZip, file_index);
  if (!p) return MZ_FALSE;
  m_bit_flag = MZ_READ_LE16(p + MZ_ZIP_CDH_BIT_FLAG_OFS);
  return (m_bit_flag & 1);
}