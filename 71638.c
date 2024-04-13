MagickExport ssize_t FormatLocaleString(char *magick_restrict string,
  const size_t length,const char *magick_restrict format,...)
{
  ssize_t
    n;

  va_list
    operands;

  va_start(operands,format);
  n=FormatLocaleStringList(string,length,format,operands);
  va_end(operands);
  return(n);
}
