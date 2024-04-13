file_accessible (char const *file)
{
# if defined _LIBC || HAVE_FACCESSAT
  return __faccessat (AT_FDCWD, file, F_OK, AT_EACCESS) == 0;
# else
  struct stat st;
  return __stat (file, &st) == 0 || errno == EOVERFLOW;
# endif
}