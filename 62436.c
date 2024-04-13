MagickExport char *GetMagickHomeURL(void)
{
  char
    path[MagickPathExtent];

  const char
    *element;

  ExceptionInfo
    *exception;

  LinkedListInfo
    *paths;

  exception=AcquireExceptionInfo();
  paths=GetConfigurePaths(MagickURLFilename,exception);
  exception=DestroyExceptionInfo(exception);
  if (paths == (LinkedListInfo *) NULL)
    return(ConstantString(MagickHomeURL));
  element=(const char *) GetNextValueInLinkedList(paths);
  while (element != (const char *) NULL)
  {
    (void) FormatLocaleString(path,MagickPathExtent,"%s%s%s",element,
      DirectorySeparator,MagickURLFilename);
    if (IsPathAccessible(path) != MagickFalse)
      return(ConstantString(path));
    element=(const char *) GetNextValueInLinkedList(paths);
  }
  return(ConstantString(MagickHomeURL));
}
