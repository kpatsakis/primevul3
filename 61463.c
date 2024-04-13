static void *DestroyModuleNode(void *module_info)
{
  ExceptionInfo
    *exception;

  register ModuleInfo
    *p;

  exception=AcquireExceptionInfo();
  p=(ModuleInfo *) module_info;
  if (UnregisterModule(p,exception) == MagickFalse)
    CatchException(exception);
  if (p->tag != (char *) NULL)
    p->tag=DestroyString(p->tag);
  if (p->path != (char *) NULL)
    p->path=DestroyString(p->path);
  exception=DestroyExceptionInfo(exception);
  return(RelinquishMagickMemory(p));
}
