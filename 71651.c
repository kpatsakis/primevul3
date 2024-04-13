MagickPrivate MagickBooleanType LocaleComponentGenesis(void)
{
  if (locale_semaphore == (SemaphoreInfo *) NULL)
    locale_semaphore=AcquireSemaphoreInfo();
  return(MagickTrue);
}
