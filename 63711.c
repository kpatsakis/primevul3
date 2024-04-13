decimal_point_char (void)
{
  const char *point;
  /* Determine it in a multithread-safe way.  We know nl_langinfo is
     multithread-safe on glibc systems and Mac OS X systems, but is not required
     to be multithread-safe by POSIX.  sprintf(), however, is multithread-safe.
     localeconv() is rarely multithread-safe.  */
#  if HAVE_NL_LANGINFO && (__GLIBC__ || defined __UCLIBC__ || (defined __APPLE__ && defined __MACH__))
  point = nl_langinfo (RADIXCHAR);
#  elif 1
  char pointbuf[5];
  sprintf (pointbuf, "%#.0f", 1.0);
  point = &pointbuf[1];
#  else
  point = localeconv () -> decimal_point;
#  endif
  /* The decimal point is always a single byte: either '.' or ','.  */
  return (point[0] != '\0' ? point[0] : '.');
}
