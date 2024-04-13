static mz_bool mz_zip_get_file_modified_time(const char *pFilename,
                                             mz_uint16 *pDOS_time,
                                             mz_uint16 *pDOS_date) {
#ifdef MINIZ_NO_TIME
  (void)pFilename;
  *pDOS_date = *pDOS_time = 0;
#else
  struct MZ_FILE_STAT_STRUCT file_stat;
  // On Linux with x86 glibc, this call will fail on large files (>= 0x80000000
  // bytes) unless you compiled with _LARGEFILE64_SOURCE. Argh.
  if (MZ_FILE_STAT(pFilename, &file_stat) != 0) return MZ_FALSE;
  mz_zip_time_to_dos_time(file_stat.st_mtime, pDOS_time, pDOS_date);
#endif  // #ifdef MINIZ_NO_TIME
  return MZ_TRUE;
}