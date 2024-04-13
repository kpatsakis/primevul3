static void DestroyCLocale(void)
{
  if (c_locale != (locale_t) NULL)
    freelocale(c_locale);
  c_locale=(locale_t) NULL;
}
