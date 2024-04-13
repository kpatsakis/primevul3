MagickPrivate ssize_t FormatLocaleFileList(FILE *file,
  const char *magick_restrict format,va_list operands)
{
  ssize_t
    n;

#if defined(MAGICKCORE_LOCALE_SUPPORT) && defined(MAGICKCORE_HAVE_VFPRINTF_L)
  {
    locale_t
      locale;

    locale=AcquireCLocale();
    if (locale == (locale_t) NULL)
      n=(ssize_t) vfprintf(file,format,operands);
    else
#if defined(MAGICKCORE_WINDOWS_SUPPORT)
      n=(ssize_t) vfprintf_l(file,format,locale,operands);
#else
      n=(ssize_t) vfprintf_l(file,locale,format,operands);
#endif
  }
#else
#if defined(MAGICKCORE_LOCALE_SUPPORT) && defined(MAGICKCORE_HAVE_USELOCALE)
  {
    locale_t
      locale,
      previous_locale;

    locale=AcquireCLocale();
    if (locale == (locale_t) NULL)
      n=(ssize_t) vfprintf(file,format,operands);
    else
      {
        previous_locale=uselocale(locale);
        n=(ssize_t) vfprintf(file,format,operands);
        uselocale(previous_locale);
      }
  }
#else
  n=(ssize_t) vfprintf(file,format,operands);
#endif
#endif
  return(n);
}
