MagickExport int LocaleCompare(const char *p,const char *q)
{
  if (p == (char *) NULL)
    {
      if (q == (char *) NULL)
        return(0);
      return(-1);
    }
  if (q == (char *) NULL)
    return(1);
#if defined(MAGICKCORE_HAVE_STRCASECMP)
  return(strcasecmp(p,q));
#else
  {
    register int
      c,
      d;

    for ( ; ; )
    {
      c=(int) *((unsigned char *) p);
      d=(int) *((unsigned char *) q);
      if ((c == 0) || (AsciiMap[c] != AsciiMap[d]))
        break;
      p++;
      q++;
    }
    return(AsciiMap[c]-(int) AsciiMap[d]);
  }
#endif
}
