static void mz_zip_time_to_dos_time(time_t time, mz_uint16 *pDOS_time,
                                    mz_uint16 *pDOS_date) {
#ifdef _MSC_VER
  struct tm tm_struct;
  struct tm *tm = &tm_struct;
  errno_t err = localtime_s(tm, &time);
  if (err) {
    *pDOS_date = 0;
    *pDOS_time = 0;
    return;
  }
#else
  struct tm *tm = localtime(&time);
#endif
  *pDOS_time = (mz_uint16)(((tm->tm_hour) << 11) + ((tm->tm_min) << 5) +
                           ((tm->tm_sec) >> 1));
  *pDOS_date = (mz_uint16)(((tm->tm_year + 1900 - 1980) << 9) +
                           ((tm->tm_mon + 1) << 5) + tm->tm_mday);
}