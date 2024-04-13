static MagickBooleanType IsLocaleTreeInstantiated(ExceptionInfo *exception)
{
  if (locale_cache == (SplayTreeInfo *) NULL)
    {
      if (locale_semaphore == (SemaphoreInfo *) NULL)
        ActivateSemaphoreInfo(&locale_semaphore);
      LockSemaphoreInfo(locale_semaphore);
      if (locale_cache == (SplayTreeInfo *) NULL)
        {
          char
            *locale;

          register const char
            *p;

          locale=(char *) NULL;
          p=setlocale(LC_CTYPE,(const char *) NULL);
          if (p != (const char *) NULL)
            locale=ConstantString(p);
          if (locale == (char *) NULL)
            locale=GetEnvironmentValue("LC_ALL");
          if (locale == (char *) NULL)
            locale=GetEnvironmentValue("LC_MESSAGES");
          if (locale == (char *) NULL)
            locale=GetEnvironmentValue("LC_CTYPE");
          if (locale == (char *) NULL)
            locale=GetEnvironmentValue("LANG");
          if (locale == (char *) NULL)
            locale=ConstantString("C");
          locale_cache=AcquireLocaleSplayTree(LocaleFilename,locale,exception);
          locale=DestroyString(locale);
        }
      UnlockSemaphoreInfo(locale_semaphore);
    }
  return(locale_cache != (SplayTreeInfo *) NULL ? MagickTrue : MagickFalse);
}
