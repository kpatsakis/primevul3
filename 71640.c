MagickExport const LocaleInfo **GetLocaleInfoList(const char *pattern,
  size_t *number_messages,ExceptionInfo *exception)
{
  const LocaleInfo
    **messages;

  register const LocaleInfo
    *p;

  register ssize_t
    i;

  /*
    Allocate locale list.
  */
  assert(pattern != (char *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",pattern);
  assert(number_messages != (size_t *) NULL);
  *number_messages=0;
  p=GetLocaleInfo_("*",exception);
  if (p == (const LocaleInfo *) NULL)
    return((const LocaleInfo **) NULL);
  messages=(const LocaleInfo **) AcquireQuantumMemory((size_t)
    GetNumberOfNodesInSplayTree(locale_cache)+1UL,sizeof(*messages));
  if (messages == (const LocaleInfo **) NULL)
    return((const LocaleInfo **) NULL);
  /*
    Generate locale list.
  */
  LockSemaphoreInfo(locale_semaphore);
  ResetSplayTreeIterator(locale_cache);
  p=(const LocaleInfo *) GetNextValueInSplayTree(locale_cache);
  for (i=0; p != (const LocaleInfo *) NULL; )
  {
    if ((p->stealth == MagickFalse) &&
        (GlobExpression(p->tag,pattern,MagickTrue) != MagickFalse))
      messages[i++]=p;
    p=(const LocaleInfo *) GetNextValueInSplayTree(locale_cache);
  }
  UnlockSemaphoreInfo(locale_semaphore);
  qsort((void *) messages,(size_t) i,sizeof(*messages),LocaleInfoCompare);
  messages[i]=(LocaleInfo *) NULL;
  *number_messages=(size_t) i;
  return(messages);
}
