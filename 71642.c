MagickExport char **GetLocaleList(const char *pattern,size_t *number_messages,
  ExceptionInfo *exception)
{
  char
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
    return((char **) NULL);
  messages=(char **) AcquireQuantumMemory((size_t)
    GetNumberOfNodesInSplayTree(locale_cache)+1UL,sizeof(*messages));
  if (messages == (char **) NULL)
    return((char **) NULL);
  LockSemaphoreInfo(locale_semaphore);
  p=(const LocaleInfo *) GetNextValueInSplayTree(locale_cache);
  for (i=0; p != (const LocaleInfo *) NULL; )
  {
    if ((p->stealth == MagickFalse) &&
        (GlobExpression(p->tag,pattern,MagickTrue) != MagickFalse))
      messages[i++]=ConstantString(p->tag);
    p=(const LocaleInfo *) GetNextValueInSplayTree(locale_cache);
  }
  UnlockSemaphoreInfo(locale_semaphore);
  qsort((void *) messages,(size_t) i,sizeof(*messages),LocaleTagCompare);
  messages[i]=(char *) NULL;
  *number_messages=(size_t) i;
  return(messages);
}
