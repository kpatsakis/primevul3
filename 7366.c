static mz_bool mz_zip_set_file_times(const char *pFilename, time_t access_time,
                                     time_t modified_time) {
  struct utimbuf t;
  t.actime = access_time;
  t.modtime = modified_time;
  return !utime(pFilename, &t);
}