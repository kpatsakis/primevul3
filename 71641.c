MagickExport const LocaleInfo *GetLocaleInfo_(const char *tag,
  ExceptionInfo *exception)
{
  const LocaleInfo
    *locale_info;

  assert(exception != (ExceptionInfo *) NULL);
  if (IsLocaleTreeInstantiated(exception) == MagickFalse)
    return((const LocaleInfo *) NULL);
  LockSemaphoreInfo(locale_semaphore);
  if ((tag == (const char *) NULL) || (LocaleCompare(tag,"*") == 0))
    {
      ResetSplayTreeIterator(locale_cache);
      locale_info=(const LocaleInfo *) GetNextValueInSplayTree(locale_cache);
      UnlockSemaphoreInfo(locale_semaphore);
      return(locale_info);
    }
  locale_info=(const LocaleInfo *) GetValueFromSplayTree(locale_cache,tag);
  UnlockSemaphoreInfo(locale_semaphore);
  return(locale_info);
}
