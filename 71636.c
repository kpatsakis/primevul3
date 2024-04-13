MagickExport ssize_t FormatLocaleFile(FILE *file,
  const char *magick_restrict format,...)
{
  ssize_t
    n;

  va_list
    operands;

  va_start(operands,format);
  n=FormatLocaleFileList(file,format,operands);
  va_end(operands);
  return(n);
}
