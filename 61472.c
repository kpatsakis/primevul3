MagickExport MagickBooleanType ModuleComponentGenesis(void)
{
  ExceptionInfo
    *exception;

  MagickBooleanType
    status;

  if (module_semaphore == (SemaphoreInfo *) NULL)
    module_semaphore=AllocateSemaphoreInfo();
  exception=AcquireExceptionInfo();
  status=IsModuleTreeInstantiated(exception);
  exception=DestroyExceptionInfo(exception);
  return(status);
}
