static char *XMLTreeTagToXML(XMLTreeInfo *xml_info,char **source,size_t *length,
  size_t *extent,size_t start,char ***attributes)
{
  char
    *content;

  const char
    *attribute;

  register ssize_t
    i;

  size_t
    offset;

  ssize_t
    j;

  content=(char *) "";
  if (xml_info->parent != (XMLTreeInfo *) NULL)
    content=xml_info->parent->content;
  offset=0;
  *source=EncodePredefinedEntities(content+start,(ssize_t) (xml_info->offset-
    start),source,length,extent,MagickFalse);
  if ((*length+strlen(xml_info->tag)+MaxTextExtent) > *extent)
    {
      *extent=(*length)+strlen(xml_info->tag)+MaxTextExtent;
      *source=(char *) ResizeQuantumMemory(*source,*extent,sizeof(**source));
      if (*source == (char *) NULL)
        return(*source);
    }
  *length+=FormatLocaleString(*source+(*length),*extent,"<%s",xml_info->tag);
  for (i=0; xml_info->attributes[i]; i+=2)
  {
    attribute=GetXMLTreeAttribute(xml_info,xml_info->attributes[i]);
    if (attribute != xml_info->attributes[i+1])
      continue;
    if ((*length+strlen(xml_info->attributes[i])+MaxTextExtent) > *extent)
      {
        *extent=(*length)+strlen(xml_info->attributes[i])+MaxTextExtent;
        *source=(char *) ResizeQuantumMemory(*source,*extent,sizeof(**source));
        if (*source == (char *) NULL)
          return((char *) NULL);
      }
    *length+=FormatLocaleString(*source+(*length),*extent," %s=\"",
      xml_info->attributes[i]);
    (void) EncodePredefinedEntities(xml_info->attributes[i+1],-1,source,length,
      extent,MagickTrue);
    *length+=FormatLocaleString(*source+(*length),*extent,"\"");
  }
  i=0;
  while ((attributes[i] != (char **) NULL) &&
         (strcmp(attributes[i][0],xml_info->tag) != 0))
    i++;
  j=1;
  while ((attributes[i] != (char **) NULL) &&
         (attributes[i][j] != (char *) NULL))
  {
    if ((attributes[i][j+1] == (char *) NULL) ||
        (GetXMLTreeAttribute(xml_info,attributes[i][j]) != attributes[i][j+1]))
      {
        j+=3;
        continue;
      }
    if ((*length+strlen(attributes[i][j])+MaxTextExtent) > *extent)
      {
        *extent=(*length)+strlen(attributes[i][j])+MaxTextExtent;
        *source=(char *) ResizeQuantumMemory(*source,*extent,sizeof(**source));
        if (*source == (char *) NULL)
          return((char *) NULL);
      }
    *length+=FormatLocaleString(*source+(*length),*extent," %s=\"",
      attributes[i][j]);
    (void) EncodePredefinedEntities(attributes[i][j+1],-1,source,length,extent,
      MagickTrue);
    *length+=FormatLocaleString(*source+(*length),*extent,"\"");
    j+=3;
  }
  *length+=FormatLocaleString(*source+(*length),*extent,*xml_info->content ?
    ">" : "/>");
  if (xml_info->child != (XMLTreeInfo *) NULL)
    *source=XMLTreeTagToXML(xml_info->child,source,length,extent,0,attributes);
  else
    *source=EncodePredefinedEntities(xml_info->content,-1,source,length,extent,
      MagickFalse);
  if ((*length+strlen(xml_info->tag)+MaxTextExtent) > *extent)
    {
      *extent=(*length)+strlen(xml_info->tag)+MaxTextExtent;
      *source=(char *) ResizeQuantumMemory(*source,*extent,sizeof(**source));
      if (*source == (char *) NULL)
        return((char *) NULL);
    }
  if (*xml_info->content != '\0')
    *length+=FormatLocaleString(*source+(*length),*extent,"</%s>",
      xml_info->tag);
  while ((content[offset] != '\0') && (offset < xml_info->offset))
    offset++;
  if (xml_info->ordered != (XMLTreeInfo *) NULL)
    content=XMLTreeTagToXML(xml_info->ordered,source,length,extent,offset,
      attributes);
  else
    content=EncodePredefinedEntities(content+offset,-1,source,length,extent,
      MagickFalse);
  return(content);
}
