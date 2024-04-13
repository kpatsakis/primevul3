static char **SamplingFactorToList(const char *text)
{
  char
    **textlist;

  register char
    *q;

  register const char
    *p;

  register ssize_t
    i;

  size_t
    lines;

  if (text == (char *) NULL)
    return((char **) NULL);
  /*
    Convert string to an ASCII list.
  */
  lines=1;
  for (p=text; *p != '\0'; p++)
    if (*p == ',')
      lines++;
  textlist=(char **) AcquireQuantumMemory((size_t) lines+MaxTextExtent,
    sizeof(*textlist));
  if (textlist == (char **) NULL)
    ThrowFatalException(ResourceLimitFatalError,"UnableToConvertText");
  p=text;
  for (i=0; i < (ssize_t) lines; i++)
  {
    for (q=(char *) p; *q != '\0'; q++)
      if (*q == ',')
        break;
    textlist[i]=(char *) AcquireQuantumMemory((size_t) (q-p)+MaxTextExtent,
      sizeof(*textlist[i]));
    if (textlist[i] == (char *) NULL)
      ThrowFatalException(ResourceLimitFatalError,"UnableToConvertText");
    (void) CopyMagickString(textlist[i],p,(size_t) (q-p+1));
    if (*q == '\r')
      q++;
    p=q+1;
  }
  textlist[i]=(char *) NULL;
  return(textlist);
}
