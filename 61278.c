static char **GetTransformTokens(void *context,const char *text,
  int *number_tokens)
{
  char
    **tokens;

  register const char
    *p,
    *q;

  register ssize_t
    i;

  SVGInfo
    *svg_info;

  svg_info=(SVGInfo *) context;
  *number_tokens=0;
  if (text == (const char *) NULL)
    return((char **) NULL);
  /*
    Determine the number of arguments.
  */
  for (p=text; *p != '\0'; p++)
  {
    if (*p == '(')
      (*number_tokens)+=2;
  }
  tokens=(char **) AcquireQuantumMemory(*number_tokens+2UL,sizeof(*tokens));
  if (tokens == (char **) NULL)
    {
      (void) ThrowMagickException(svg_info->exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",text);
      return((char **) NULL);
    }
  /*
    Convert string to an ASCII list.
  */
  i=0;
  p=text;
  for (q=p; *q != '\0'; q++)
  {
    if ((*q != '(') && (*q != ')') && (*q != '\0'))
      continue;
    tokens[i]=AcquireString(p);
    (void) CopyMagickString(tokens[i],p,(size_t) (q-p+1));
    StripString(tokens[i++]);
    p=q+1;
  }
  tokens[i]=AcquireString(p);
  (void) CopyMagickString(tokens[i],p,(size_t) (q-p+1));
  StripString(tokens[i++]);
  tokens[i]=(char *) NULL;
  return(tokens);
}
