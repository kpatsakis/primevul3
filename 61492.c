static char *EncodePredefinedEntities(const char *source,ssize_t offset,
  char **destination,size_t *length,size_t *extent,MagickBooleanType pedantic)
{
  char
    *canonical_content;

  if (offset < 0)
    canonical_content=CanonicalXMLContent(source,pedantic);
  else
    {
      char
        *content;

      content=AcquireString(source);
      content[offset]='\0';
      canonical_content=CanonicalXMLContent(content,pedantic);
      content=DestroyString(content);
    }
  if (canonical_content == (char *) NULL)
    return(*destination);
  if ((*length+strlen(canonical_content)+MaxTextExtent) > *extent)
    {
      *extent=(*length)+strlen(canonical_content)+MaxTextExtent;
      *destination=(char *) ResizeQuantumMemory(*destination,*extent,
        sizeof(**destination));
      if (*destination == (char *) NULL)
        return(*destination);
    }
  *length+=FormatLocaleString(*destination+(*length),*extent,"%s",
    canonical_content);
  canonical_content=DestroyString(canonical_content);
  return(*destination);
}
