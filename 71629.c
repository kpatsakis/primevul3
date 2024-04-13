static locale_t AcquireCLocale(void)
{
#if defined(MAGICKCORE_HAVE_NEWLOCALE)
  if (c_locale == (locale_t) NULL)
    c_locale=newlocale(LC_ALL_MASK,"C",(locale_t) 0);
#elif defined(MAGICKCORE_WINDOWS_SUPPORT) && !defined(__MINGW32__)
  if (c_locale == (locale_t) NULL)
    c_locale=_create_locale(LC_ALL,"C");
#endif
  return(c_locale);
}
