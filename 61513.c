static MagickBooleanType ValidateEntities(char *tag,char *xml,char **entities)
{
  register ssize_t
    i;

  /*
    Check for circular entity references.
  */
  for ( ; ; xml++)
  {
    while ((*xml != '\0') && (*xml != '&'))
      xml++;
    if (*xml == '\0')
      return(MagickTrue);
    if (strncmp(xml+1,tag,strlen(tag)) == 0)
      return(MagickFalse);
    i=0;
    while ((entities[i] != (char *) NULL) &&
           (strncmp(entities[i],xml+1,strlen(entities[i])) == 0))
      i+=2;
    if ((entities[i] != (char *) NULL) &&
        (ValidateEntities(tag,entities[i+1],entities) == 0))
      return(MagickFalse);
  }
}
