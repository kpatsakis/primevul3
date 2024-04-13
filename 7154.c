get_path_max (void)
{
# ifdef PATH_MAX
  long int path_max = PATH_MAX;
# else
  /* The caller invoked realpath with a null RESOLVED, even though
     PATH_MAX is not defined as a constant.  The glibc manual says
     programs should not do this, and POSIX says the behavior is undefined.
     Historically, glibc here used the result of pathconf, or 1024 if that
     failed; stay consistent with this (dubious) historical practice.  */
  int err = errno;
  long int path_max = __pathconf ("/", _PC_PATH_MAX);
  __set_errno (err);
# endif
  return path_max < 0 ? 1024 : path_max <= IDX_MAX ? path_max : IDX_MAX;
}