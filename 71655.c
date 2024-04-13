MagickExport int LocaleNCompare(const char *p,const char *q,const size_t length)
{
  if (p == (char *) NULL)
    {
      if (q == (char *) NULL)
        return(0);
      return(-1);
    }
  if (q == (char *) NULL)
    return(1);
#if defined(MAGICKCORE_HAVE_STRNCASECMP)
  return(strncasecmp(p,q,length));
#else
  {
    register int
      c,
      d;

    register size_t
      i;

    for (i=length; i != 0; i--)
    {
      c=(int) *((unsigned char *) p);
      d=(int) *((unsigned char *) q);
      if (AsciiMap[c] != AsciiMap[d])
        return(AsciiMap[c]-(int) AsciiMap[d]);
      if (c == 0)
        return(0);
      p++;
      q++;
    }
    return(0);
  }
#endif
}
