static char **GetStyleTokens(void *context,const char *style,int *number_tokens)
{
  char
    *text,
    **tokens;

  register ssize_t
    i;

  SVGInfo
    *svg_info;

  svg_info=(SVGInfo *) context;
  (void) svg_info;
  *number_tokens=0;
  if (style == (const char *) NULL)
    return((char **) NULL);
  text=AcquireString(style);
  (void) SubstituteString(&text,":","\n");
  (void) SubstituteString(&text,";","\n");
  tokens=StringToList(text);
  text=DestroyString(text);
  for (i=0; tokens[i] != (char *) NULL; i++)
    StripStyleTokens(tokens[i]);
  *number_tokens=i;
  return(tokens);
}
