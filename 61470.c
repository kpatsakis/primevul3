static inline int MagickReadDirectory(DIR *directory,struct dirent *entry,
  struct dirent **result)
{
#if defined(MAGICKCORE_HAVE_READDIR_R)
  return(readdir_r(directory,entry,result));
#else
  (void) entry;
  errno=0;
  *result=readdir(directory);
  return(errno);
#endif
}
