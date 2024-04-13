MagickExport ExceptionInfo *DestroyExceptionInfo(ExceptionInfo *exception)
{
  if (ClearExceptionInfo(exception,MagickFalse) != MagickFalse)
    exception=(ExceptionInfo *) RelinquishMagickMemory(exception);
  return(exception);
}
