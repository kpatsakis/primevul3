MagickPrivate ssize_t FormatLocaleStringList(char *magick_restrict string,
  const size_t length,const char *magick_restrict format,va_list operands)
{
  ssize_t
    n;

#if defined(MAGICKCORE_LOCALE_SUPPORT) && defined(MAGICKCORE_HAVE_VSNPRINTF_L)
  {
    locale_t
      locale;

    locale=AcquireCLocale();
    if (locale == (locale_t) NULL)
      n=(ssize_t) vsnprintf(string,length,format,operands);
    else
#if defined(MAGICKCORE_WINDOWS_SUPPORT)
      n=(ssize_t) vsnprintf_l(string,length,format,locale,operands);
#else
      n=(ssize_t) vsnprintf_l(string,length,locale,format,operands);
#endif
  }
#elif defined(MAGICKCORE_HAVE_VSNPRINTF)
#if defined(MAGICKCORE_LOCALE_SUPPORT) && defined(MAGICKCORE_HAVE_USELOCALE)
  {
    locale_t
      locale,
      previous_locale;

    locale=AcquireCLocale();
    if (locale == (locale_t) NULL)
      n=(ssize_t) vsnprintf(string,length,format,operands);
    else
      {
        previous_locale=uselocale(locale);
        n=(ssize_t) vsnprintf(string,length,format,operands);
        uselocale(previous_locale);
      }
  }
#else
  n=(ssize_t) vsnprintf(string,length,format,operands);
#endif
#else
  n=(ssize_t) vsprintf(string,format,operands);
#endif
  if (n < 0)
    string[length-1]='\0';
  return(n);
}
