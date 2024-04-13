MagickExport double InterpretLocaleValue(const char *magick_restrict string,
  char **magick_restrict sentinal)
{
  char
    *q;

  double
    value;

  if ((*string == '0') && ((string[1] | 0x20)=='x'))
    value=(double) strtoul(string,&q,16);
  else
    {
#if defined(MAGICKCORE_LOCALE_SUPPORT) && defined(MAGICKCORE_HAVE_STRTOD_L)
      locale_t
        locale;

      locale=AcquireCLocale();
      if (locale == (locale_t) NULL)
        value=strtod(string,&q);
      else
        value=strtod_l(string,&q,locale);
#else
      value=strtod(string,&q);
#endif
    }
  if (sentinal != (char **) NULL)
    *sentinal=q;
  return(value);
}
