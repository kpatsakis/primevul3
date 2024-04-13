MagickExport MagickBooleanType ListLocaleInfo(FILE *file,
  ExceptionInfo *exception)
{
  const char
    *path;

  const LocaleInfo
    **locale_info;

  register ssize_t
    i;

  size_t
    number_messages;

  if (file == (const FILE *) NULL)
    file=stdout;
  number_messages=0;
  locale_info=GetLocaleInfoList("*",&number_messages,exception);
  if (locale_info == (const LocaleInfo **) NULL)
    return(MagickFalse);
  path=(const char *) NULL;
  for (i=0; i < (ssize_t) number_messages; i++)
  {
    if (locale_info[i]->stealth != MagickFalse)
      continue;
    if ((path == (const char *) NULL) ||
        (LocaleCompare(path,locale_info[i]->path) != 0))
      {
        if (locale_info[i]->path != (char *) NULL)
          (void) FormatLocaleFile(file,"\nPath: %s\n\n",locale_info[i]->path);
        (void) FormatLocaleFile(file,"Tag/Message\n");
        (void) FormatLocaleFile(file,
          "-------------------------------------------------"
          "------------------------------\n");
      }
    path=locale_info[i]->path;
    (void) FormatLocaleFile(file,"%s\n",locale_info[i]->tag);
    if (locale_info[i]->message != (char *) NULL)
      (void) FormatLocaleFile(file,"  %s",locale_info[i]->message);
    (void) FormatLocaleFile(file,"\n");
  }
  (void) fflush(file);
  locale_info=(const LocaleInfo **)
    RelinquishMagickMemory((void *) locale_info);
  return(MagickTrue);
}
