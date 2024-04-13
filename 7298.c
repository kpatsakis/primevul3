mz_bool mz_zip_reader_is_file_a_directory(mz_zip_archive *pZip,
                                          mz_uint file_index) {
  mz_uint filename_len, external_attr;
  const mz_uint8 *p = mz_zip_reader_get_cdh(pZip, file_index);
  if (!p) return MZ_FALSE;

  // First see if the filename ends with a '/' character.
  filename_len = MZ_READ_LE16(p + MZ_ZIP_CDH_FILENAME_LEN_OFS);
  if (filename_len) {
    if (*(p + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_len - 1) == '/')
      return MZ_TRUE;
  }

  // Bugfix: This code was also checking if the internal attribute was non-zero,
  // which wasn't correct.
  // Most/all zip writers (hopefully) set DOS file/directory attributes in the
  // low 16-bits, so check for the DOS directory flag and ignore the source OS
  // ID in the created by field.
  // FIXME: Remove this check? Is it necessary - we already check the filename.
  external_attr = MZ_READ_LE32(p + MZ_ZIP_CDH_EXTERNAL_ATTR_OFS);
  if ((external_attr & 0x10) != 0) return MZ_TRUE;

  return MZ_FALSE;
}