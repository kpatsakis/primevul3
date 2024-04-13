MagickExport const char *GetLocaleMessage(const char *tag)
{
  char
    name[MagickLocaleExtent];

  const LocaleInfo
    *locale_info;

  ExceptionInfo
    *exception;

  if ((tag == (const char *) NULL) || (*tag == '\0'))
    return(tag);
  exception=AcquireExceptionInfo();
  (void) FormatLocaleString(name,MagickLocaleExtent,"%s/",tag);
  locale_info=GetLocaleInfo_(name,exception);
  exception=DestroyExceptionInfo(exception);
  if (locale_info != (const LocaleInfo *) NULL)
    return(locale_info->message);
  return(tag);
}
