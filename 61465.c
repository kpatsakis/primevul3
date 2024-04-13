MagickExport const ModuleInfo **GetModuleInfoList(const char *pattern,
  size_t *number_modules,ExceptionInfo *exception)
{
  const ModuleInfo
    **modules;

  register const ModuleInfo
    *p;

  register ssize_t
    i;

  /*
    Allocate module list.
  */
  assert(pattern != (char *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",pattern);
  assert(number_modules != (size_t *) NULL);
  *number_modules=0;
  p=GetModuleInfo("*",exception);
  if (p == (const ModuleInfo *) NULL)
    return((const ModuleInfo **) NULL);
  modules=(const ModuleInfo **) AcquireQuantumMemory((size_t)
    GetNumberOfNodesInSplayTree(module_list)+1UL,sizeof(*modules));
  if (modules == (const ModuleInfo **) NULL)
    return((const ModuleInfo **) NULL);
  /*
    Generate module list.
  */
  LockSemaphoreInfo(module_semaphore);
  ResetSplayTreeIterator(module_list);
  p=(const ModuleInfo *) GetNextValueInSplayTree(module_list);
  for (i=0; p != (const ModuleInfo *) NULL; )
  {
    if ((p->stealth == MagickFalse) &&
        (GlobExpression(p->tag,pattern,MagickFalse) != MagickFalse))
      modules[i++]=p;
    p=(const ModuleInfo *) GetNextValueInSplayTree(module_list);
  }
  UnlockSemaphoreInfo(module_semaphore);
  qsort((void *) modules,(size_t) i,sizeof(*modules),ModuleInfoCompare);
  modules[i]=(ModuleInfo *) NULL;
  *number_modules=(size_t) i;
  return(modules);
}
