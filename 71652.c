MagickPrivate void LocaleComponentTerminus(void)
{
  if (locale_semaphore == (SemaphoreInfo *) NULL)
    ActivateSemaphoreInfo(&locale_semaphore);
  LockSemaphoreInfo(locale_semaphore);
  if (locale_cache != (SplayTreeInfo *) NULL)
    locale_cache=DestroySplayTree(locale_cache);
#if defined(MAGICKCORE_LOCALE_SUPPORT)
  DestroyCLocale();
#endif
  UnlockSemaphoreInfo(locale_semaphore);
  RelinquishSemaphoreInfo(&locale_semaphore);
}
