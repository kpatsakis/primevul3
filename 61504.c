static inline MagickBooleanType IsSkipTag(const char *tag)
{
  register ssize_t
    i;

  i=0;
  while (skip_tags[i] != (const char *) NULL)
  {
    if (LocaleCompare(tag,skip_tags[i]) == 0)
      return(MagickTrue);
    i++;
  }
  return(MagickFalse);
}
